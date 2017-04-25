#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

extern char **environ;

/*******************************************************************************
 * Non-reentrant implementation of getenv
 ******************************************************************************/
#define MAX_STRING_SIZE 4096

static char envbuf[MAX_STRING_SIZE];

char * getenv(const char *name)
{
    int k, len;

    len = strlen(name);

    for (k = 0; environ[k] != NULL; k++) {
        /* return the value after the '=' */
        if ((strncmp(name, environ[k], len) == 0) && (environ[k][len] == '=')) {
            strncpy(envbuf, &environ[k][len+1], MAX_STRING_SIZE - 1);
            return envbuf;
        }
    }

    return NULL;
}

/*******************************************************************************
 * Reentrant implementation of getenv
 ******************************************************************************/
pthread_mutex_t envMtx;

static pthread_once_t initDone = PTHREAD_ONCE_INIT;

static void threadInit(void)
{
    pthread_mutexattr_t attr;

    /* Configure the mutex attributes */
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

    /* Initialize the environment mutex with given attributes */
    pthread_mutex_init(&envMtx, &attr);

    pthread_mutexattr_destroy(&attr);
}

int getenv_r(const char *name, char *buf, int buflen)
{
    int k, len, envValLen;

    /* The following is called only once, ever */
    pthread_once(&initDone, threadInit);

    len = strlen(name);

    pthread_mutex_lock(&envMtx);

    for (k = 0; environ[k] != NULL; k++) {
        /* return the value after the '=' */
        if ((strncmp(name, environ[k], len) == 0) && (environ[k][len] == '=')) {
            envValLen = strlen(&environ[k][len+1]);

            if (envValLen >= buflen) {
                pthread_mutex_unlock(&envMtx);
                return ENOSPC;  /* No space */
            }

            strcpy(buf, &environ[k][len+1]);
            pthread_mutex_unlock(&envMtx);
            return 0;
        }
    }

    pthread_mutex_unlock(&envMtx);
    return ENOENT; /* Doesn't exist */
}

/******************************************************************************/
int main(void)
{
    char * envstr;

    envstr = getenv("HOME");
    printf("NON-REENTRANT: %s\n", envstr);

    char buf[20];

    if (getenv_r("HOME", buf, 20) == 0) {
        printf("REENTRANT    : %s\n", envstr);
    }
    else {
        printf("REENTRANT    : FAILED\n");
    }

    return 0;
}
