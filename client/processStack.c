#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../headers/processStack.h"

Stack* stackInit() {
    Stack* s;
    s = (Stack*)malloc(sizeof(Stack));
    s->top = NULL;
    return s;
}

void stackPush(Stack *s, Process p)
{
  struct node *n;
  n = (struct node *)malloc(sizeof(struct node));
  n->content = p;
  n->next = s->top;
  s->top = n;
}

Process stackPop(Stack *s)
{
  if (stackEmpty(s))
  {
    return -1;
  }

  struct node *n;
  n = s->top;
  s->top = n->next;

  Process p;
  p = n->content;
  free(n);

  return p;
}

void stackDestroy(Stack* s) {
    while (!(stackEmpty(s))) {
        stackPop(s);
    }
    free(s);
}

bool stackEmpty(Stack* s) {
    return s->top == NULL;
}