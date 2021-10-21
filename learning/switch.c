#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int main(int argc, char *argv[argc + 1])
{   size_t m = 0 - 1;
    printf("%zu \n", m);
    for (int i = 1; i < argc; ++i)
    {
        int v = atoi(argv[i]);
        printf("%d \n", v);

        switch (v)
        {
        default:
            puts("++++ ..... +++");
        case 4:
            puts("++++");
        case 3:
            puts("+++");
        case 2:
            puts("++");
        case 1:
            puts("+");
        case 0:;
        }
    }
    return EXIT_SUCCESS;
}