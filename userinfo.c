#include <stdio.h>
#include <stdlib.h>     /* for exit */
#include <string.h>     /* for strcmp */
#include <pwd.h>        /* for passwd */
#include <shadow.h>     /* for spwd */
#include <grp.h>        /* for group */

static void displayPasswdInfo(struct passwd * pwd)
{
    printf("\n/etc/passwd:\n");
    printf("      Username: %s\n", pwd->pw_name);
    printf("       User ID: %d\n", pwd->pw_uid);
    printf("      Group ID: %d\n", pwd->pw_gid);
    printf("Home directory: %s\n", pwd->pw_dir);
    printf("         Shell: %s\n", pwd->pw_shell);
}

static void displaySpwdInfo(struct spwd * spwd)
{
    printf("\n/etc/shadow:\n");
    printf("                                Username: %s\n",  spwd->sp_namp);
    printf("                      Encrypted password: %s\n",  spwd->sp_pwdp);
    printf("Days since EPOCH of last password change: %ld\n", spwd->sp_lstchg);
    printf("               Days until change allowed: %ld\n", spwd->sp_min);
    printf("             Days before change required: %ld\n", spwd->sp_max);
    printf("             Days warning for expiration: %ld\n", spwd->sp_warn);
    printf("            Days before account inactive: %ld\n", spwd->sp_inact);
    printf("   Days since Epoch when account expires: %ld\n", spwd->sp_expire);
    printf("                                reserved: %lu\n", spwd->sp_flag);
}

static void displayGroupInfo(struct group *grp)
{
    printf("\nGroup %d info:\n", grp->gr_gid);
    printf("              Name: %s\n", grp->gr_name);
    printf("Encrypted password: %s\n", grp->gr_passwd);

    int k = 0;

    while (grp->gr_mem[k] != NULL) {
        printf("          Member %d: %s\n", k, grp->gr_mem[k]);
        k++;
    }
}

static struct passwd * fetchPasswd(const char *name)
{
    struct passwd *ptr;

    setpwent(); /* rewind passwd file */

    while ((ptr = getpwent()) != NULL) {
        if (strcmp(name, ptr->pw_name) == 0) {
            break;
        }
    }

    endpwent(); /* close passwd file */

    return ptr;
}

static struct spwd * fetchSpwd(const char *name)
{
    struct spwd *ptr;

    setspent(); /* rewind shadow file */

    while ((ptr = getspent()) != NULL) {
        if (strcmp(name, ptr->sp_namp) == 0) {
            break;
        }
    }

    endspent(); /* close shadow file */

    return ptr;
}

int main(int argc, char **argv)
{
    char *username;
    struct passwd *passwd;
    struct spwd *spwd;

    if (argc < 2) {
        printf("Usage: %s <username>\n", argv[0]);
        exit(0);
    }

    username = argv[1];

    passwd = fetchPasswd(username);
    spwd   = fetchSpwd(username);

    if (passwd != NULL) {
        struct group *grp;

        displayPasswdInfo(passwd);

        grp = getgrgid(passwd->pw_gid);

        if (grp != NULL) {
            displayGroupInfo(grp);
        } else {
            perror("grp failed");
            exit(1);
        }
    } else {
        perror("passwd failed");
        exit(1);
    }

    /* Note: Must be superuser to run the following */
    if (spwd != NULL) {
        displaySpwdInfo(spwd);
    }

    exit(0);
}
