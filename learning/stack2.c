#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

struct node {
    int data;
    struct node *next;
};

static struct node *top = NULL;

static void terminate(const char *message)
{
    printf("%s\n", message);
    exit(EXIT_FAILURE);
}

void make_empty(void) {
    while (!is_empty()) {
        pop();
    }
}

bool is_empty(void) {
    return NULL == top;
}

bool is_full(void) {
    return false;
}

void push(int i) {
    struct node *new_node = malloc(sizeof(struct node));
    if (NULL == new_node) {
        terminate("Stack is full");
    }
    new_node->data = i;
    new_node->next = top;
    top = new_node;  
}

int pop() {
    int i;
    if (is_empty()) {
        terminate("Stack is empty");
    }
    struct node *old_top = top;
    i = top->data;
    top = top->next;
    free(old_top);
    return i;
}

