#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../headers/commandLine.h"
#include "../headers/signal.h"
#include "../headers/psh.h"

int main(int argc, char const *argv[]) {
    printf("master pid = %d\n", getpid());

    setSignalsVacinated();
    shell_terminal = tcgetpgrp(STDIN_FILENO);
    pshLoop();

    return 0;
}
