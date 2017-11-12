#include <stdio.h>
#include <stdlib.h>     /* for exit */
#include <string.h>     /* for strlen */
#include <unistd.h>     /* for pipe */

static char msg[] = "Hello world\n";

#define MAXLINE 24

int main(void)
{
    int n;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];

    if (pipe(fd) < 0) {
        printf("pipe error\n");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        printf("fork error\n");
        exit(1);
    }

    if (pid > 0) { /* parent */
        close(fd[0]);
        write(fd[1], msg, strlen(msg));
    } else { /* child */
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }

    exit(0);
}
