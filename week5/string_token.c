#include <stdio.h>
#include <string.h>

int main()
{
    FILE *fp = fopen("myfile.txt", "r");
    char line[100];
    int linecount = 1;

    while (fgets(line, 100, fp) != NULL)
    {
        printf("Line %d contains: %s\n", linecount, line);
        linecount++;
    }
    return 0;
}