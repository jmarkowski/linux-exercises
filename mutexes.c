#include <stdlib.h>     /* for exit */
#include <stdio.h>      /* for perror */
#include <pthread.h>

static int incCount = 0;
static int decCount = 0;

typedef struct {
    int count;
    int incCount;
    int decCount;
    pthread_mutex_t lock;
    int id;
} foo_t;

foo_t *fooInit(int id)
{
    foo_t *foo = (foo_t *) malloc(sizeof(foo_t));

    if (foo) {
        foo->id = id;
        foo->count = 1;
        pthread_mutex_init(&foo->lock, NULL);
    }

    return foo;
}

void fooInc(foo_t *foo)
{
    if (foo) {
        pthread_mutex_lock(&foo->lock);
        printf("+");
        foo->count++;
        incCount++;
        pthread_mutex_unlock(&foo->lock);
    }
}

void fooDec(foo_t *foo)
{
    if (foo) {
        pthread_mutex_lock(&foo->lock);
        printf("-");
        foo->count--;
        decCount++;
        pthread_mutex_unlock(&foo->lock);
    }
}

int fooCount(foo_t *foo)
{
    int count = 0;

    if (foo) {
        pthread_mutex_lock(&foo->lock);
        count = foo->count;
        pthread_mutex_unlock(&foo->lock);
    }

    return count;
}

void fooDestroy(foo_t *foo)
{
    if (foo) {
        pthread_mutex_unlock(&foo->lock);
        pthread_mutex_destroy(&foo->lock);
        free(foo);
    }
}


static foo_t *globalFoo;

/*
 * The addThread will increment foo->count until it's 3
 */
void *addThread(void *arg)
{
    while (fooCount(globalFoo) < 10) {
        fooInc(globalFoo);
    }

    pthread_exit((void *) 0);
}

/*
 * The subThread will decrement foo->count until it's 0
 */
void *subThread(void *arg)
{
    while (fooCount(globalFoo) > 0) {
        fooDec(globalFoo);
    }

    pthread_exit((void *) 0);
}

int main(void)
{
    pthread_t addThreadId;
    pthread_t subThreadId;
    void *retval;

    globalFoo = fooInit(1);

    pthread_create(&addThreadId, NULL, addThread, NULL);
    pthread_create(&subThreadId, NULL, subThread, NULL);

    pthread_join(addThreadId, &retval);
    pthread_join(subThreadId, &retval);

    printf("\nFinal tally:\n");
    printf("Inc Count = %d\n", incCount);
    printf("Dec Count = %d\n", decCount);

    fooDestroy(globalFoo);

    exit((long) retval);
}
