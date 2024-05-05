#include <iostream>
#include <vector>

#define ROWS 19
#define COLS 19

using namespace std;

struct point
{
public:
    char shape;
    int x;
    int y;
};

void printMap(vector<vector<char>> map, vector<point> &pointList);
bool dropPoist(vector<point> &pointList, int x, int y, char shape);

int main()
{
    vector<point> pointList;
    vector<vector<char>> map(ROWS, vector<char>(COLS, '*'));

    bool p_status = true; // true o, false x;

    int x;
    int y;

    while (true)
    {
        if (p_status) // o
        {
            cout << "o 돌의 입력 차례입니다. x y : ";
            cin >> x >> y;
            if (x > 19 || x < 0 || y > 19 || y < 0)
            {
                cout << "0~19 사이에 돌을 놓을 수 있습니다." << endl;
            }
            else
            {
                if (dropPoist(pointList, x, y, 'o'))
                    p_status = false;
                else
                {
                    cout << "이미 돌이 놓여 있습니다." << endl;
                    p_status = true;
                }
            }
        }
        else // x
        {
            cout << "x 돌의 입력 차례입니다. x y : ";
            cin >> x >> y;
            if (x > 19 || x < 0 || y > 19 || y < 0)
            {
                cout << "0~19 사이에 돌을 놓을 수 있습니다." << endl;
            }
            else
            {
                if (dropPoist(pointList, x, y, 'x'))
                    p_status = true;
                else
                {
                    cout << "이미 돌이 놓여 있습니다." << endl;
                    p_status = false;
                }
            }
        }

        printMap(map, pointList);

        if (384 == pointList.size())
        {
            cout << "모든 돌이 놓였습니다. 게임을 종료합니다. " << endl;
            exit(0);
        }
    }
}

bool dropPoist(vector<point> &pointList, int x, int y, char shape)
{
    for (int i = 0; i < pointList.size(); ++i)
    {
        if (pointList[i].x == x && pointList[i].y == y)
        {
            return false;
        }
    }

    point p;
    p.shape = shape;
    p.x = x;
    p.y = y;

    pointList.push_back(p);

    return true;
}

void printMap(vector<vector<char>> map, vector<point> &pointList)
{
    bool status = true;
    int o_cnt{};
    int x_cnt{};
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLS; ++j)
        {
            for (int k = 0; k < pointList.size(); ++k)
            {
                if (pointList[k].x == i && pointList[k].y == j)
                {
                    if (pointList[k].shape == 'x')
                        ++x_cnt;
                    else
                        ++o_cnt;

                    cout << pointList[k].shape << " ";
                    status = false;
                }
            }
            if (status)
                cout << map[i][j] << " ";
            else
                status = true;
        }
        cout << endl;
    }
    cout << "x - " << x_cnt << "개, o - " << o_cnt << "개" << endl;
}