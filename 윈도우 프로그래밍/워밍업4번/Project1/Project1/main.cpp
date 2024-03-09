#include <iostream>
#include "point.h"
#include <random>
#include <windows.h> 

using namespace std;

void sprint_v(char back[10][10], point p1, point p2);
void fillSpace(char back[10][10]);
void pointMove(point&, int, int);
void changeShape(point& p);
bool checkShape(point& p, char oldP);
void checkXY(point& p);

point p1 = { 2, 5, 'x' };
point p2 = { 7, 5, 'o' };

int main()
{
    cout << "player1 = wasd, player 2 = ijkl" << endl;
    bool player_status = true;
    char back[10][10];
    char result{};
    fillSpace(back);


    sprint_v(back, p1, p2);

    while (true)
    {
        cin >> result;
       // cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 문자 여러개 입력하는거 방지 (입력된 버퍼 지움)

        if (player_status)
        {
            cout << "----- player1 턴 ----" << endl;
            switch (result)
            {
            case 'w':
                player_status = false;
                pointMove(p1, 0, -1);
                checkXY(p2);
                sprint_v(back, p1, p2);
                break;
            case 'a':
                player_status = false;
                pointMove(p1, -1, 0);
                checkXY(p2);
                sprint_v(back, p1, p2);
                break;
            case 's':
                player_status = false;
                pointMove(p1, 0, 1);
                checkXY(p2);
                sprint_v(back, p1, p2);
                break;
            case 'd':
                player_status = false;
                pointMove(p1, 1, 0);
                checkXY(p2);
                sprint_v(back, p1, p2);
                break;
            default:
                cout << "player1 차례입니다. player1가 입력해주세요." << endl;
                break;
            }
        }
        else
        {
            cout << "----- player2 턴 ----" << endl;
            switch (result)
            {
            case 'i':
                player_status = true;
                pointMove(p2, 0, -1);
                checkXY(p1);
                sprint_v(back, p2, p1);
                break;
            case 'j':
                player_status = true;
                pointMove(p2, -1, 0);
                checkXY(p1);
                sprint_v(back, p2, p1);
                break;
            case 'k':
                player_status = true;
                pointMove(p2, 0, 1);
                checkXY(p1);
                sprint_v(back, p2, p1);
                break;
            case 'l':
                player_status = true;
                pointMove(p2, 1, 0);
                checkXY(p1);
                sprint_v(back, p2, p1);
                break;
            default:
                cout << "player2 차례입니다. player2가 입력해주세요." << endl;
                break;
            }
            
        }
    }
}

void checkXY(point &p) {
    if (p1.x == p2.x && p1.y == p2.y) {
        Beep(500, 500);
        changeShape(p);
        if (p1.shape == p2.shape)
            changeShape(p);
   }
}

void pointMove(point& p, int x, int y)
{
    p.x = p.x + x;
    p.y = p.y + y;
    switch (p.x)
    {
    case 10:
        p.x = 0;
        changeShape(p);
        if (p1.shape == p2.shape)
            changeShape(p);
        break;
    case -1:
        changeShape(p);
        if (p1.shape == p2.shape)
            changeShape(p);
        p.x = 9;
    default:
        break;
    }

    switch (p.y)
    {
    case 10:
        p.y = 0;
        changeShape(p);
        if (p1.shape == p2.shape)
            changeShape(p);
        break;
    case -1:
        p.y = 9;
        changeShape(p);
        if (p1.shape == p2.shape)
            changeShape(p);
        break;
    default:
        break;
    }
}
void changeShape(point &p) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> uid(0, 4);

    int ran = uid(gen);

    char oldP{};
    oldP = p.shape;

    switch (ran)
    {
    case 0:
        p.shape = 'x';
        break;
    case 1:
        p.shape = 'o';
        break;
    case 2:
        p.shape = '#';
        break;
    case 3:
        p.shape = '@';
        break;
    case 4:
        p.shape = '*';
        break;
    default:
        break;
    }

    if (checkShape(p, oldP))
        changeShape(p);
    
}

bool checkShape(point &p,char oldP) { // 했던거 똑같이 하는거 방지
    if (p.shape == oldP)
        return true;
    else
        return false;
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