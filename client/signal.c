#include <stdio.h>
#include <sys/prctl.h>
#include <sys/signal.h>
#include <unistd.h>
#include "../headers/psh.h"
#include "../headers/signal.h"

void signalHandler(int signal) {
    setSignalsIgnore();
    printf("Estou vacinada... desista!!\n" );
    setSignalsVacinated();
}

void setSignalsVacinated() {
    signal(SIGINT, signalHandler);
    signal(SIGTSTP, signalHandler);
    signal(SIGQUIT, signalHandler);
}

void setSignalsIgnore() {
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

