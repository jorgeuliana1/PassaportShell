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

#define STDOUT 1

bool vaccinatedProcessesExist = false;

void pshLoop()
{
    // printf("prompt pid = %d\n", getpid());
    showPrompt();
    char *commands[MAX_COMMANDS];
    size_t numOfCommands = getCommands(commands);
    int defOut;

    for (int i = 0; i < numOfCommands; i++) {
        if (!strcmp(commands[i], "term")){
            exit(0);
        }
        else if (!strcmp(commands[i], "fg")){
            pid_t vaccinatedGpid;
            vaccinatedGpid = getgid();

            if (vaccinatedProcessesExist) {
                tcsetpgrp(0, vaccinatedGpid);
                sleep(30);
                tcsetpgrp(0, vaccinatedGpid);
            }
        }
        else
        { // Executing the commands
            int proccesses;
            char **command;

            char *destinyPath = NULL;
            command = splitCommand(commands[i], &destinyPath);

            if (numOfCommands == 1)
                executeCommandForeground(command, destinyPath);
            else {
                vaccinatedProcessesExist = true;
                proccesses = executeCommandBackground(command, NULL);
            }

            for (int j = 0; j < ARGV_LEN && command[j] != NULL; j++) {
                free(command[j]);
            }
            free(command);
        }
      
    }

    /*for(int i = 0; i < MAX_COMMANDS && commands[i] != NULL; i++) {
      free(commands[i]);
    }*/

    pshLoop();
}

int executeCommandBackground(char **command, char *destPath)
{
    pid_t f_a;

    f_a = fork();
    if (!f_a)
    {
        setSignalsVacinated();
        
        int stdout_bkp, fd;
        stdout_bkp = dup(STDOUT_FILENO);
        fd = open("/dev/null", O_WRONLY);
        
        dup2(fd, STDOUT_FILENO);
        close(fd);

        executeCommand(command, NULL, false);
        wait(NULL);

        dup2(stdout_bkp, STDOUT_FILENO);
        close(stdout_bkp); 

        exit(0);
    }
    return -1;
}

void executeCommandForeground(char **command, char *destPath)
{
    executeCommand(command, destPath, true);
    wait(NULL);
}

int executeCommand(char **command, char *destPath, bool foreground)
{
    pid_t pid;

    if ((pid = fork()) == 0) 
    { // The command will run in foreground at the child process (background of the parent process)
        if (foreground) {
            pid_t fork_pid;
            fork_pid = getpid();
            setpgid(fork_pid, fork_pid);
        }

        if (destPath)
        {
            int fd = open(destPath, O_RDWR, S_IRUSR | S_IWUSR);
            close(STDOUT);
            dup2(fd, STDOUT);
        }

        // This must make the program run
        if(execvp(command[0], command) == -1) 
        {
            perror("execvp"); 
        }
        exit(0);
    }

    return -1;
}

void showPrompt()
{
    fwrite("\033[1;33m", 1, 8, stdout);
    fwrite("psh> ", 1, 5, stdout);
    fwrite("\033[0m", 1, 5, stdout);
    fflush(stdout);
}

void renderKillImage()
{
    printf(
        "                       ,---.\n"
        "                       /    |\n"
        "                      /     |\n"
        "                     /      |\n"
        "                    /       |\n"
        "               ___,'        |\n"
        "             <  -'          :\n"
        "              `-.__..--'``-,_\\_\n"
        "                 |o/ ` :,.)_`>\n"
        "                 :/ `     ||/)\n"
        "                 (_.).__,-` |\\ \n"
        "                 /( `.``   `| :\n"
        "                 \'`-.)  `  ; ;\n"
        "                 | `       /-<\n"
        "                 |     `  /   `.\n"
        " ,-_-..____     /|  `    :__..-'\\ \n"
        "/,'-.__\\  ``-./ :`      ;       \\ \n"
        "`\\ `\\  `\\  \\ :  (   `  /  ,   `. \\ \n"
        "  \\` \\   \\   |  | `   :  :     .\\ \\ \n"
        "   \\ `\\_  ))  :  ;     |  |      ): :\n"
        "  (`-.-'\\ ||  |\\ \\   ` ;  ;       | |\n"
        "   \\-_   `;;._   ( `  /  /_       | |\n"
        "    `-.-.// ,'`-._\\__/_,'         ; |\n"
        "       \\:: :     /     `     ,   /  |\n"
        "        || |    (        ,' /   /   |\n"
        "        ||                ,'   /    |\n"
        "________ Unfortunately all process died!________\n"
        "___ Vaccination should be a social contract!____\n"
        "____Cooperation was the morally right choice!___\n"
    );
}