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

Stack* stackInit();

void stackPush(Stack* s, Process* p);

Process *stackPop(Stack* s);

void stackDestroy(Stack* s);

bool stackEmpty(Stack* s);

#endif /* PROCESS_STACK_H */