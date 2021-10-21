#include <stdio.h>
#include "StackADTLL.h"

int main(void) {
    Stack s1, s2;
    s1 = create();
    s2 = create();

    push(s1, 1);
    push(s1, 2);

    int n = pop(s1);
    printf("s1 pop %d\n", n);
    push(s2, n);

    destroy(s1);
    while (!is_empty(s2)) {
        printf("s2 pop %d\n", pop(s2));
    }

    push(s2, 3);
    make_empty(s2);

    if (is_empty(s2)) {
        printf("s2 is empty\n");
    } else {
        printf("s2 is not empty\n");
    }
    destroy(s2);
    return 0;

}