#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_LINE_SIZE 1024
#define MAX_COMMANDS 5
#define ARGV_LEN 3

int executeCommandInForeground(char *command, int stream);
int executeCommandInBackground(char *command, int stream);
unsigned getCommands(char** commands);
char *trimString(char *str);
bool isNullCommand(char* commandLine);
void showPrompt();
unsigned splitCommand(char* command, char** argv);
void signalHandler(int signo);

int main(int argc, char const *argv[]){
    
    // Shell's main loop
    while (true) {
        showPrompt();
        char* commands[MAX_COMMANDS];
        size_t numOfCommands = getCommands(commands);

        //DEBUG: print commands
        printf("COMMANDS:%ld\n", numOfCommands);
        // for (int i = 0; i < numOfCommands; i++) {
        //     printf(">> %s\n", commands[i]);
        // }
        int defOut;

        for (int i = 0; i < numOfCommands; i++) {
            if(!strcmp(commands[i],"term")) {
              printf("|| term command ||\n");
              return 0;
            }
            if(!strcmp(commands[i],"fg"))
                //TODO 
                return 0;
            
            else { // Executing the commands
                /*
                char** cargv;
                cargv = (char**)malloc(sizeof(char*) * ARGV_LEN);
                splitCommand(commands[i], cargv);

                //char* defOutName;
                //defOutName = (char*)malloc(sizeof(char) * (strlen(cargv[0]) + 5));
                //sprintf(defOutName, "%s.txt", cargv[0]);

                //printf("%s\n", defOutName);

                //defOut = open(defOutName, O_WRONLY);
                */
                defOut = 1;

                int proccesses;
                if(numOfCommands == 1) {
                  executeCommandInForeground(commands[i], defOut);
                }

                proccesses = executeCommandInBackground(commands[i], defOut);
            }
            
        }

        // free commands
        for (int i = 0; i < numOfCommands; i++) {
            free(commands[i]);
        }
    }

    return 0;
}

void signalHandler(int signo){
  if
}

//Split commands into arguments 
unsigned splitCommand(char* command, char** argv) {
    int i;
    char* token = trimString(strtok(command, " "));
    if (!token) return 0;
    char* args[ARGV_LEN];
    
    for(i = 0; token != NULL && i < ARGV_LEN; i++) {
      // Verifying if string is only a space
      if (!strlen(trimString(token))) {
          i--;
          continue;
      }
      args[i] = (char*)malloc((strlen(trimString(token)) + 1) * sizeof(char));
      strcpy(args[i], trimString(token));
      argv[i] = args[i];
      
      // Going to the next token
      token = strtok(NULL, " ");
    }

    for (int j = i; j < ARGV_LEN; j++) {
        argv[j] = NULL;
    }

    return (unsigned)i;
}

int executeCommandInForeground(char * command, int stream) {
    //FILE* process;
    int letter, process;

    // Allocating the argv
    char** argv = (char**)malloc(sizeof(char*) * ARGV_LEN);

    // Running the commmand in background with po
    int numOfArgs = splitCommand(command, argv);
    char* argvHead = argv[0];

    process = execvp(argvHead, argv); // This must make the program run

    return 0; // Everything is alright
}

int executeCommandInBackground(char* command, int stream) {
    pid_t pidWriter, pidReader;

    int pipe_p[2];
    if (pipe(pipe_p) < 0)
        exit(1);
    
    if ((pidWriter = fork()) == 0) { // The command will run in foreground at the child process (background of the parent process)W
        // Changing the default output
        close(STDOUT_FILENO);
        pipe_p[0] = open("banana.txt", O_WRONLY);
        close(pipe_p[1]);

        int commandReturn = executeCommandInForeground(command, stream);

        // Closing the output file and returning the commandReturn
        close(pipe_p[0]);
        perror("");
        //return commandReturn;
    }

    close(pipe_p[0]);
    close(pipe_p[1]);

    return -1;
}

unsigned getCommands(char** commands) {
    // Reading the line
    char* commandLine;
    size_t bufferSize = MAX_LINE_SIZE;
    commandLine = (char*)malloc(sizeof(char) * bufferSize);
    getline(&commandLine, &bufferSize, stdin);

    if (isNullCommand(trimString(commandLine))) { // case of null character
        free(commandLine);
        return 0;
    } 
    
    char* token = trimString(strtok(commandLine, ";"));
    if (!token) return 0;
    
    // loop through the string to extract all other tokens
    int i;
    char* mallocTokens[5];
    for(i = 0; token != NULL && i < MAX_COMMANDS; i++) {
        // Verifying if string is only a space
        if (!strlen(trimString(token))) {
            i--;
            continue;
        }

        mallocTokens[i] = (char*)malloc((strlen(trimString(token)) + 1) * sizeof(char));
        strcpy(mallocTokens[i], trimString(token));
        commands[i] = mallocTokens[i];
        
        // Going to the next token
        token = strtok(NULL, ";");
    }

    free(commandLine);

    return (unsigned)i;
}

bool isNullCommand(char* commandLine){
    bool isNullLine, isNullChar;
    isNullChar = (int)commandLine[0] >= 0 && (int)commandLine[0] <= 31;
    return strlen(commandLine) <= 1 && isNullChar;
}

char* trimString(char *str) {
    char* end;
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';

    return str;
}

void showPrompt() {
    printf("\033[1;33m");
    printf("psh> ");
    printf("\033[0m");
}

