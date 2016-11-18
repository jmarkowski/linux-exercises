#include <stdio.h>
#include <stdlib.h> /* for exit */
#include <signal.h> /* for signal */
#include <unistd.h> /* for alarm */

static void sigAlarm(int signo)
{
    /* Do nothing. Simply returning wakes up sigsuspend() */
}

/*
 * sigaction members:
 *  sa_handler: pointer to a signal-catching function (or SIG_IGN or SIG_DFL)
 *  sa_mask:    additional set of signals to be blocked during execution of
 *              signal-catching function.
 *  sa_flags:   special flags to affect behaviour of signal.
 *  sa_sigaction: pointer to a signal-catching function.
 */
typedef struct sigaction sigaction_t;

unsigned sleep(unsigned seconds)
{
    sigaction_t newAct, oldAct;
    sigset_t    newSet, oldSet, suspSet;
    unsigned int unslept;

    /*
     * Set our handler, save previous information
     */
    newAct.sa_handler = sigAlarm;
    sigemptyset(&newAct.sa_mask);   /* no signals to be blocked by sigAlarm */
    newAct.sa_flags = 0;

    /* Specify the new action to be associated with SIGALRM */
    sigaction(SIGALRM, &newAct, &oldAct);

    /*
     * Block SIGALRM and save current signal mask
     */
    sigemptyset(&newSet);           /* initialize as empty... */
    sigaddset(&newSet, SIGALRM);    /* add SIGALRM */

    /* Block the SIGALRM signal. Blocked signals are ignored from being
     * processed. */
    sigprocmask(SIG_BLOCK, &newSet, &oldSet);

    alarm(seconds);
    suspSet = oldSet;

    /* Make sure SIGALRM isn't blocked */

    /* Remove SIGALRM from the old signal set */
    sigdelset(&suspSet, SIGALRM);

    /* Suspend this process and wait for any signal to be caught */
    sigsuspend(&suspSet);

    /* When sigsuspend returns, the signal mask is restored back to newSet */

    /* Some signal has been caught, SIGALRM is now blocked */

    unslept = alarm(0);

    /*
     * Restoration
     */

    /* Restore the previous SIGALRM action */
    sigaction(SIGALRM, &oldAct, NULL);

    /* Restore the old signal set, which unblocks SIGALRM */
    sigprocmask(SIG_SETMASK, &oldSet, NULL);

    return unslept;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: sleep.o SLEEP_TIME_SEC\n");
        exit(1);
    }

    unsigned seconds = atoi(argv[1]);

    printf("Sleeping for %d seconds...\n", seconds);
    fflush(stdout);

    sleep(seconds);
}
