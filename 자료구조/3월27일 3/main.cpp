#include <iostream>

using namespace std;

#define COL 19
#define LOW 19

struct point
{
    int *x = nullptr;
    int *y = nullptr;
    char shape = '+';
};

bool print(int x, int y, char shape[2], int user_status, point p[COL][LOW])
{

    if (x > 18 || y > 18 || x < 0 || y < 0)
    {
        cout << "0~18 까지의 좌표를 입력해주세요." << endl;
        return false;
    }

    int size_x{};
    int size_o{};

    if (p[x][y].x != nullptr && p[x][y].y != nullptr)
    { // 3번 : 입력된 좌표에 이미 돌이 놓여 있는 경우 출력
        cout << "이미 돌이 놓여 있습니다." << endl;
        return false;
    }
    else
    {
        p[x][y].x = &x;
        p[x][y].y = &y;
        p[x][y].shape = shape[user_status];

        for (size_t i = 0; i < COL; ++i)
        { // 1번 : 바둑판에 화면을 그림
            for (size_t j = 0; j < LOW; ++j)
            {

                if (p[i][j].shape == 'x')
                    size_x++;
                else if (p[i][j].shape == 'o')
                    size_o++;

                cout << p[i][j].shape << " ";
            }
            cout << endl;
        }
        cout << "o의 개수 : " << size_o << "개, x의 개수 : " << size_x << "개" << endl; // 5번 : 바둑판에 놓인 개수
    }
}

int main()
{
    int x;
    int y;
    char shape[2]{
        'o',
        'x'};
    int user_status{};

    point p[COL][LOW];

    while (true)
    {
        cout << "좌표 값 두 곳을 입력하세요 : ";

        try
        {
            cin >> x >> y; // 2번 : 좌표의 위치 값 두 곳을 입력 받음
        }
        catch (exception &e)
        {
            cerr << "오류 발생" << endl;
            exit(0);
        }

        if (print(x, y, shape, user_status, p))
        {

            if (user_status) // 4번 : 검은돌과 흰돌을 번갈아 놓음과 동시에 흰돌 바둑돌의 모양을 바꿔 놓음
                user_status = 0;
            else
                user_status = 1;
        }
    }
}