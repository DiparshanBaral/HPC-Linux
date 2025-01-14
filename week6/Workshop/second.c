#include <stdio.h>

int main(){
    int i, c;
    printf("Prime numbers between 1 and 100 are: \n");
    #pragma omp parallel for
    for(i = 1; i <= 100; i++){
        for(c = 2; c <= i-1; c++){
            if(i%c == 0){
                break;
            }
        }
        if(c == i){
            printf("%d\n", i);
        }
    }

    return 0;
}