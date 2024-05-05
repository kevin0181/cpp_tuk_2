#include <iostream>

#define COL 19
#define LOW 19

using namespace std;

struct point
{
    int *x = nullptr;
    int *y = nullptr;
    char shape = '+';
};

bool draw(int x, int y, point map[COL][LOW], int shape_result, char shape[2])
{
    if (map[x][y].x != nullptr || map[x][y].y != nullptr)
    {
        cout << "이미 돌이 놓여 있습니다." << endl;
        return false; // 비정상
    }
    else
    {
        for (int i = 0; i < COL; ++i)
        {
            for (int j = 0; j < LOW; ++j)
            {
                if (x == i && y == j)
                {
                    map[i][j].x = new int(x);
                    map[i][j].y = new int(y);
                    map[i][j].shape = shape[shape_result];
                }
                cout << map[i][j].shape << " ";
            }
            cout << endl;
        }
        return true; // 정상
    }
}

int main()
{
    char shape[2] = {
        'o',
        'x'};

    point map[COL][LOW];

    int x;
    int y;
    int shape_result = 0;
    while (true)
    {

        cout << "돌의 위치 입력 : ";
        cin >> x >> y;

        if (draw(x, y, map, shape_result, shape))
        {
            if (shape_result)
                shape_result = 0;
            else
                shape_result = 1;
        }
    }

    // 메모리 누수 방지를 위해 할당된 메모리 해제
    for (int i = 0; i < COL; ++i)
    {
        for (int j = 0; j < LOW; ++j)
        {
            if (map[i][j].x != nullptr)
                delete map[i][j].x;
            if (map[i][j].y != nullptr)
                delete map[i][j].y;
        }
    }
}
