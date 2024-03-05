#include <stdio.h>
#include <unistd.h>

void printRoopStars(int n)
{

    printf("%c", '*');

    if (!(n == 0))
    {
        printRoopStars(--n);
    }
}

int main()
{
    int getLine = 0;

    printf("출력할 줄의 수 : ");
    scanf("%d", &getLine);

    for (int i = 0; i < getLine; ++i)
    {
        if (i == 1)
            sleep(1);
        printRoopStars(i);
        printf("\n");
    }
}