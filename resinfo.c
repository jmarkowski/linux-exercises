#include <stdio.h>
#include <sys/resource.h>
#include <stdlib.h>


static void printResInfo(char *name, int resource)
{
    struct rlimit limit;
    unsigned long long lim;

    if (getrlimit(resource, &limit) < 0) {
        printf("getrlimit error for %s\n", name);
        perror("Failure");
        exit(1);
    }

    printf("%-20s  ", name);

    if (limit.rlim_cur == RLIM_INFINITY) {
        printf("(infinite)  ");
    } else {
        lim = limit.rlim_cur;
        printf("%10llu  ", lim);
    }

    if (limit.rlim_max == RLIM_INFINITY) {
        printf("(infinite)");
    } else {
        lim = limit.rlim_max;
        printf("%10llu", lim);
    }

    putchar((int)'\n');
}

#define resInfo(resource) printResInfo(#resource, resource)

int main(void)
{
    printf("Resource limits:\n");
    resInfo(RLIMIT_AS);
    resInfo(RLIMIT_CORE);
    resInfo(RLIMIT_CPU);
    resInfo(RLIMIT_DATA);
    resInfo(RLIMIT_FSIZE);
    resInfo(RLIMIT_MEMLOCK);
    resInfo(RLIMIT_MSGQUEUE);
    resInfo(RLIMIT_NICE);
    resInfo(RLIMIT_NOFILE);
    resInfo(RLIMIT_NPROC);
#ifdef RLIMIT_NPTS
    resInfo(RLIMIT_NPTS);
#endif
    resInfo(RLIMIT_RSS);
#ifdef RLIMIT_SBSIZE
    resInfo(RLIMIT_SBSIZE);
#endif
    resInfo(RLIMIT_SIGPENDING);
    resInfo(RLIMIT_STACK);
#ifdef RLIMIT_SWAP
    resInfo(RLIMIT_SWAP);
#endif
#ifdef RLIMIT_VMEM
    resInfo(RLIMIT_VMEM);
#endif

    exit(0);
}
