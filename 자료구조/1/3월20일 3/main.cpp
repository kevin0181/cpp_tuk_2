#include <iostream>

using namespace std;

#define COL 19
#define LOW 19

struct point
{
    int *x = nullptr;
    int *y = nullptr;
    char shape{'+'};
};

bool print(int x, int y, char shape[2], int shape_status, point p[COL][LOW])
{

    int size_o{};
    int size_x{};

    if (size_o + size_x == COL * LOW)
    {
        cout << "끝났습니다!" << endl;
        exit(0);
    }

    if (x > 18 || y > 18)
    {
        cout << "18X18 범위 내로 입력해주세요." << endl;
        return false;
    }

    if (p[x][y].x != nullptr && p[x][y].y != nullptr)
    {
        cout << "이미 돌이 놓여져 있습니다." << endl;
        return false;
    }
    else
    {
        for (size_t i = 0; i < COL; ++i)
        {
            for (size_t j = 0; j < LOW; ++j)
            {
                if (x == i && y == j)
                {
                    p[x][y].x = &x;
                    p[x][y].y = &y;
                    p[x][y].shape = shape[shape_status];
                }

                cout << p[i][j].shape << " ";

                if (p[i][j].shape == 'o')
                    size_o++;
                else if (p[i][j].shape == 'x')
                    size_x++;
            }
            cout << endl;
        }
        cout << "o 의 개수 : " << size_o << "개, x 의 개수 : " << size_x << "개" << endl;
        return true;
    }
}

int main()
{
    int x;
    int y;
    point p[COL][LOW];
    char shape[2] = {
        'o',
        'x'};
    int shape_status{0};

    while (true)
    {
        cout << "좌표 두곳을 입력하세요 ex) x y : ";
        cin >> x >> y;

        if (print(x, y, shape, shape_status, p))
        {
            if (shape_status)
                shape_status = 0;
            else
                shape_status = 1;
        }
    }
}