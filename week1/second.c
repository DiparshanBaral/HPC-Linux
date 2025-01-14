#include <stdio.h>
#include <stdlib.h>

int main(){
    int age;
    char name[10];
    printf("Enter your name: ");
    scanf("%s", name);
    printf("Enter your age: ");
    scanf("%d", &age);
    printf("You are %s and you are %d years old.\n", name, age);
    return 0;
}