#include <stdio.h>
#include <unistd.h>

static int globalInt;

void childFunc(pid_t pid)
{
    printf("Child work (pid = %d) (globalInt = %d)\n", pid, globalInt);
}

void parentFunc(pid_t pid)
{
    printf("Parent work (pid = %d) (globalInt = %d)\n", pid, globalInt);
}

int main(void)
{
    pid_t pid;

    printf("Program start\n");

    globalInt++;

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
    pid = fork();

    if (pid == 0) {     /* child */
        globalInt++;
        childFunc(pid);
    } else {            /* parent */
        globalInt++;
        parentFunc(pid);
    }

    globalInt++;

    printf("DONE (pid = %d) (gint=%d)\n", pid, globalInt);

    return 0;
}
