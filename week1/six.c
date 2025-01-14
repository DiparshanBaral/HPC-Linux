#include <stdio.h>

void swap(int *A, int *B){
    int temp;
    temp = *A;
    *A = *B;
    *B = temp;
}

int main(){
    int a = 1; int b = 2;
    printf("a is %d, b is %d.\n",a, b);

    swap(&a, &b);

    printf("a is now %d, b is now %d.\n",a, b);

    return 0;
}