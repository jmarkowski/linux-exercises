#include <stdio.h>      /* for perror */
#include <stdlib.h>     /* for exit */
#include <unistd.h>     /* for sleep */
#include <pthread.h>

void printThreadInfo(const char *name)
{
    pid_t     pid = getpid();
    pthread_t tid = pthread_self();

    printf("%s:\n", name);

    /*
     * Because this is a single process launching multiple threads, the process
     * ID should be the same for all threads
     */
    printf(" process id = %lu\n", (unsigned long) pid);
    printf(" thread id  = %lu (0x%lx)\n", (unsigned long) tid,
                                          (unsigned long) tid);

    printf("\n");
}

void *thread1(void * arg)
{
    printThreadInfo(__FUNCTION__);
    return NULL;
}

int main(void)
{
    int err;
    pthread_t idThread1;

    err = pthread_create(&idThread1, NULL, thread1, NULL);

    if (err != 0) {
        perror("Thread creation failed");
        exit(1);
    }

    printThreadInfo(__FUNCTION__);

    /*
     * Sleep the main thread, otherwise the new thread may not get a chance to
     * run
     */
    sleep(1);

    /*
     * Calling exit, _Exit, or _exit will kill the entire process, not just a
     * thread, even if called by "child" threads.
     */
    exit(0);
}
