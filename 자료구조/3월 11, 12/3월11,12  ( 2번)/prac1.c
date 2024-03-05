#include <stdio.h>
#include <unistd.h>

int main()
{
    int getLine = 0;
    
    printf("출력할 줄의 수 : ");
    scanf("%d", &getLine);

    for (int i = 0; i < getLine; ++i)
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