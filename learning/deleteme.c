#include <stdio.h> 
int main(void) 
{ 
    int arr[] = {10, 20}; 
    int *p = arr; 
    int b = *++p; 
    printf("arr[0] = %d, arr[1] = %d, *p = %d b= %d\n", 
                          arr[0], arr[1], *p, b); 
    printf("arr[0] = %d, 0[arr] = %d\n", 
                          arr[0], 0[arr]); 
    return 0; 
} 