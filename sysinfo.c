#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>    /* for utsname */
#include <unistd.h>         /* for gethostname */
#include <string.h>         /* for strcmp */

#define HOST_NAME_MAX 64

void printUnameInfo(void)
{
    struct utsname *sysinfo;

    sysinfo = (struct utsname *) malloc(sizeof(struct utsname));

    if (uname(sysinfo) == 0) {
        printf("%-20s: %s\n", "Operating system", sysinfo->sysname);
        printf("%-20s: %s\n", "Node name", sysinfo->nodename);
        printf("%-20s: %s\n", "Release of OS", sysinfo->release);
        printf("%-20s: %s\n", "Version of release", sysinfo->version);
        printf("%-20s: %s\n", "Hardware type", sysinfo->machine);
    } else {
        perror("Failed");
        exit(1);
    }
}

void printHostname(void)
{
    char hostname[HOST_NAME_MAX];

    if (gethostname(hostname, HOST_NAME_MAX) == 0) {
        printf("%-20s: %s\n", "Hostname", hostname);
    } else {
        perror("Failed");
        exit(1);
    }
}

int main(void)
{
    printUnameInfo();
    printHostname();

    exit(0);
}
