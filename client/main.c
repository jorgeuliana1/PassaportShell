#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_LINE_SIZE 1024
#define MAX_COMMANDS 5
#define ARGV_LEN 3

int executeCommandInBackground(char *command, FILE* stream);
unsigned getCommands(char** commands);
char *trimString(char *str);
bool isNullCommand(char* commandLine);
void showPrompt();

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

        for (int i = 0; i < numOfCommands; i++) {
            if(!strcmp(commands[i],"term")) {
              printf("|| term command ||\n");
              return 0;
            }
            if(!strcmp(commands[i],"fg"))
                //TODO 
                return 0;
            
            else { // Executing the commands
                int proccess;
                proccess = executeCommandInBackground(commands[i], stdout);
            }
            
        }

        // free commands
        for (int i = 0; i < numOfCommands; i++) {
            free(commands[i]);
        }
    }

    return 0;
}

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

int executeCommandInBackground(char * command, FILE* stream) {
    //FILE* process;
    int letter, process;

    // Allocating the argv
    char** argv = (char**)malloc(sizeof(char*) * ARGV_LEN);

    // Running the commmand in background with po
    int numOfArgs = splitCommand(command, argv);
    char* argvHead = argv[0];
    process = execvp(argvHead, argv); // This must make the program run
    if(process)

    // Running the commmand in background with popen
    // process = popen(command, "w");

    // // If popen could not be open
    // if (popen == NULL) {
    //     return 1;
    // }

    // while (letter = fgetc(process) != EOF)
    //    fprintf(stream, "%c", letter); // Outputting data in the given stream
    
    // pclose(process);

    return 0; // Everything is alright
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

