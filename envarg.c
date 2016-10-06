#include <stdio.h>

extern char **environ;

int main(int argc, char *argv[])
{
    int k;

    for (k = 0; k < argc; k++) {
        printf("argv[%d] = %s\n", k, argv[k]);
    }

    for (k = 0; environ[k] != NULL; k++) {
        printf("%s\n", environ[k]);
    }

    return 0;
}
