#include <stdio.h>
#include <stdlib.h>     /* for exit */
#include <unistd.h>     /* for fork, execle */
#include <sys/wait.h>   /* for wait */

/*
 * execl    - exec with list of args
 * execv    - exec with vector of args
 * exele    - exec with list of args and specified environment
 * execve   - exec with vector of args and specified environment
 * execlp   - exec with list of args and filename from PATH environment
 * execvp   - exec with vector of args and filename from PATH environment
 * fexecve  - exec with vector of args and specified environment, using file
 *            descriptor
 *      All return -1 on error, no return on success
 *
 *  execlp               execl              execle
 *    |build argv          |build argv        |build argv
 *    |                    |                  |
 *    V      try each      V      use         V
 *  execvp ----------->  execv -----------> execve
 *          PATH prefix         environ  (system call)
 *                                            ^
 *                                            | build path from
 *                                            | /proc/self/fd alias
 *                                            |
 *                                         fexecve
 *
 */

#define BLOCKED_BY_CHILD 1

int main(void)
{
    pid_t pid;
    int waitOpts = (BLOCKED_BY_CHILD) ? 0 : WNOHANG;

    pid = fork();

    if (pid < 0) {
        printf("fork error\n");
    } else if (pid == 0) { /* child */
        char * env_init[] = { "USER=unknown", "PATH=/tmp", NULL };
        if (execle("./envarg.o",/* pathname */
                   "envarg.o",  /* arg0, doesn't appear necessary */
                   "one",       /* arg1 */
                   "two",       /* arg2 */
                   (char *) 0,   /* null pointer */
                   env_init) < 0) {
            printf("execle error\n");
            exit(1);
        }
    }

    if (waitpid(pid, NULL, waitOpts) < 0) {
        printf("waitpid error\n");
    }

    pid = fork();

    char cwd[255];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        setenv("PATH", "/home/markowski/github/linux-exercises.git", 1);
    } else {
        printf("Unable to get the current working directory\n");
        exit(1);
    }

    if (pid < 0) {
        printf("fork error\n");
    } else if (pid == 0) { /* child */
        if (execlp("envarg.o", /* filename, must be in PATH */
                   "envarg.o", /* arg0, doesn't appear necessary */
                   ".", /* cwd */
                   (char *) 0) < 0) {
            printf("execlp error\n");
            exit(1);
        }
    }

    printf("\033[1;31mParent finished\033[0m\n");
    exit(0);
}
