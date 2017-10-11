#include <fcntl.h>      /* for open */
#include <stdio.h>      /* for printf */
#include <stdlib.h>     /* for exit */
#include <string.h>     /* for memcpy */
#include <sys/stat.h>   /* for fstat */
#include <sys/mman.h>   /* for mmap */
#include <unistd.h>     /* for ftruncate */

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define COPY_INCREMENT (1024 * 1024 * 1024)  /* 1 GB */

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("usage: %s <fromfile> <tofile>\n", argv[0]);
        exit(0);
    }

    int fdin, fdout;

    if ((fdin = open(argv[1], O_RDONLY)) < 0) {
        printf("Cannot open %s for reading\n", argv[1]);
        exit(1);
    }

    if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) {
        printf("Cannot create %s for writing\n", argv[2]);
        exit(1);
    }

    struct stat sbuf;

    /* Fetch size of input file */
    if (fstat(fdin, &sbuf) < 0) {
        printf("fstat error\n");
        exit(1);
    }

    off_t srcSize = sbuf.st_size; /* size in bytes */

    /* Set output file size to match the input file size */
    if (ftruncate(fdout, srcSize) < 0) {
        printf("ftruncate error\n");
        exit(1);
    }

    off_t dstSize = 0;

    size_t copySize;
    void *src;
    void *dst;

    while (dstSize < srcSize) {
        if ((srcSize - dstSize) > COPY_INCREMENT) {
            copySize = COPY_INCREMENT;
        } else {
            copySize = srcSize - dstSize;
        }

        /* Create memory mapped IO regions of memory */

        off_t offset = dstSize;

        src = mmap(0, copySize, PROT_READ,
                   MAP_SHARED, fdin, offset);

        dst = mmap(0, copySize, PROT_READ | PROT_WRITE,
                   MAP_SHARED, fdout, offset);

        if (src == MAP_FAILED || dst == MAP_FAILED) {
            printf("mmap error for input or output\n");
            exit(1);
        }

        memcpy(dst, src, copySize);

        /* Done for this memory region */
        munmap(src, copySize);
        munmap(dst, copySize);

        /* Our output file size grew by the copied size */
        dstSize += copySize;
    }

    exit(0);
}
