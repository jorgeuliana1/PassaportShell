#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_SIZE 1024
#define MAX_COMMANDS 5

unsigned getCommands(char** commands);
char *trimString(char *str);
bool isNullCommand(char* commandLine);

int main(int argc, char const *argv[]){
    
    // Shell's main loop
    while (true) {
        printf("psh> ");
        
        char* commands[MAX_COMMANDS];
        size_t numOfCommands = getCommands(commands);

        //DEBUG: print commands
        printf("COMMANDS:%ld\n", numOfCommands);
        for (int i = 0; i < numOfCommands; i++) {
            printf(">> %s\n", commands[i]);
        }

        for (int i = 0; i < numOfCommands; i++) {
            if(!strcmp(commands[i],"term"))
                return 0;
        }

        // free commands
        for (int i = 0; i < numOfCommands; i++) {
            free(commands[i]);
        }
    }

    return 0;
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
    
    char* token = strtok(commandLine, ";");
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

        mallocTokens[i] = (char*)malloc((strlen(token) + 1) * sizeof(char));
        strcpy(mallocTokens[i], token);
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

