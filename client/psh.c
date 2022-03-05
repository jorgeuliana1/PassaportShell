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

void pshLoop()
{
    printf("prompt pid = %d\n", getpid());
    showPrompt();
    char *commands[MAX_COMMANDS];
    size_t numOfCommands = getCommands(commands);
    int defOut;

    for (int i = 0; i < numOfCommands; i++)
    {
        if (!strcmp(commands[i], "term"))
        {
            printf("|| term command ||\n");
            exit(0);
        }
        if (!strcmp(commands[i], "fg"))
        {
            wait(NULL);
            // TODO
        }
        else
        { // Executing the commands
            int proccesses;
            char **command;

            char *destinyPath = NULL;
            command = splitCommand(commands[i], &destinyPath);

            if (numOfCommands == 1)
                executeCommandForeground(command, destinyPath);
            else
                proccesses = executeCommandBackground(command, NULL);
        }
    }
    pshLoop();
}

int executeCommandBackground(char **command, char *destPath)
{
    pid_t f_a;

    f_a = fork();
    if (!f_a)
    {
        int stdout_bkp, fd;
        stdout_bkp = dup(STDOUT_FILENO);
        fd = open("/dev/null", O_WRONLY);

        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);

        executeCommand(command, NULL);

        dup2(stdout_bkp, STDOUT_FILENO);
        dup2(stdout_bkp, STDERR_FILENO);
        close(stdout_bkp);

        exit(0);
    }
}

int executeCommandForeground(char **command, char *destPath)
{
    executeCommand(command, destPath);
    wait(NULL);
}

int executeCommand(char **command, char *destPath)
{
    pid_t pid;

    if ((pid = fork()) == 0) 
    { // The command will run in foreground at the child process (background of the parent process)W
        // Changing the default output
        int ret, process;
        ret = tcsetpgrp(shell_terminal, getpgid(getpid()));
        if (destPath)
        {
            int fd = open(destPath, O_RDWR, S_IRUSR | S_IWUSR);
            close(STDOUT);
            dup2(fd, STDOUT);
        }
        process = execvp(command[0], command); // This must make the program run
        exit(1);
    }

    return -1;
}

void showPrompt()
{
    printf("\033[1;33m");
    printf("psh> ");
    printf("\033[0m");
}

void renderKillImage()
{
    printf("                           ,---.\n");
    printf("                       /    |\n");
    printf("                      /     |\n");
    printf("                     /      |\n");
    printf("                    /       |\n");
    printf("               ___,'        |\n");
    printf("             <  -'          :\n");
    printf("              `-.__..--'``-,_\\_\n");
    printf("                 |o/ ` :,.)_`>\n");
    printf("                 :/ `     ||/)\n");
    printf("                 (_.).__,-` |\\n");
    printf("                 /( `.``   `| :\n");
    printf("                 \'`-.)  `  ; ;\n");
    printf("                 | `       /-<\n");
    printf("                 |     `  /   `.\n");
    printf(" ,-_-..____     /|  `    :__..-'\\n");
    printf("/,'-.__\\  ``-./ :`      ;       \\n");
    printf("`\\ `\\  `\\  \\ :  (   `  /  ,   `. \\n");
    printf("  \\` \\   \\   |  | `   :  :     .\\ \\n");
    printf("   \\ `\\_  ))  :  ;     |  |      ): :\n");
    printf("  (`-.-'\\ ||  |\\ \\   ` ;  ;       | |\n");
    printf("   \\-_   `;;._   ( `  /  /_       | |\n");
    printf("    `-.-.// ,'`-._\\__/_,'         ; |\n");
    printf("       \\:: :     /     `     ,   /  |\n");
    printf("        || |    (        ,' /   /   |\n");
    printf("        ||                ,'   /    |\n");
    printf("________ Unfortunately all process died!________\n");
    printf("___ Vaccination should be a social contract!____\n");
    printf("____Cooperation was the morally right choice!___\n");
}