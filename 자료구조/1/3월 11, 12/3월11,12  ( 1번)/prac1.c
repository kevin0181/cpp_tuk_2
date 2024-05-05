#include <stdio.h>
#include <unistd.h>

int main()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < i + 1; ++j)
        {
            if (i == 1)
                sleep(1);
            printf("*");
        }
        printf("\n");
    }
}