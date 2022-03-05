#ifndef PSH_H
#define PSH_H

pid_t shell_terminal;

int executeCommand(char ** command, char* destPath);
int executeCommandBackground(char ** command, char * destPath);
int executeCommandForeground(char ** command, char* destPath);
void showPrompt();
void pshLoop();
void renderKillImage();

#endif