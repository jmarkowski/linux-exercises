#include <stdio.h>      /* for fgets, popen */
#include <stdlib.h>     /* for exit */
#include <ctype.h>      /* for toupper */

/*
 * This program simply opens a pipe to a command and capitalizes every
 * character.
 */

#define MAXLINE 4096

int main(void)
{

    FILE *fpin;
    char *cmd = "ls -1 *.c";
    char line[MAXLINE];

    /*
     * 'r' means that fp is connected to stdout of command
     * 'w' means that fp is connected to stdin of command
     *
     * The command string is executed by the Bourne shell, as in:
     *     sh -c cmd
     */
    fpin = popen(cmd, "r");

    while (fgets(line, MAXLINE, fpin) != NULL) {
        for (int k = 0; k < MAXLINE && line[k] != '\0'; k++) {
            char c = line[k];

            if (islower(c)) {
                c = toupper(c);
            }
            printf("%c", c);
        }
    }

    exit(0);
}
