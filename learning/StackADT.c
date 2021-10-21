#include <stdio.h>
#include <stdlib.h>
#include "StackADT.h"


struct stack_type {
    Item *contents;
    int top;
    int size;
};


static void terminate(const char *message)
{
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}

Stack create(size) {
    Stack s = malloc(sizeof(struct stack_type));
    if (s == NULL) {
        terminate("could not create stack");
    }

    s->contents = malloc(sizeof(Item) * s->size);
    if (s->contents == NULL) {
        free(s);
        terminate("2:could not create stack");
    }

    s->top =0;
    s->size = size;
    return s;
}

void destroy(Stack s) {
    free(s->contents);
    free(s);
}
void make_empty(Stack s) {
    s->top =0;
}
bool is_empty(Stack s) {
    return s->top == 0;
}

bool is_full(Stack s) {
    return s->top == s->size;
}
void push(Stack s, Item i) {
    if (is_full(s)) {
        terminate("Stack is full");
    }
    s->contents[s->top++] = i;
}
Item pop(Stack s) {
    if (is_empty(s)) {
        terminate("Stack is empty");
    }
    return s->contents[--s->top];
}