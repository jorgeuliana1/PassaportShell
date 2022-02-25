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

int executeCommand(char ** command, FILE* de);
int executeCommandForeground(char ** command, FILE* dest);

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

        //DEBUG: print commands
        printf("COMMANDS:%ld\n", numOfCommands);
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
                
                FILE* destiny = NULL;
                command = splitCommand(commands[i], destiny);

                // Allocating the argv
                if(numOfCommands == 1) executeCommandForeground(command, destiny);
                
                else proccesses = executeCommand(command, NULL);
                
                if(destiny) fclose(destiny);
            }

        }

        // free commands
        for (int i = 0; i < numOfCommands; i++) {
            free(commands[i]);
        }
    }

    return 0;
}

int executeCommandForeground(char ** command, FILE* dest) {
    executeCommand(command, dest);
    wait(NULL);
}

int executeCommand(char** command, FILE* dest) {
    pid_t pidWriter, pidReader;

    if ((pidWriter = fork()) == 0) { // The command will run in foreground at the child process (background of the parent process)W
        // Changing the default output

        int ret = tcsetpgrp(shell_terminal, getpgid(getpid()));
        printf("ret = %d\n", ret);
        printf("pgid = %u\n", tcgetpgrp(getpid()));
        // int commandReturn = executeCommand(command, stream);
        int process;

        process = execvp(command[0], command); // This must make the program run      
    }

    return -1;
}

void showPrompt() {
    printf("\033[1;33m");
    printf("psh> ");
    printf("\033[0m");
}