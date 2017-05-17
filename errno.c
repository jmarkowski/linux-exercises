#include <stdio.h>
#include <errno.h>      /* for errno */
#include <string.h>     /* for strerror */
#include <stdlib.h>     /* for exit */

int main(void)
{
    int k;

    for (k = 0; k < 256; k++) {
        errno = k;

        printf("errno (%03d) = %s\n", errno, strerror(errno));
    }

    exit(0);
}
