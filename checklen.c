#include <stdlib.h>     /* for exit, atoi */
#include <stdio.h>

#define MAXCOL 1024

/**
 * From a given filename, specify the maximum line length to find
 */
static unsigned countLineLenViolations(char *filename, unsigned maxLineLen)
{
    unsigned k;
    unsigned line = 0;
    unsigned numViolations = 0;
    char buf[MAXCOL];
    FILE *fp;

    fp = fopen(filename, "r");

    while (fgets(buf, MAXCOL, fp) != NULL) {
        line++;
        for (k = 0; k < MAXCOL; k++) {
            if (buf[k] == '\0') {   /* NUL character for end of line */
                if (k - 1 > maxLineLen) {
                    printf("Line %d (len=%d): %s", line, k - 1, buf);
                    numViolations++;
                }
                break;
            }
        }

        if (k == MAXCOL) {
            printf("LINE LENGTH EXCEEDED MAX BUFFER OF %d!\n", MAXCOL);
            numViolations++;
        }
    }

    return numViolations;
}

int main(int argc, char **argv)
{
    unsigned maxLineLen = 0;
    unsigned numViolations;
    char *filename;

    if (argc < 3) {
        printf("Usage: %s <file> <#max-len>\n", argv[0]);
        exit(1);
    }

    filename   = argv[1];
    maxLineLen = atoi(argv[2]);

    numViolations = countLineLenViolations(filename, maxLineLen);

    printf("Number of lines exceeding %u: %u\n", maxLineLen, numViolations);

    return 0;
}
