#include <iostream>
#include "point.h"

using namespace std;

void sprint_v(char back[10][10], point p1, point p2);
void fillSpace(char back[10][10]);
void pointMove(point &, int, int);

int main()
{
    cout << "player1 = wasd, player 2 = ijkl" << endl;
    bool player_status = true;
    char back[10][10];
    char result{};
    fillSpace(back);

    point p1 = {2, 5, 'x'};
    point p2 = {7, 5, 'o'};
    sprint_v(back, p1, p2);

    while (true)
    {
        cin >> result;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 문자 여러개 입력하는거 방지 (입력된 버퍼 지움)

        if (player_status)
        {
            cout << "----- player1 턴 ----" << endl;
            switch (result)
            {
            case 'w':
                player_status = false;
                pointMove(p1, 0, -1);
                break;
            case 'a':
                player_status = false;
                break;
            case 's':
                player_status = false;
                break;
            case 'd':
                player_status = false;
                break;
            default:
                cout << "player1 차례입니다. player1가 입력해주세요." << endl;
                break;
            }
            sprint_v(back, p1, p2);
        }
        else
        {
            cout << "----- player2 턴 ----" << endl;
            switch (result)
            {
            case 'i':
                player_status = true;
                break;
            case 'j':
                player_status = true;
                break;
            case 'k':
                player_status = true;
                break;
            case 'l':
                player_status = true;
                break;
            default:
                cout << "player2 차례입니다. player2가 입력해주세요." << endl;
                break;
            }
            sprint_v(back, p2, p1);
        }
    }
}

void pointMove(point &p, int x, int y)
{
    p.x = p.x + x;
    p.y = p.y + y;
}

void sprint_v(char back[10][10], point p1, point p2)
{
    for (int i = 0; i < 10; ++i) // y축
    {
        cout << "---------------------" << endl;
        cout << "|";
        for (int j = 0; j < 10; ++j) // x축
        {
            if (p1.y == i && p1.x == j)
            {
                cout << p1.shape << "|";
            }
            else if (p2.y == i && p2.x == j)
            {
                cout << p2.shape << "|";
            }
            else
            {
                cout << back[i][j] << "|";
            }
        }
        cout << endl;
    }
    cout << "---------------------" << endl;
}

void fillSpace(char back[10][10])
{
    for (int i = 0; i < 10; ++i)
    {

        for (int j = 0; j < 10; ++j)
        {
            back[i][j] = ' ';
        }
    }
}