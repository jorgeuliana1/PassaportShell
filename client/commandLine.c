#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "commandLine.h"

char** splitCommand(char* command, char** destPath) {
    int i;
    char** argv = (char**)malloc(sizeof(char*) * ARGV_LEN);
    char* token = trimString(strtok(command, " "));
    if (!token) return 0;
    char* args[ARGV_LEN];
    
    for(i = 0; token != NULL && i < ARGV_LEN; i++) {
    	// Verifying if string is only a space
    	if (!strlen(trimString(token))) {
    	    i--;
    	    continue;
    	}
    	else if (!strcmp(token, ">")) {
    	    i--;
            char* tempString = strtok(NULL, " ");
            trimString(tempString);
            *destPath = tempString;
     	}
    	else {
    		args[i] = (char*)malloc((strlen(trimString(token)) + 1) * sizeof(char));
    		strcpy(args[i], trimString(token));
    		argv[i] = args[i];
    	}
        // Going to the next token
        token = strtok(NULL, " ");
    }

    for (int j = i; j < ARGV_LEN; j++) {
        argv[j] = NULL;
    }

    return argv;
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