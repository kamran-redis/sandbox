#include <stdio.h>
#include <stdlib.h>
#include "StackADTLL.h"

struct node {
    Item data;
    struct node *next;
};

struct stack_type {
    struct node *top;
};


static void terminate(const char *message)
{
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}

Stack create() {
    Stack s = malloc(sizeof(struct stack_type));
    if (s == NULL) {
        terminate("could not create stack");
    }

    s->top = NULL;
    return s;
}

void destroy(Stack s) {
    make_empty(s);
    free(s);
}
void make_empty(Stack s) {
    while(!is_empty(s)) {
        pop(s);
    }
}

bool is_empty(Stack s) {
    return s->top == NULL;
}

void push(Stack s, Item i) {
    struct node *new_node = malloc(sizeof(struct node));
    if (new_node == NULL) {
        terminate("could not create node");
    }

    new_node->data = i;
    new_node->next = s->top;
    s->top = new_node; 

}
Item pop(Stack s) {
    if (is_empty(s)) {
        terminate("Stack is empty");
    }
    Item i;
    i = s->top->data;
    struct node *temp = s->top;
    s->top = s->top->next;
    free(temp);

    return i;
}