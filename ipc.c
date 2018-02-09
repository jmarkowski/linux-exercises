#include <stdio.h>      /* for printf */
#include <stdlib.h>     /* for exit */
#include <pthread.h>
#include <unistd.h>     /* for sleep */

/*
 * In this example two threads are created and communication between the threads
 * is performed using one of the three IPC methods.
 */

/*
 * There are three types of IPC (interprocess communications) that we call XSI
 * IPC:
 *  - message queues
 *  - semaphores
 *  - shared memory
 *
 * Each IPC structure is referred to by a non-negative integer identifier.
 */

void *thread_1hz(void *arg)
{
    printf("Started %s\n", __FUNCTION__);

    while (1) {
        printf("Thread 1\n");
        sleep(1);
    }

    return (void *) 1;
}

void *thread_0_5hz(void *arg)
{
    printf("Started %s\n", __FUNCTION__);

    while (1) {
        printf("Thread 2\n");
        sleep(2);
    }

    return (void *) 99;
}

void thread_start(pthread_t *id, void *(*thread_fn)(void *arg))
{
    int err = pthread_create(id, NULL, thread_fn, NULL);

    if (err) {
        perror("Thread creation failed");
        exit(1);
    }
}

void thread_stop(pthread_t *id)
{
    void *retval;

    pthread_join(*id, &retval);

    printf("Thread exit code: %ld\n", (long) retval);
}

int main(void)
{
    pthread_t thread_id_1hz;
    pthread_t thread_id_2hz;

    thread_start(&thread_id_1hz, thread_1hz);
    thread_start(&thread_id_2hz, thread_0_5hz);

    sleep(1);

    thread_stop(&thread_id_1hz);
    thread_stop(&thread_id_2hz);

    exit(0);
}
