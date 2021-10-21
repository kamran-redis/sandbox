#include <stdio.h>
#include <string.h>
#define NUM_PLANETS 9
#define PRINT_INT(n) printf (#n " = %d\n", n); 
#define FUNCTION_CALLED() printf("%s called\n", __func__)
#define FUNCTION_RETURNED() printf("%s returned\n", __func__)


int main(int argc, char **argv)
{
    FUNCTION_CALLED();
    char *planets[] = {"Mercury",
                       "Venus",
                       "E",
                       "Mars",
                       "J",
                       "S",
                       "U",
                       "N",
                       "P"};

    int i, j;
    for (i = 1; i < argc; i++)
    {
        for (j = 0; j < NUM_PLANETS; j++)
        {
            PRINT_INT(i);
            PRINT_INT(j);

            if (strcmp(argv[i], planets[j]) == 0)
            {
                printf("Match %s \n", argv[i]);
                break;
            }
        }
        if (j == NUM_PLANETS)
        {
            printf("NO  %s \n", argv[i]);
        }
    }
    FUNCTION_RETURNED();
}