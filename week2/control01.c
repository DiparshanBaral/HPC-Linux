#include <stdio.h>
#include <stdlib.h>

int main()
{
    for (int i = 0; i < 5; i++)
    {
        if (i != 4)
        {
            printf("%d,", i);
        }
        else
        {
            printf("%d", i);
        }
    }
    printf("\n");
    return 0;
}