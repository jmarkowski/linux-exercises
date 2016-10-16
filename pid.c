#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

/*
 * Hang-up signal handler
 */
static void sighup(int signo)
{
    printf("SIGHUP (%d) received, pid = %ld\n", signo, (long) getpid());
}

static void printProcessIds(const char *name)
{
    printf("%8s: pid = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n",
           name,
           (long) getpid(),
           (long) getppid(),
           (long) getpgrp(),
           (long) tcgetpgrp(STDIN_FILENO));

    fflush(stdout);
}

int main(void)
{
    char c;
    pid_t pid;

    printf("pid   = process id\n"
           "ppid  = parent process id\n"
           "pgrp  = process group id\n"
           "tpgrp = terminal foreground process group\n\n");
    printProcessIds("parent");

    pid = fork();

    if (pid < 0) {
        printf("fork error\n");
        exit(1);
    } else if (pid > 0) { /* parent */
        /* Crude way to allow the child to execute before parent finishes */
        sleep(5);
    } else { /* child */
        printProcessIds("child");

        /*
         * Register the hang-up signal handler, otherwise process will terminate
         * when the kill signal comes in.
         */
        signal(SIGHUP, sighup);

        kill(getpid(), SIGTSTP); /* stop child */

        printProcessIds("child");

        if (read(STDIN_FILENO, &c, 1) != 1) {
            printf("read error %d on controllying TTY\n", errno);
        }
    }

    exit(0);
}
