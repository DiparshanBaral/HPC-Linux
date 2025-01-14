#include <stdio.h>
#include <stdlib.h>

int main()
{
    int size;
    printf("Enter a number between 1 and 50: ");
    scanf("%d", &size);

    int *numbers = malloc(size * sizeof(int));
    if(numbers == NULL){
        printf("Memory allocation failed.\n");
        return 1;
    }

    for(int i = 1; i <= size; i++){
        numbers[i] = rand();
        printf("Element %d is: %d \n",i, numbers[i]);
    }

    free(numbers);
    return 0;
}
