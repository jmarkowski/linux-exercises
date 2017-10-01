#include <stdio.h>
#include <signal.h>         /* for sigaction */
#include <unistd.h>         /* for setsid */
#include <stdlib.h>         /* for exit */
#include <sys/stat.h>       /* for umask */
#include <sys/resource.h>   /* for getrlimit */
#include <fcntl.h>          /* for open */
#include <syslog.h>         /* for openlog */
#include <unistd.h>         /* for getpid */
#include <string.h>         /* for strlen */
#include <errno.h>          /* for EACCES */

/**
 * Daemon conventions
 *
 * 1. Lock files are typically created and stored in /var/run (as su) with the
 *    file name as x.pid, where x is the name of the daemon.
 *
 * 2. Configuration options are typically stored in /etc as x.conf, where x is
 *    the name of daemon.
 *
 * 3. Daemons are typically started from a system initialization script in
 *    /etc/systemd/
 *
 * 4. If a conf file exists, the daemon reads the file when it starts. Changes
 *    to conf typically requires daemon restart.
 *    Typically, to avoid restarting, SIGHUP signals are used to tell the daemon
 *    to reread the conf file.
 */

char abortStr[80];
#define ABORT(...) \
        sprintf(abortStr, __VA_ARGS__); \
        perror(abortStr); \
        exit(1);

#define LOCKFILE "/tmp/daemon.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

/*
 * The purpose of this function is to ensure that the daemon is a single
 * instance
 */
int alreadyRunning(void)
{
    int fd;
    char buf[16];

    fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);

    if (fd < 0) {
        syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
        return 1;
    }

    if (lockf(fd, F_TEST, 8) == -1) {
        syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
        return 1;
    }

    if (lockf(fd, F_TLOCK, 8) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            close(fd);
            return 1;
        }

        syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
        exit(1);
    }

    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long) getpid());
    write(fd, buf, strlen(buf) + 1);
    return 0;
}

/*
 * Daemons must be coded according to a few rules that prevent any unwanted
 * interactions from happening.
 */

void daemonize(const char *cmd)
{
    struct rlimit       fileLimit;
    struct sigaction    sa;
    pid_t               pid;
    int k, fd0, fd1, fd2;
    mode_t prevUMask, newUMask = 0;

    /*
     * Initialize the log file.
     *
     * You can read the contents with $ journalctl -xe
     *
     * Valid options:
     *  LOG_CONS
     *  LOG_NDELAY
     *  LOG_NOWAIT
     *  LOG_ODELAY
     *  LOG_PERROR
     *  LOG_PID
     *
     * Valid facility:
     *  LOG_DAEMON (among others)
     */
    openlog(cmd, LOG_CONS, LOG_DAEMON);

    /*
     * Clear file creation mask and set it to a known value.
     */
    prevUMask = umask(newUMask);

    printf("umask change: %03o -> %03o\n", prevUMask, newUMask);

    /*
     * Get the maximum number of file descriptors.
     */
    if (getrlimit(RLIMIT_NOFILE, &fileLimit) < 0) {
        ABORT("%s: can't get file limit", cmd);
    }

    /*
     * Create a new session and become the session leader to lose controlling
     * TTY.
     *
     * A session is a collection of one or more process groups. e.g. calling:
     * $ proc1 | proc2    # This is a process group consisting of proc1 and
     *                    # proc2
     *
     * Calling setsid() causes three things:
     * 1. The forked child process becomes a session leader.
     * 2. The process will become the process group leader of a new process
     *    group.
     * 3. The process has no controlling terminal (if it had one before the
     *    call, the association between it and the terminal is broken)
     */
    if ((pid = fork()) < 0) {
        ABORT("%s: can't fork", cmd);
    } else if (pid != 0) { /* parent */
        syslog(LOG_INFO, "Parent [%d] closed (child [%d] lives on)\n", getpid(), pid);
        exit(0);
    }

    setsid();

    /*
     * Ensure future opens won't allocate controlling TTYs. We must fork again!
     */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        ABORT("%s: can't ignore SIGHUP", cmd);
    }

    if ((pid = fork()) < 0) {
        ABORT("%s: can't fork", cmd);
    } else if (pid != 0) { /* parent */
        syslog(LOG_INFO, "Parent [%d] closed (child [%d] lives on)", getpid(),
               pid);
        exit(0);
    }

    /*
     * The remaining code in a child process as a daemon. It owns no TTY.
     */

    /*
     * Change the current working directory to the root so we won't prevent file
     * systems from being unmounted.
     */
    if (chdir("/") < 0) {
        ABORT("%s: can't change directory to /", cmd);
    }

    /*
     * Close all open file descriptors
     */
    if (fileLimit.rlim_max == RLIM_INFINITY) {
        fileLimit.rlim_max = 1024;
    }

    for (k = 0; k < fileLimit.rlim_max; k++) {
        close(k);
    }

    /*
     * Now that all the file descriptors are closed, attach file descriptors
     * 0, 1, and 2 (i.e. STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO) to
     * /dev/null.
     *
     * The reason for this is that there is nowhere for output to be displayed,
     * nor is there anywhere to receive input from an interactive user. The
     * daemon runs in the background.
     */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0); /* Get the next incremental available fd, duplicate of fd0 */
    fd2 = dup(0); /* Get the next incremental available fd, duplicate of fd0 */

    syslog(LOG_INFO, "DAEMON: file descriptors: %d, %d, %d", fd0, fd1, fd2);

     if (fd0 != STDIN_FILENO || fd1 != STDOUT_FILENO || fd2 != STDERR_FILENO) {
        syslog(LOG_ERR, "DAEMON: Unexpected file descriptors %d %d %d",
               fd0, fd1, fd2);
        exit(1);
    }

    /*** Daemon initialization is finished ***/

}

void daemonTask(void)
{
    syslog(LOG_INFO, "Run daemon (sleep 30 seconds) (%s is %d)",
           LOCKFILE, getpid());

    sleep(30); /* Run daemon for 30 seconds */

    /*
     * %m is replaced with error message string corresponding to value of errno
     */
    syslog(LOG_INFO, "DAEMON: DONE! (errno = %m)");
}

int main(int argc, char *argv[])
{
    printf("Type 'journal -xe' to see DAEMON logs\n");

    daemonize("DAEMON");

    if (alreadyRunning()) {
        syslog(LOG_ERR, "Aborting since daemon is already running");
        exit(1);
    }

    daemonTask();

    exit(0);
}
