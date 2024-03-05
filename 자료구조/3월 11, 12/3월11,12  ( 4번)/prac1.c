#include <stdio.h>
#include <unistd.h>

void printAstar()
{
    printf("%c", '*');
}

int main()
{
    int getLine = 0;

    printf("출력할 줄의 수 : ");
    scanf("%d", &getLine);

    for (int i = 1; i < getLine + 1; ++i)
    {
        for (int j = 1; j < i + 1; ++j)
        {
            if (i == 2)
                sleep(1);
            printAstar();
        }
        printf("\n");
    }
}