#include <stdbool.h>

#ifndef PSH_H
#define PSH_H

pid_t shell_terminal;

/* Executa um comando
* INPUTs: Comando; Caminho do arquivo para redirecionamento;
  Booleano para informar se o comando deve ser executado em foreground
* OUTPUTs: Inteiro que informa que deu erro.
* Pré-condição: Comando existe.
* Pós-condição: Comando executado.
*/
int executeCommand(char ** command, char* destPath, bool foreground);

/* Executa um comando em background
* INPUTs: Comando; Caminho do arquivo para redirecionamento;
* OUTPUTs: Inteiro que informa que deu erro.
* Pré-condição: Comando existe.
* Pós-condição: Comando executado.
*/
int executeCommandBackground(char ** command, char * destPath);

/* Executa um comando em foreground
* INPUTs: Comando; Caminho do arquivo para redirecionamento;
* OUTPUTs: Nenhum.
* Pré-condição: Comando existe.
* Pós-condição: Comando executado.
*/
void executeCommandForeground(char ** command, char* destPath);

/* Exibe o prompt de comando
* INPUTs: Nenhum.
* OUTPUTs: Nenhum.
* Pré-condição: Nenhuma.
* Pós-condição:Prompt exibido na saida padrão.
*/
void showPrompt();

/* Loop principal da psh 
* INPUTs: Nenhum.
* OUTPUTs: Nenhum.
* Pré-condição: Nenhuma.
* Pós-condição: psh é executada.
*/
void pshLoop();

/* Exibe uma imagem quando os processo são contaminados
* INPUTs: Nenhum.
* OUTPUTs: Nenhum.
* Pré-condição: Nenhum.
* Pós-condição: Imagem exibida na saída padrao.
*/
void renderKillImage();

#endif