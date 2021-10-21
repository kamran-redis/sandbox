#include <stdio.h>

#define N 10

void max_min(const int a[], int n, int *max, int *min);
void reverse(const int *a, int n);
int main(void)
{
    int b[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    int big, small, *p;
    p = b;

    printf("1: size b=%lu p=%lu *p=%lu\n", sizeof(b), sizeof(p), sizeof(*p));
    for (int i = 0; i < N; i++, p++)
    {
        printf("%d ", *p);
    }
    printf("\n");

    reverse(b, N);

    max_min(b, N, &big, &small);
    printf("%d %d \n", big, small);
    return 0;
}


void reverse(const int *a, int n)
{   const int *p= a + (n - 1);
    printf("2: size a=%lu p=%lu \n", sizeof(a), sizeof(p));
    for (; p >= a; --p)
    {
        printf("%d ", *p);
    }
    printf("\n");
}

void max_min(const int a[], int n, int *max, int *min)
{

    *max = *min = a[0];
    for (int i = 1; i < n; i++)
    {
        if (a[i] > *max)
            *max = a[i];
        else if (a[i] < *min)
            *min = a[i];
    }
}
