#include <stdio.h>
#include <fcntl.h>  /* for open */
#include <unistd.h> /* for read */
#include <stdlib.h> /* for exit */

/**
 * This file simply demonstrates the differences between open a file using the
 * file descriptor method vs the file pointer method.
 *
 * Using file descriptors is a form of "unbuffered I/O", which means that for
 * every read/write function call, this is a system call to the kernel, and
 * we're guaranteeing that the I/O occurs before continuing.
 *
 * Use file pointers is a form of "buffered I/O", which typically revolves
 * around the notion of "streams". Buffered I/O is less disk intensive and
 * allows the OS to buffer the read/write operations before they are carried out
 * (or if they are flushed).
 *
 * When should one use buffered vs unbuffered I/O?
 *
 * Basically, you'll want to always use buffered I/O, unless you require data
 * that is read or written to take place immediately (think of the stderr
 * stream). If you're logging data, and an error occurs, you'd like the error to
 * be logged before aborting from the program.
 */

#define SIZE 80

/**
 * Open a file using the file descriptor functions
 */
void fileDescriptor(char *targetFile)
{
    int fd;
    char errstr[30];

    printf("File Descriptor Method:\n");

    fd = open(targetFile, O_RDONLY);

    if (fd < 0) {
        sprintf(errstr, "Failed to read %s", targetFile);
        perror(errstr);
    } else {
        char buf[SIZE] = { 0 };
        printf("Successfully opened %s using fd=%d:\n", targetFile, fd);
        if (read(fd, (void *) buf, 18) == 18) { /* read returns bytes read */
            printf("%s\n", buf);
        }
    }

    if (close(fd) != 0) {
        sprintf(errstr, "Failed to close %s", targetFile);
        perror(errstr);
    }

    printf("\n");
}

/**
 * Open a file using the file pointer functions
 */
void filePointer(char *targetFile)
{
    FILE *fp;
    char errstr[30];
    char buf[SIZE] = { 0 };

    printf("File Pointer Method:\n");

    fp = fopen(targetFile, "r");

    /* Must call after open stream, but before any operation */
    if (fp && setvbuf(fp, buf, _IOLBF, 10) == 0) {
        printf("Configured for line buffered IO of size 10\n");
    } else {
        sprintf(errstr, "Failed to configure buffer");
        perror(errstr);
    }

    if (fp == NULL) {
        sprintf(errstr, "Failed to read %s", targetFile);
        perror(errstr);
    } else {
        printf("Successfully opened %s using fp=%8p (fd=%d):\n", targetFile, fp,
                                                                    fileno(fp));
        if (fgets(buf, 18, fp) != NULL) {
            printf("%s\n", buf);
        }
    }

    if (fp && fclose(fp) == EOF) {
        sprintf(errstr, "Failed to close %s", targetFile);
        perror(errstr);
    }

    printf("\n");
}

int main(void)
{
    char badFile[]  = "badfile";
    char goodFile[] = "fdvsfp.c";

    /* Read a file using a file descriptor */
    fileDescriptor(badFile);
    fileDescriptor(goodFile);

    /* Read a file using a file pointer */
    filePointer(badFile);
    filePointer(goodFile);

    return 0;
}
