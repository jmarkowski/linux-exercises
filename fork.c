#include <stdio.h>
#include <unistd.h>

static int k;

void childFunc(pid_t pid)
{
    printf("[%d] (%d) Child fork (pid = %d.. useless now, "
           "this was just a signal to know which PID is the child)\n",
           getpid(), ++k, pid);
}

void parentFunc(pid_t pid)
{
    printf("[%d] (%d) Parent fork (child pid = %d)\n", getpid(), ++k, pid);
}

int main(void)
{
    printf("[%d] (%d) Program start\n", getpid(), ++k);

    /*
     * By flushing stdout, when the output is piped to a file, the output order
     * is the same as when printf'ing to the terminal.
     *
     * If we don't flush, and we pipe the output to a file, the contents of the
     * stdout buffer are copied from parent to child and will be written the
     * same.
     */
#if 1
    fflush(stdout);
#endif

    /*
     * fork():
     * returns 0 in child process, the process ID of child in parent process, or
     * -1 on error
     */
    pid_t pid;

    pid = fork();

    if (pid == 0) {
        childFunc(pid);
    } else {
        parentFunc(pid);
    }

    if (pid == 0) {
        printf("[%d] (%d) Child Program finished\n", getpid(), ++k);
    } else {
        printf("[%d] (%d) Parent Program finished\n", getpid(), ++k);
    }

    return 0;
}
