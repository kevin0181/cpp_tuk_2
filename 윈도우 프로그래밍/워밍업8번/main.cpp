#include <iostream>
#include <vector>
#include <random>
#include <string>
using namespace std;
void throwPlayerPrint(int result, string shapeArray[], int user_status);
void printMap(vector<vector<char>> map);

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid(0, 4);

#define ROW 11
#define COL 11

int main()
{
    vector<vector<char>> map(ROW, vector<char>(COL, ' '));

    bool w_status = false;
    char orderC{};
    cout << "윷놀이를 시작하시겠습니까? 시작 : p : ";
    cin >> orderC;
    if (orderC == 'p')
        w_status = true;
    else
        exit(0);
    cout << "-------------윷놀이------------" << endl;
    int user_status = 0; // 0 = player1, 1 = player2
    while (w_status)
    {
        int result{}; // 윷놀이 결과값
        string shapeArray[] = {
            "■",
            "●"};
        cout << user_status + 1 << "번째 플레이어 입니다. 던지기 위해서는 s를 입력하세요 : ";
        cin >> orderC;
        result = uid(gen);                                 // 랜덤
        throwPlayerPrint(result, shapeArray, user_status); // 그려줌

        cout << "-------윷놀이 판------" << endl;
        printMap(map);
        if (user_status == 0)
            user_status = 1;
        else
            user_status = 0;

        for (int i = result; i < 0; --result)
        {
        }
    }
}

void printMap(vector<vector<char>> map)
{
    for (int i = 0; i < ROW; ++i)
    {
        for (int j = 0; j < COL; ++j)
        {

            if (i == 2 && j == 4 || i == 2 && j == 6 || i == 4 && j == 2 || i == 6 && j == 2 || i == 6 && j == 8 || i == 4 && j == 8 || i == 8 && j == 6 || i == 8 && j == 4)
            {
                cout << " "
                     << " ";
            }
            else if (i == 5 && j == 5)
            {
                cout << "□"
                     << " ";
            }
            else if (j % 2 == 1)
            {
                cout << " "
                     << " ";
            }
            else if (i % 2 == 1)
            {
                cout << " "
                     << " ";
            }
            else
            {
                cout << "□"
                     << " ";
            }
        }
        cout << endl;
    }
}

void throwPlayerPrint(int result, string shapeArray[], int user_status) // 윷 놀이 상태 보여지기만
{                                                                       // 윷 던지기
    int max = 5;
    for (int i = 0; i < max; ++i)
    {
        if (i < result + 1)
        {
            cout << " " << shapeArray[user_status] << " ";
        }
        else
        {
            cout << " □ ";
        }
    }

    switch (result)
    {
    case 0:
        cout << "  도!";
        break;
    case 1:
        cout << "  개!";
        break;
    case 2:
        cout << "  걸!";
        break;
    case 3:
        cout << "  윷!";
        break;
    case 4:
        cout << "  모!";
        break;
    default:
        break;
    }
    cout << endl;
}