#include <iostream>
#include <string>

using namespace std;

#define COL 19
#define LOW 19

struct Point {
    int* x;
    int* y;
    string shape;
    Point() :x(nullptr), y(nullptr), shape("·") {};
};

struct Count_p
{
    int cnt;
    int x;
    int y;
    string shape;
    Count_p() :cnt(0), x(-1), y(-1), shape(" ") {};
};

void count_continue(int& i, int& j, Count_p result_p[10], Count_p& compare_p, Point p[COL][LOW],int& size) {
    if (p[i][j].shape != "·") {
        if (p[i][j].shape == compare_p.shape) {
            compare_p.shape = p[i][j].shape;
            compare_p.cnt++;
            compare_p.x = j;
        }
        else {
            compare_p.cnt = 1;
            compare_p.shape = p[i][j].shape;
        }
    }
    else {
        compare_p.cnt = 0;
    }

    if (compare_p.cnt >= result_p[size].cnt && compare_p.cnt > 1) {
        if (compare_p.cnt == result_p[size].cnt) {
            result_p[++size] = compare_p;
        }
        else {
            result_p[0] = compare_p;
            size = 0;
        }
    }
}

void print_v(Point p[COL][LOW]) {

    cout << "(x,y) x →  y↓" << endl;

    int size_w{};
    int size_b{};

    for (int i = 0; i < COL; ++i) {

        int size_w_w{};
        int size_w_b{};
        int size_h_w{};
        int size_h_b{};

        Count_p result_p1[10];
        Count_p compare_p1;
        Count_p result_p2[10];
        Count_p compare_p2;
        int size1 = 0;
        int size2 = 0;
        for (int j = 0; j < LOW; ++j) {
            if (p[i][j].shape == "○") {
                size_w_w++;
                size_w++;
            }
            else if (p[i][j].shape == "●") {
                size_w_b++;
                size_b++;
            }

            if (p[j][i].shape == "○")
                size_h_w++;
            else if (p[j][i].shape == "●")
                size_h_b++;

            cout << p[i][j].shape << " ";

            count_continue(i, j, result_p1, compare_p1, p, size1);
            count_continue(j, i, result_p2, compare_p2, p, size2);
        }

        cout << " | w" << i << " (○ : " << size_w_w << ") (● : " << size_w_b << ")";
        cout << " | h" << i << " (○ : " << size_h_w << ") (● : " << size_h_b << ")";
        for (int k = 0; k < size1 + 1; ++k) {
            cout << " | w" << i << " cnt: " << result_p1[k].cnt << " shape:" << result_p1[k].shape;
        }
        for (int k = 0; k < size2 + 1; ++k) {
            cout << " | h" << i << " cnt: " << result_p2[k].cnt << " shape:" << result_p2[k].shape;
        }
        for (int k = 0; k < size1 + 1; ++k) {
            if (result_p1[k].x >= 1 && (result_p1[k].x - result_p1[k].cnt + 1) != -1)
                cout << " (" << result_p1[k].x - result_p1[k].cnt + 1 << ", " << i << ") ~ (" << result_p1[k].x << ", " << i << ")";
        }
        cout << endl;
    }

    for (size_t i = 0; i < LOW; ++i) {
        cout << "ㅡ";
    }
    cout << endl;

    for (int i = 0; i < COL * 2; ++i) {
        Count_p result_p1[10];
        Count_p compare_p1;
        Count_p result_p2[10];
        Count_p compare_p2;
        int size1 = 0;
        int size2 = 0;
        for (int j = 0; j <= i; ++j) {
            int k = i - j;
            int back_slash = COL - 1 - j;
            if (j < COL && k < LOW) {
                count_continue(j, k, result_p1, compare_p1, p, size1);
                count_continue(back_slash, k, result_p2, compare_p2, p, size2);
            }
        }

        if (i == 37)
            break;
        for (int k = 0; k < size1 + 1; ++k) {
            cout << i << " / " << " cnt: " << result_p1[k].cnt << " shape: " << result_p1[k].shape;
        }
        for (int k = 0; k < size2 + 1; ++k) {
            cout << i << " ＼ " << " cnt: " << result_p2[k].cnt << " shape: " << result_p2[k].shape;
        }
        cout << endl;

    }

    cout << endl << "○ : " << size_w << " ● : " << size_b << endl << endl;
}

bool insert_c(int x, int y, string shape[2], int user_status, Point p[COL][LOW]) {

    if (x > 18 || y > 18 || x < 0 || y < 0) {
        cout << "0~18 까지의 좌표를 입력해주세요." << endl;
        return false;
    }
    if (p[y][x].x != nullptr && p[y][x].y != nullptr) {
        cout << "이미 돌이 놓여 있습니다" << endl;
        return false;
    }
    else {
        p[y][x].x = &x;
        p[y][x].y = &y;
        p[y][x].shape = shape[user_status];

        print_v(p);

        return true;
    }
}

int main() {

    int x;
    int y;
    string shape[2]{
       "○",
       "●"
    };
    int user_status{};
    Point p[COL][LOW];

    while (true) {
        cout << "좌표 값 두 곳을 입력하세요 : ";

        cin >> x >> y;

        if (cin.fail()) {
            cout << "잘못된 입력값 입니다." << endl;
            cin.clear();
            break; // 잘못된 입력값이 들어오면 종료.
        }
        else {
            if (insert_c(x, y, shape, user_status, p)) {
                if (user_status)
                    user_status = 0;
                else
                    user_status = 1;

            }
        }
    }
}