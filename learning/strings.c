#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[argc + 1])
{
    size_t const len = strlen(argv[0]);
    char name[len + 1];

    memcpy(name, argv[0], len);
    name[len] =0;
    if (!strcmp(name, argv[0]))
    {
        printf("Program name \"%s\" copied\n", name);
    }
    else
    {
        printf("Program %s name copied differently as %s \n", argv[0], name);
    }

    return 0;
}