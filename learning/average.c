#include <stdio.h>

double average(double a, double b);
int sum_array(int a[], int n);

int main(void)
{
    printf("avrage %g \n", average(5, 10));
    printf("sum %d \n", sum_array((int[]){5,2,3,4,5}, 5));
}

double average(double a, double b)
{
    return ((a + b) / 2);
}

int sum_array(int a[], int n) {
    int sum =0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}