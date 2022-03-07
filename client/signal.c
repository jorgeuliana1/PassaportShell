#include <stdio.h>
#include <sys/prctl.h>
#include <sys/signal.h>
#include <unistd.h>
#include <string.h>
#include "../headers/psh.h"
#include "../headers/signal.h"

void signalHandler(int signal) {
    setSignalsIgnore();
    char str[] = "Estou vacinada... desista!!\n";

    write(STDERR_FILENO, str, strlen(str) + 1);

    setSignalsVacinated();
    showPrompt();
}

void setSignalsVacinated() {
    signal(SIGINT, signalHandler);
    signal(SIGTSTP, signalHandler);
    signal(SIGQUIT, signalHandler);
    signal(SIGUSR1, signalHandler);
}

void setSignalsIgnore() {
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGUSR1, SIG_IGN);
}

