#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main(void)
{
    char ctermid_name[L_ctermid];

    ctermid(ctermid_name);

    /*
     * Most UNIX systems use /dev/tty as the ame of the controlling terminal, so
     * this function is intended to aid portability to other operating systems.
     */
    printf("The name of the controller terminal is: \"%s\" "
           "(max len = %d)\n\n", ctermid_name, L_ctermid);

    printf("Testing to see which file descriptors refer to a terminal:\n");
    printf("fd 0: %s\n", isatty(STDIN_FILENO) ? "tty" : "not a tty");
    printf("fd 1: %s\n", isatty(STDOUT_FILENO) ? "tty" : "not a tty");
    printf("fd 2: %s\n", isatty(STDERR_FILENO) ? "tty" : "not a tty");

    printf("\nUse redirection (e.g. ./ttyinfo.o < /etc/passwd 2> /dev/null)\n"
           "to see how the results may change\n");

    return 0;
}
