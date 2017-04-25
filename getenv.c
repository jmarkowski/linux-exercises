#include <stdio.h>
#include <string.h>

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

int main(void)
{
    char * envstr;

    envstr = getenv("HOME");
    printf("NON-REENTRANT: %s\n", envstr);

}
