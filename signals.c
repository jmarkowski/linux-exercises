#include <stdio.h>
#include <stdlib.h> /* for system */
#include <unistd.h> /* for pause */
#include <signal.h> /* for signal */

#define SIG_CASE(SIG) case SIG: printf(" Received " #SIG "\n"); break;

void signalHandler(int signo)
{
    switch (signo) {
    SIG_CASE(SIGHUP)
    SIG_CASE(SIGINT)
    SIG_CASE(SIGQUIT)
    SIG_CASE(SIGILL)
    SIG_CASE(SIGTRAP)
    SIG_CASE(SIGABRT)
    SIG_CASE(SIGBUS)
    SIG_CASE(SIGFPE)
    //SIG_CASE(SIGKILL)
    SIG_CASE(SIGUSR1)
    SIG_CASE(SIGSEGV)
    SIG_CASE(SIGUSR2)
    SIG_CASE(SIGPIPE)
    SIG_CASE(SIGALRM)
    SIG_CASE(SIGTERM)
    SIG_CASE(SIGSTKFLT)
    SIG_CASE(SIGCHLD)
    SIG_CASE(SIGCONT)
    //SIG_CASE(SIGSTOP)
    SIG_CASE(SIGTSTP)
    SIG_CASE(SIGTTIN)
    SIG_CASE(SIGTTOU)
    SIG_CASE(SIGURG)
    SIG_CASE(SIGXCPU)
    SIG_CASE(SIGXFSZ)
    SIG_CASE(SIGVTALRM)
    SIG_CASE(SIGPROF)
    SIG_CASE(SIGWINCH)
    SIG_CASE(SIGIO)
    SIG_CASE(SIGPWR)
    SIG_CASE(SIGSYS)
    case SIGKILL:
    case SIGSTOP:
        /**************************************
         * THESE SIGNALS CAN NEVER BE CAUGHT
         *
         * This should never run.
         **************************************/
        printf("Should never happen\n");
        break;
    }
}

/*
 * Instead of signalHandler, you may install SIG_IGN or SIG_DFL instead.
 * SIG_IGN tells the system to ignore the signal
 * SIG_DFL tells the system to use the default action
 */
#define SIG_INSTALL(SIG) \
    if (signal(SIG, signalHandler) == SIG_ERR) { \
        printf("Cannot catch " #SIG "\n"); \
    } else { \
        printf("Installed signal handler for " #SIG "\n"); \
    }

static void installSignalHandlers(void)
{
    SIG_INSTALL(SIGHUP)
    SIG_INSTALL(SIGINT)
    SIG_INSTALL(SIGQUIT)
    SIG_INSTALL(SIGILL)
    SIG_INSTALL(SIGTRAP)
    SIG_INSTALL(SIGABRT)
    SIG_INSTALL(SIGBUS)
    SIG_INSTALL(SIGFPE)
    SIG_INSTALL(SIGKILL)
    SIG_INSTALL(SIGUSR1)
    SIG_INSTALL(SIGSEGV)
    SIG_INSTALL(SIGUSR2)
    SIG_INSTALL(SIGPIPE)
    SIG_INSTALL(SIGALRM)
    SIG_INSTALL(SIGTERM)
    SIG_INSTALL(SIGSTKFLT)
    SIG_INSTALL(SIGCHLD)
    SIG_INSTALL(SIGCONT)
    SIG_INSTALL(SIGSTOP)
    SIG_INSTALL(SIGTSTP)
    SIG_INSTALL(SIGTTIN)
    SIG_INSTALL(SIGTTOU)
    SIG_INSTALL(SIGURG)
    SIG_INSTALL(SIGXCPU)
    SIG_INSTALL(SIGXFSZ)
    SIG_INSTALL(SIGVTALRM)
    SIG_INSTALL(SIGPROF)
    SIG_INSTALL(SIGWINCH)
    SIG_INSTALL(SIGIO)
    SIG_INSTALL(SIGPWR)
    SIG_INSTALL(SIGSYS)
    printf("Finished signal installs\n");
}

int main(void)
{
    pid_t pid = getpid();

    printf("Available system signals:\n");
    system("/usr/bin/bash -c \"kill -l\"");

    installSignalHandlers();

    printf("\n\nRun as \"./signal.o &\", and send signals with "
           "\"kill -SIG %%1\"\n");
    printf("or...\n");
    printf("Press <Ctrl-\\> (SIGQUIT)\n");
    printf("Press <Ctrl-C> (SIGINT)\n\n");
    printf("Stop process with \"kill -KILL %d\"\n\n", pid);

    for (;;) {
        pause(); /* suspend thread until a signal is received */
    }

    exit(0); /* should never happen */
}
