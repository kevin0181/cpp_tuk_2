#include <stdio.h>
#include <unistd.h>

void printStars(int n)
{
    for (int i = 1; i < n + 1; ++i)
    {
        if (n == 2)
            sleep(1);
        printf("%c", '*');
    }
}

int main()
{
    int getLine = 0;

    printf("출력할 줄의 수 : ");
    scanf("%d", &getLine);

    for (int i = 1; i < getLine + 1; ++i)
    {
        printStars(i);
        printf("\n");
    }
}