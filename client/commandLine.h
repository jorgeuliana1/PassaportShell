#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <stdio.h>
#include <stdbool.h>

//Split commands into arguments 
//Define a destiny file if the command has a > operator
#define MAX_LINE_SIZE 1024
#define MAX_COMMANDS 5
#define ARGV_LEN 3

char** splitCommand(char* command, FILE* dest);

unsigned getCommands(char** commands);

bool isNullCommand(char* commandLine);

char* trimString(char *str);

#endif