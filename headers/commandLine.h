#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <stdio.h>
#include <stdbool.h>

#define MAX_LINE_SIZE 1024
#define MAX_COMMANDS 5
#define ARGV_LEN 3

/* Divide um comando e analisa os argumentos
* INPUTs: Comando; Caminho do arquivo para redirecionamento
* OUTPUTs: Matriz de comandos.
* Pré-condição: Vetor com a linhda de comandos;
* Pós-condição: Matriz de argumentos existe;
*/
char** splitCommand(char* command, char** destPath);

/* Lê e divide uma linha de comandos no terminal em vários comandos
* INPUTs: Matriz  de comandos.
* OUTPUTs: Quantidade de comandos recebidos.
* Pré-condição: Nenhuma.
* Pós-condição: Matriz de comandos existe; Quantidade de comandos existe.
*/
unsigned getCommands(char** commands);

/* Verifica se um comando é nulo
* INPUTs: Comando.
* OUTPUTs: true se o comando for nulo, false se não for.
* Pré-condição: Comando existe;
* Pós-condição: Valor booleano existe.
*/
bool isNullCommand(char* commandLine);

/* Retira espaços de uma string
* INPUTs: String.
* OUTPUTs: String sem espaços.
* Pré-condição: String existe;
* Pós-condição: String sem espaços existe.
*/
char* trimString(char *str);

#endif