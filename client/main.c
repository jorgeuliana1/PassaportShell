#include <unistd.h>

#include "../headers/commandLine.h"
#include "../headers/signal.h"
#include "../headers/psh.h"

int main(int argc, char const *argv[]) {
    setSignalsIgnore();
    shell_terminal = tcgetpgrp(STDIN_FILENO);
    pshLoop();

    return 0;
}
