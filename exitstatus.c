#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void process_exit(int status)
{
    if (WIFEXITED(status)) {
        /*
         * True if status has returned for a child that terminanted normally.
         * This allows us to use WEXITSTATUS to fetch the low-order 8 bits of
         * the argument that the child passed to exit, _exit, or _Exit
         */
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        /*
         * True if status has returned for a child that terminated abnormally,
         * by receipt of a signal that it didn't catch. In this case we can
         * execute WTERMSIG to fetch the signal number that caused termination.
         */
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status),
#ifdef WCOREDUMP
                WCOREDUMP(status) ? " (core file generated)" : ""
#else
                ""
#endif
        );
    } else if (WIFSTOPPED(status)) {
        /*
         * True if status has returned for a child that is currently stopped. In
         * this case we can execute WSTOPSIG to fetch the signal number
         * that caused the child to stop.
         */
        printf("child stopped, signal number %d\n", WSTOPSIG(status));
    }
}

int main(void)
{
    pid_t pid;
    int status;

    {
        pid = fork();

        if (pid < 0) {
            printf("fork error");
            exit(1);
        } else if (pid == 0) {      /* child */
            exit(7);
        }
        /* wait / waitpid returns process ID if OK, 0, or -1 if error */
        if (wait(&status) != pid) { /* wait for child */
            printf("fork error");
            exit(1);
        }

        process_exit(status);
    }

    {
        pid = fork();

        if (pid < 0) {
            printf("fork error");
            exit(1);
        } else if (pid == 0) {
            abort(); /* generates SIGABRT */
        }

        if (wait(&status) != pid) {
            printf("fork error");
            exit(1);
        }

        process_exit(status);
    }

    {
        pid = fork();
        int zero = 0;

        if (pid < 0) {
            printf("fork error");
            exit(1);
        } else if (pid == 0) {
            status /= zero; /* divide by 0 generates SIGFPE */
        }

        if (wait(&status) != pid) {
            printf("fork error");
            exit(1);
        }

        process_exit(status);
    }

    exit(0);
}
