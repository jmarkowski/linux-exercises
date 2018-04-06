#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main(void)
{
    struct termios term;

    int term_fd = STDIN_FILENO;

    /* Check if the file descriptor is a terminal */
    if (isatty(term_fd) == 0) {
        printf("%d does not refer to a terminal\n", term_fd);
        exit(1);
    }

    int status = 0;

    /* Fetch the POSIX VDISABLE value using fpathconf */
    long vdisable = fpathconf(term_fd, _PC_VDISABLE);

    /* Get the terminal attributes */
    status = tcgetattr(term_fd, &term);

    if (status < 0) {
        printf("tcgetattr error\n");
        exit(1);
    }

    /* Modify the terminal control characters */
    term.c_cc[VINTR] = vdisable;    /* disable INTR character */
    term.c_cc[VEOF] = 2;            /* EOF is Control-B now */

    /*
     * Set the terminal attributes
     *
     * With TDCSANOW the change occurs immediately.
     *
     * With TCSADRAIN the change occurs after all output has been transmitted.
     * (use only if changing the output parameters)
     *
     * With TCSAFLUSH, the change occurs after all output has been transmitted.
     *      Furthermore, when the change takes place, all input data that has
     *      not been read is discarded (flushed).
     */
    status = tcsetattr(term_fd, TCSAFLUSH, &term);

    if (status < 0) {
        printf("tcsetattr error\n");
        exit(1);
    }

    printf("If you try pressing Ctrl-C now, nothing will happen! "
           "(normally interrupts control)\n");
    printf("If you try pressing Ctrl-D now, nothing will happen! "
           "(normally kills terminal -> remapped to Ctrl-B)\n");

    exit(0);
}
