#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <map>

using namespace std;
void throwPlayerPrint(int result, string shapeArray[], int user_status);
void printMap(vector<vector<char>> map, int& p1, int& p2, string shapeArray[], int user_status, int r_result);
void calculateCoordinates(int n, int& x, int& y);

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid(0, 4);

#define ROW 11
#define COL 11
#define MAP_SIZE 6

int main()
{
    vector<vector<char>> map(ROW, vector<char>(COL, ' '));
    int p1 = 0;
    int p2 = 0;

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
            "●" };
        cout << user_status + 1 << "번째 플레이어 입니다. 던지기 위해서는 아무 키를 입력하세요 : ";
        cin >> orderC;
        result = uid(gen); // 랜덤

        throwPlayerPrint(result, shapeArray, user_status); // 그려줌

        cout << "-------윷놀이 판------" << endl;

        printMap(map, p1, p2, shapeArray, user_status, result);

        // player 변경
        if (user_status == 0)
            user_status = 1;
        else
            user_status = 0;
    }
}


int p1_x_status = false;
int p1_y_status = false;
int p2_x_status = false;
int p2_y_status = false;

int p1_xx = 0;
int p1_yy = 0;
int p2_xx = 0;
int p2_yy = 0;
void printMap(vector<vector<char>> map, int& p1, int& p2, string shapeArray[], int user_status, int r_result)
{

    int p1_x = 0;
    int p1_y = 0;
    int p2_x = 0;
    int p2_y = 0;
    
    
        if (user_status == 0)
            p1 += (r_result + 1);
        else
            p2 += (r_result + 1);

        calculateCoordinates(p1, p1_x, p1_y);
        calculateCoordinates(p2, p2_x, p2_y);

        if (p1_x_status && user_status == 0) {
            p1_xx += r_result + 1;
        }

        if (p1_y_status && user_status == 0) {
            p1_yy += r_result + 1;
        }

        if (p2_x_status && user_status == 1) {
            p2_xx += r_result + 1;
        }

        if (p2_y_status && user_status == 1) {
            p2_yy += r_result + 1;
        }

        if (p1_x_status) {
            
            switch (p1_xx)
            {
            case 1:
                p1_x = 2;
                p1_y = 8;
                break;
            case 2:
                p1_x = 4;
                p1_y = 6;
                break;
            case 3:
                p1_x = 5;
                p1_y = 5;
                p1_y_status = true;
                p1_x_status = false;
                p1_yy = 3;
                break;
            case 4:
                p1_x = 6;
                p1_y = 4;
                break;
            case 5:
                p1_x = 8;
                p1_y = 2;
                break;
            case 6:
                p1_x = 10;
                p1_y = 0;
                p1_x_status = false;
                p1_y_status = false;
                p1 = 15;
                break;
            default:
                if (user_status == 0) {
                    p1_x_status = false;
                    p1_y_status = false;
                    //p1 = 15;

                    switch (p1_xx)
                    {
                    case 7:
                        p1 = 16;
                        break;
                    case 8:
                        p1 = 17;
                        break;
                    case 9:
                        p1 = 18;
                        break;
                    case 10:
                        p1 = 19;
                    case 11:
                        p1 = 20;
                        break;
                    default:
                        //p1 += r_result + 1;
                        break;
                    }
                   
                    calculateCoordinates(p1, p1_x, p1_y);
                }
                break;
            }
        }

        if (p1_y_status) {
            switch (p1_yy)
            {
            case 1:
                p1_x = 2;
                p1_y = 2;
                break;
            case 2:
                p1_x = 4;
                p1_y = 4;
                break;
            case 3:
                p1_x = 5;
                p1_y = 5;
                break;
            case 4:
                p1_x = 6;
                p1_y = 6;
                break;
            case 5:
                p1_x = 8;
                p1_y = 8;
                break;
            default:
                if (user_status == 0) {
                    p1_x_status = false;
                    p1_y_status = false;
                    //p1 = 20;
                   
                    switch (p1_yy)
                    {
                    case 6:
                        p1 = 20;
                        break;
                    case 7:
                        p1 = 21;
                        break;
                    default:
                        p1 = 22;
                        break;
                    }

                    calculateCoordinates(p1, p1_x, p1_y);
                }
                break;
            }
        }

        //------

        if (p2_x_status) {

            switch (p2_xx)
            {
            case 1:
                p2_x = 2;
                p2_y = 8;
                break;
            case 2:
                p2_x = 4;
                p2_y = 6;
                break;
            case 3:
                p2_x = 5;
                p2_y = 5;
                p2_y_status = true;
                p2_x_status = false;
                p2_yy = 3;
                break;
            case 4:
                p2_x = 6;
                p2_y = 4;
                break;
            case 5:
                p2_x = 8;
                p2_y = 2;
                break;
            case 6:
                p2_x = 10;
                p2_y = 0;
                p2_x_status = false;
                p2_y_status = false;
                p2 = 15;
                break;
            default:
                if (user_status == 1) {
                    p2_x_status = false;
                    p2_y_status = false;
                    //p1 = 15;

                    switch (p2_xx)
                    {
                    case 7:
                        p2 = 16;
                        break;
                    case 8:
                        p2 = 17;
                        break;
                    case 9:
                        p2 = 18;
                        break;
                    case 10:
                        p2 = 19;
                    case 11:
                        p2 = 20;
                        break;
                    default:
                        //p1 += r_result + 1;
                        break;
                    }

                    calculateCoordinates(p2, p2_x, p2_y);
                }
                break;
            }
        }

        if (p2_y_status) {
            switch (p2_yy)
            {
            case 1:
                p2_x = 2;
                p2_y = 2;
                break;
            case 2:
                p2_x = 4;
                p2_y = 4;
                break;
            case 3:
                p2_x = 5;
                p2_y = 5;
                break;
            case 4:
                p2_x = 6;
                p2_y = 6;
                break;
            case 5:
                p2_x = 8;
                p2_y = 8;
                break;
            default:
                if (user_status == 1) {
                    p2_x_status = false;
                    p2_y_status = false;
                    switch (p2_yy)
                    {
                    case 6:
                        p2 = 20;
                        break;
                    case 7:
                        p2 = 21;
                        break;
                    default:
                        p2 = 22;
                        break;
                    }
                    calculateCoordinates(p2, p2_x, p2_y);
                }
                break;
            }
        }

        switch (p1)
        {
        case 5:
            p1_x_status = true;
            break;
        case 10:
            p1_y_status = true;
        default:
            break;
        }

        switch (p2)
        {
        case 5:
            p2_x_status = true;
            break;
        case 10:
            p2_y_status = true;
        default:
            break;
        }

        
        cout << p1 << " : " << p1_x << ", " << p1_y << endl;
        cout << p2 << " : " << p2_x << ", " << p2_y << endl;

        if (p1 > 20) {
            cout << endl << "1번 player가 승리하였습니다!!!" << endl;
            exit(0);
        }

        if (p2 > 20) {
            cout << endl << "2번 player가 승리하였습니다!!!" << endl;
            exit(0);
        }

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
                if (p2_x == 5 && p2_y == 5)
                {
                    cout << shapeArray[1] << " ";
                }
                else if (p1_x == 5 && p1_y == 5)
                {
                    cout << shapeArray[0] << " ";
                }
                else
                {
                    cout << "□"
                        << " ";
                }
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
                if (p2_x == i && p2_y == j)
                {
                    cout << shapeArray[1] << " ";
                }
                else if (p1_x == i && p1_y == j)
                {
                    cout << shapeArray[0] << " ";
                }
                else
                {
                    cout << "□"
                        << " ";
                }
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

void calculateCoordinates(int value, int& x, int& y)
{

    if (value <= 5)
    {
        x = 10 - value * 2;
        y = 10;
    }
    else if (value <= 10)
    {
        x = 0;
        y = 10 - (value - 5) * 2;
    }
    else if (value <= 15)
    {
        x = (value - 10) * 2;
        y = 0;
    }
    else
    {
        x = 10;
        y = (value - 15) * 2;
    }
}