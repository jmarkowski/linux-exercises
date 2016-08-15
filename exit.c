#include <stdlib.h>     /* for exit */
#include <stdio.h>      /* for printf */

static int exitCount = 0;

void exitMsg1(void)
{
    printf("%d: Exit message 1\n", exitCount);
    exitCount++;
}

void exitMsg2(void)
{
    printf("%d: Exit message 2\n", exitCount);
    exitCount++;
}

void exitMsg3(void)
{
    printf("%d: Exit message 3\n", exitCount);
    exitCount++;
}

int main(void)
{
    atexit(exitMsg1);
    atexit(exitMsg2);
    atexit(exitMsg3);

    atexit(exitMsg1);
    atexit(exitMsg1);
    atexit(exitMsg1);

    /*
     * "exit" will close all IO streams and perform clean up when exiting before
     * giving control back to the kernel (e.g. closing open file streams)
     *
     * Whereas, "_exit" or "_Exit" will skip any cleanup and return straight to
     * the kernel.
     */
    exit(0);
}
