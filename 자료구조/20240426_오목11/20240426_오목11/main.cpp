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

struct Count_p {
    int x = -1;
    int cnt = 0;
    string shape;
};

void count_continue(int i, int j, Count_p result[10], Count_p& compare, Point p[COL][LOW], int& size) {
    if (p[i][j].shape != "·") {
        if (compare.shape == p[i][j].shape) {
            compare.cnt++;
            compare.shape = p[i][j].shape;
            compare.x = j;
        }
        else {
            compare.cnt = 1;
            compare.shape = p[i][j].shape;
        }
    }
    else {
        compare.cnt = 0;
    }

    if (compare.cnt >= result[size].cnt && compare.cnt > 1) {
        if (compare.cnt == result[size].cnt) {
            result[++size] = compare;
        }
        else {
            result[0] = compare;
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

        Count_p result_w[10];
        Count_p compare_w;
        Count_p result_h[10];
        Count_p compare_h;

        int size1{};
        int size2{};
        for (int j = 0; j < LOW; ++j) {
            if (p[i][j].shape == "○") { size_w_w++; size_w++; }
            else if (p[i][j].shape == "●") { size_w_b++; size_b++; }

            if (p[j][i].shape == "○") size_h_w++;
            else if (p[j][i].shape == "●") size_h_b++;

            count_continue(i, j, result_w, compare_w, p, size1);
            count_continue(j, i, result_h, compare_h, p, size2);

            cout << p[i][j].shape;
        }
        cout << "| w"<<i << " (○ : " << size_w_w << ") (● : " << size_w_b << ") ";
        cout << "| h" << i << " (○ : " << size_h_w << ") (● : " << size_h_b << ") ";

        for (int k = 0; k < size1 + 1; ++k) {
            cout << "| w" << i << " cnt: " << result_w[k].cnt << " shape: " << result_w[k].shape;
        }

        for (int k = 0; k < size2 + 1; ++k) {
            cout << "| h" << i << " cnt: " << result_h[k].cnt << " shape: " << result_h[k].shape;
        }

        for (int k = 0; k < size1 + 1; ++k) {
            if (result_w[k].x >= 1 && (result_w[k].x - result_w[k].cnt + 1) != -1)
                cout << "| " << result_w[k].shape << ": (" << result_w[k].x - result_w[k].cnt + 1 << ", " << i << ") ~ (" << result_w[k].x << ", " << i << ") ";
        }
        cout << endl;
    }

    for (size_t i = 0; i < LOW; ++i) {
        cout << "ㅡ";
    }
    cout << endl;

    for (int i = 0; i < COL * 2; ++i) {

        Count_p result_slash[10];
        Count_p compare_slash;
        Count_p result_backSlash[10];
        Count_p compare_backSlash;

        int size1{};
        int size2{};

        for (int j = 0; j <= i; ++j) {
            int k = i - j;
            int back_slash = COL - 1 - j;

            if (j < COL && k < LOW) {
                count_continue(j, k, result_slash, compare_slash, p, size1);
                count_continue(back_slash, k, result_backSlash, compare_backSlash, p, size2);
            }
        }

        if (i == 37)
            break;

        for (int k = 0; k < size1 + 1; ++k) {
            cout << "| /" << i << " cnt: " << result_slash[k].cnt << " shape: " << result_slash[k].shape;
        }
        for (int k = 0; k < size2 + 1; ++k) {
            cout << "| ＼" << i << " cnt: " << result_backSlash[k].cnt << " shape: " << result_backSlash[k].shape;
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