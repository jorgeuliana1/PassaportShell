#include <unistd.h>
#include <stdbool.h>

#ifndef PROCESS_STACK_H
#define PROCESS_STACK_H

typedef pid_t Process;

struct node {
    struct node *next;
    Process *content;
};

typedef struct stack {
    struct node *top;
} Stack;

/* Inicializa uma pilha vazia.
* INPUTs: Nada.
* OUTPUTs: O ponteiro para a pilha.
* Pré-condição: Nenhuma.
* Pós-condição: Uma pilha vazia é criada.
*/
Stack* stackInit();

/* Adiciona um elemento na pilha.
* INPUTs: Ponteiro para a pilha, ponteiro para o pid do processo.
* OUTPUTs: Nada.
* Pré-condição: Uma pilha existe.
* Pós-condição: Um elemento é adicionado à pilha.
*/
void stackPush(Stack* s, Process* p);

/* Remove o elemento do topo da pilha (ultimo elemento a ser adicionado).
* INPUTs: Ponteiro para a pilha.
* OUTPUTs: Ponteiro para o elemento que acabou de ser removido da pilha, se não existem elementos, retorna NULL.
* Pré-condição: Uma pilha existe.
* Pós-condição: Um elemento é retirado da pilha, caso exista ao menos um elemento.
*/
Process *stackPop(Stack* s);

/* A pilha é destruída.
* INPUTs: Ponteiro para a pilha.
* OUTPUTs: Nada.
* Pré-condição: Uma pilha existe.
* Pós-condição: A pilha não existe mais.
*/
void stackDestroy(Stack* s);

/* Verifica se a pilha está vazia.
* INPUTs: Ponteiro para a pilha.
* OUTPUTs: Um booleana que informa se uma pilha está vazia.
* Pré-condição: Uma pilha existe.
* Pós-condição: Nada.
*/
bool stackEmpty(Stack* s);

#endif /* PROCESS_STACK_H */