#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define FP 32
#define SCALE (5.0f / 9.0f)
typedef int Item;
#define CMP(a,b) ((((a) < (b))) ? (-1) : ((a) > (b)))

//int count_spaces(const char *s);
//size_t mystrlen(const char *s);
//char *mystrcat(char *s1, const char *s2);

//char *concat(const char *s1, const char *s2);

int main(void)
{
    Item a =5;
    Item b =10;
    printf("5,10 %d \n", CMP(a,b));
    printf("10,5 %d \n", CMP(b,a));
    printf("5,5 %d \n", CMP(5,5));
    int d = CMP(a,b);

//    int * restrict p;
//    int * restrict q;

//    p = malloc(sizeof(p));

   //printf("%s", concat("abc", " def"));
    //printf("%s \n" , "\xfc" "ber");
    // char str1[10] = "abcd", str2[10];
    // printf("%lu  %lu \n", mystrlen(str1), mystrlen(str2));
    // strcpy(str2, str1);
    // printf("%lu  %lu \n", mystrlen(str1), mystrlen(str2));
    // mystrcat(str1,str2);
    // printf("%lu  %lu \n", mystrlen(str1), mystrlen(str2));

    //    int i =-10;
    //    unsigned long long j = 10;
    //    printf ("%zu\n", sizeof(i));
    //    printf ("%zu\n", sizeof(j));

    //    int a[10];
    //    int b[10][10] = {false};
    //    printf("Size of a %zu  %zu\n", sizeof(a), sizeof(a[0]));
    //    printf("Size of b %zu  %zu\n", sizeof(b), sizeof(b[0]));

    // int i = 40;
    // float x = 839.21f;

    // printf("|%d|%5d|%-5d|%5.3d|\n", i, i, i, i);
    // printf("|%10.3f|%10.3e|%-10g|\n", x, x, x);
    // int j, k;
    // float l, m;
    // scanf("%d%d%f%f", &j, &k, &l, &m);
    // printf("%d %d %f %f\n", j, k, l, m);

    /*float f;
    printf("Please enter the f value:");
    scanf("%f", &f);
    float c = (f - FP) * SCALE ;
    printf ("C is %.2f\n", c);*/
}

int count_spaces(const char *s)
{
    int count = 0;
    for (; *s != '\0'; s++)
    {
        if (*s == ' ')
            count++;
    }
    return count;
}

size_t mystrlen(const char *s)
{
    const char *p = s;
    while (*s)
        s++;
    return s - p;
}

char *mystrcat(char *s1, const char *s2)
{
    char *p = s1;
    while (*p)
        p++;
    while ((*p++ = *s2++))
        ;
    return s1;
}

char *concat(const char *s1, const char *s2) {
    char *result;
    result = malloc(strlen(s1) + strlen(s2) + 1);
    if (result == NULL) {
        printf("OOM");
    }
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int compare_ints(const void *p, const void *q) {
    return *(int *)p - *(int *)q;
}