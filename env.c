#include <stdio.h>

extern char **environ;

int main(void)
{
    for (int k = 0; environ[k] != NULL; k++) {
        printf("%s\n", environ[k]);
    }
}
