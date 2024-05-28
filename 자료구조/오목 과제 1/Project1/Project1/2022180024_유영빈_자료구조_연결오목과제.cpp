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
    int y = -1;
    int cnt = 0;
    string shape;
};

void count_continue(int i, int j, Count_p result[10], Count_p& compare, Point p[COL][LOW], int& size, bool check_gap) {
    if (p[i][j].shape != "·") {
        if (p[i][j].shape == compare.shape) {
            compare.cnt++;
            compare.shape = p[i][j].shape;
            compare.x = j;
            compare.y = i;
        }
        else {
            if (check_gap && p[i][j].shape == compare.shape && (abs(compare.x - j) == 2 || abs(compare.y - i) == 2)) {
                compare.cnt++;
                compare.x = j;
                compare.y = i;
            }
            else {
                compare.cnt = 1;
                compare.shape = p[i][j].shape;
            }
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

    int size_w{}; // 흰돌의 전체 개수
    int size_b{}; // 흑돌의 전체 개수

    for (int i = 0; i < COL; ++i) {
        int size_w_w{}; // 가로의 흰돌 개수
        int size_w_b{}; // 가로의 흑돌 개수
        int size_h_w{}; // 세로의 흰돌 개수
        int size_h_b{}; // 세로의 흑돌 개수

        Count_p result_w[10]; // 연속된 돌들의 개수 및 모양을 담는 배열 (가로)
        Count_p compare_w; // 돌을 비교하기 위한 변수
        Count_p result_h[10]; // 연속된 돌들의 개수 및 모양을 담는 배열 (세로)
        Count_p compare_h;

        int result_size_w{}; // 배열의 크기를 추적하기 위한 변수 (가로)
        int result_size_h{}; // 배열의 크기를 추적하기 위한 변수 (세로)

        for (int j = 0; j < LOW; ++j) {
            if (p[i][j].shape == "○") {
                size_w++;
                size_w_w++;
            }
            else if (p[i][j].shape == "●") {
                size_b++;
                size_w_b++;
            }

            if (p[j][i].shape == "○") {
                size_h_w++;
            }
            else if (p[j][i].shape == "●") {
                size_h_b++;
            }
            count_continue(i, j, result_w, compare_w, p, result_size_w, true); // 간격을 확인
            count_continue(j, i, result_h, compare_h, p, result_size_h, true); // 간격을 확인

            cout << p[i][j].shape;
        }
        cout << "| w" << i << "(○: " << size_w_w << ") (●: " << size_w_b << ") "; // 가로줄의 흑돌과 백돌의 개수 (1)
        cout << "| h" << i << "(○: " << size_h_w << ") (●: " << size_h_b << ") "; // 세로줄의 흑돌과 백돌의 개수 (1)

        for (int k = 0; k < result_size_w + 1; ++k) { // 가로줄의 연속된 돌들 (2)
            cout << " | w" << i << " cnt: " << result_w[k].cnt << " shape:" << result_w[k].shape;
        }

        for (int k = 0; k < result_size_h + 1; ++k) { // 세로줄의 연속된 돌들 (2)
            cout << " | h" << i << " cnt: " << result_h[k].cnt << " shape:" << result_h[k].shape;
        }

        for (int k = 0; k < result_size_w + 1; ++k) { // 가로줄의 연속된 돌들의 좌표 (4)
            if (result_w[k].x >= 1 && (result_w[k].x - result_w[k].cnt + 1) != -1)
                cout << " | " << result_w[k].shape << ":(" << result_w[k].x - result_w[k].cnt + 1 << ", " << i << ") ~ (" << result_w[k].x << ", " << i << ") ";
        }
        cout << endl;
    }

    for (size_t i = 0; i < LOW; ++i) {
        cout << "ㅡ";
    }
    cout << endl;

    for (int i = 0; i < COL * 2; ++i) {
        Count_p result_slash[10]; // /
        Count_p compare_slash; // ＼
        Count_p result_backSlash[10];
        Count_p compare_backSlash;

        int result_size_slash{};
        int result_size_backSlash{};

        for (int j = 0; j <= i; ++j) {
            int k = i - j; // / -> 왼쪽 상단부터 오른쪽 하단으로 탐색
            int back_slash = COL - 1 - j; // ＼ -> 왼쪽 하단부터 오른쪽 상단으로 탐색
            if (j < COL && k < LOW) {
                count_continue(j, k, result_slash, compare_slash, p, result_size_slash, true); // 간격을 확인
                count_continue(back_slash, k, result_backSlash, compare_backSlash, p, result_size_backSlash, true); // 간격을 확인
            }
        }

        if (i == 37)
            break;
        for (int k = 0; k < result_size_slash + 1; ++k) {
            cout << " | /" << i << " cnt: " << result_slash[k].cnt << " shape:" << result_slash[k].shape; // (3)
        }

        for (int k = 0; k < result_size_backSlash + 1; ++k) {
            cout << " | ＼" << i << " cnt: " << result_backSlash[k].cnt << " shape:" << result_backSlash[k].shape;// (3)
        }
        cout << endl;
    }

    cout << endl << "○ : " << size_w << " ● : " << size_b << endl << endl; // 전체 바둑돌이 각각 놓인 개수
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
                user_status = user_status ? 0 : 1;
            }
        }
    }
}
