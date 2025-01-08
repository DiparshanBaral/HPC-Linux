#include <stdio.h>
void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

int main()
{
    int a = 3;
    int b = 4;

    printf("Before swap: a is %d and b is %d\n", a, b);
    swap(&a, &b);
    printf("After swap: a is now %d and b is now %d\n", a, b);

    return 0;
}