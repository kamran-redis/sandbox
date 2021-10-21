#include <stdio.h>

#define N 10

void quicksort(int a[], int low, int high);
int split(int a[], int low, int high);

int main(void)
{
    int i;
    int a[N] = {5, 7, 8, 9, 3, 4, 21, 1, 8, 17};

    quicksort(a, 0, N - 1);

    for (i = 0; i < N; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}

void quicksort(int a[], int low, int high)
{
    int middle;
    if (low >= high)
        return;
    middle = split(a, low, high);
    quicksort(a, low, middle - 1);
    quicksort(a, middle + 1, high);
}

int split(int a[], int low, int high)
{
    int part_element = a[low];
    for (;;)
    {
        while (low < high && part_element <= a[high])
            high--;
        if (low >= high)
            break;
        a[low++] = a[high];

        while (low < high && a[low] <= part_element)
            low++;
        if (low >= high)
        {
            break;
        }
        a[high--] = a[low];
    }
    a[high] = part_element;
    return high;
}