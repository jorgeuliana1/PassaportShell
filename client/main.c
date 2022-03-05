#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "commandLine.h"

#define STDOUT 1

int executeCommand(char ** command, char* destPath);
int executeCommandForeground(char ** command, char* destPath);

void showPrompt();
pid_t shell_terminal;

int main(int argc, char const *argv[]){
    
    // signal(SIGINT, signalHandler);
    // signal(SIGQUIT, signalHandler);
    // signal(SIGTSTP, signalHandler);
    
    // Shell's main loop
    shell_terminal = tcgetpgrp(STDIN_FILENO);
    while (true) {
        showPrompt();
        char* commands[MAX_COMMANDS];
        size_t numOfCommands = getCommands(commands);
        int defOut;

        for (int i = 0; i < numOfCommands; i++) {
            if(!strcmp(commands[i],"term")) {
              printf("|| term command ||\n");
              return 0;
            }
            if(!strcmp(commands[i],"fg")) {
                wait(NULL);
                // TODO
            }
            else { // Executing the commands
                int proccesses;
                char** command;
                
                char* destinyPath;
                command = splitCommand(commands[i], &destinyPath);

                // Allocating the argv
                if(numOfCommands == 1) executeCommandForeground(command, destinyPath);
                
                else proccesses = executeCommand(command, NULL);
            }
        }

        // free commands
        for (int i = 0; i < numOfCommands; i++) {
            free(commands[i]);
        }
    }

    return 0;
}

int executeCommandForeground(char ** command, char* destPath) {
    executeCommand(command, destPath);
    wait(NULL);
}

int executeCommand(char** command, char* destPath) {
    pid_t pidWriter, pidReader;

    if ((pidWriter = fork()) == 0) { // The command will run in foreground at the child process (background of the parent process)W
        // Changing the default output
        int ret, process;
        ret = tcsetpgrp(shell_terminal, getpgid(getpid()));
        if (strlen(destPath)) {
            int fd = open(destPath, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
            close(STDOUT);
            dup2(fd, STDOUT);
        }
        process = execvp(command[0], command); // This must make the program run      
    }
    
    return -1;
}

void showPrompt() {
    printf("\033[1;33m");
    printf("psh> ");
    printf("\033[0m");
}