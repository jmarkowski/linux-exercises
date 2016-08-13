#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>    /* for utsname */
#include <unistd.h>         /* for gethostname */
#include <string.h>         /* for strcmp */

#define HOST_NAME_MAX 64

int main(void)
{
    struct utsname *sysinfo;
    char hostname[HOST_NAME_MAX];

    sysinfo = (struct utsname *) malloc(sizeof(struct utsname));

    if (uname(sysinfo) == 0) {
        printf("Operating System:   %s\n", sysinfo->sysname);
        printf("Node name:          %s\n", sysinfo->nodename);
        printf("Release of OS:      %s\n", sysinfo->release);
        printf("Version of release: %s\n", sysinfo->version);
        printf("Hardware type:      %s\n", sysinfo->machine);
    } else {
        perror("Failed");
        exit(1);
    }

    if (gethostname(hostname, HOST_NAME_MAX) == 0) {
        if (strcmp(hostname, sysinfo->nodename) != 0) {
            printf("Hostname:           %s\n", hostname);
        }
    } else {
        perror("Failed");
        exit(1);
    }

    return 0;
}
