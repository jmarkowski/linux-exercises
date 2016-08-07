#include <stdio.h>

#define INPUT_SIZE 10

int main(void)
{
    char buf[INPUT_SIZE];
    char c;

    setvbuf(stdin, buf, _IOLBF, INPUT_SIZE * sizeof(buf[0]));

    do {
        c = getc(stdin);
        putc(c, stdout);
    } while (c != '\n');

    return 0;
}
