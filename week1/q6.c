#include <stdio.h>
void main(int argc, char *argv[])
{
    for (int n = 0; n < 100; n++)
    {
        if (n % 2 == 0 && n % 3 == 0 && n % 5 == 0)
        {
            printf("BISH and BASH and BOSH\n");
        }
        else if (n % 2 == 0 && n % 3 == 0)
        {
            printf("BISH and BASH\n");
        }
        else if (n % 2 == 0 && n % 5 == 0)
        {
            printf("BISH and BOSH\n");
        }
        else if (n % 3 == 0 && n % 5 == 0)
        {
            printf("BASH and BOSH\n");
        }
        else if (n % 2 == 0)
        {
            printf("BISH\n");
        }
        else if (n % 3 == 0)
        {
            printf("BASH\n");
        }
        else if (n % 5 == 0)
        {
            printf("BOSH\n");
        }
    }
}