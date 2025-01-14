#include <stdio.h>

int main(){
    int a = 1, b = 2, temp;
    printf("a is %d, b is %d.\n",a, b);
    temp = a;
    a = b;
    b = temp;
    printf("a is now %d, b is now %d.\n",a, b);
    
    return 0;
}