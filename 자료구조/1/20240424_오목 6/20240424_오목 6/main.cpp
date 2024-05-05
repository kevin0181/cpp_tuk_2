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
    int cnt;
    int x;
    string shape;
    Count_p() :cnt(0), x(-1), shape(" ") {  };
};

void count_continue(int i, int j, Count_p result[10], Count_p& compare, Point p[COL][LOW], int& size) {
    if (p[i][j].shape != "·") {
        if (p[i][j].shape == " " || p[i][j].shape == compare.shape) {
            compare.cnt++;
            compare.shape = p[i][j].shape;
            compare.x = j;
        }
        else {
            if (compare.cnt >= 2) {  // 2 이상의 연속만 result에 저장
                result[size++] = compare; // size를 업데이트하며 배열에 추가
                if (size >= 10) size = 9;  // 배열 크기를 넘지 않도록 조절
            }
            compare.cnt = 1;
            compare.shape = p[i][j].shape;
        }
    }
    else {
        if (compare.cnt >= 2) {
            result[size++] = compare;
            if (size >= 10) size = 9;
        }
        compare.cnt = 0;
        compare.shape = " ";
    }

    if (j == LOW - 1 && compare.cnt >= 2) {  // 마지막 열에서도 확인
        result[size++] = compare;
        if (size >= 10) size = 9;
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

        Count_p compare_w;
        Count_p compare_h;

        Count_p result_list_w[10]; //연속될 수 있는 최대 개수
        Count_p result_list_h[10];

        int size_list_w{};
        int size_list_h{};

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
            cout << p[i][j].shape << " ";
            count_continue(i, j, result_list_w, compare_w, p, size_list_w);
            count_continue(j, i, result_list_h, compare_h, p, size_list_h);
        }
        cout << " | w" << i << " (" << "○ : " << size_w_w << ") (● : " << size_w_b << ")";
        cout << " | h" << i << " (" << "○ : " << size_h_w << ") (● : " << size_h_b << ")";

        for (int k = 0; k < size_list_w; ++k) {
            cout << " | w" << i << " cnt: " << result_list_w[k].cnt << " shape: " << result_list_w[k].shape;
        }
        for (int k = 0; k < size_list_h; ++k) {
            cout << " | h" << i << " cnt: " << result_list_h[k].cnt << " shape: " << result_list_h[k].shape;
        }
        
        for (int k = 0; k < size_list_w; ++k) {
            cout << " (" << result_list_w[k].x - result_list_w[k].cnt + 1 << ", " << i << ") ~ (" << result_list_w[k].x << ", " << i << ")";
        }
        
        cout << endl;
    }

    for (size_t i = 0; i < LOW; ++i) {
        cout << "ㅡ";
    }
    cout << endl;

    for (int i = 0; i < COL * 2; ++i) {

        //Count_p result_slash;
        Count_p compare_slash;
        //Count_p result_backSlash;
        Count_p compare_backSlash;

        Count_p result_list_slash[10];
        Count_p result_list_backSlash[10];

        int size_list_slash{};
        int size_list_backSlash{};

        for (int j = 0; j <= i; ++j) {
            int k = i - j;
            int back_slash = COL - 1 - j;
            if (j < COL && k < LOW) {
                // 주 대각선(/) 검사
                count_continue(j, k, result_list_slash, compare_slash, p, size_list_slash);
                // 부 대각선(\) 검사
                count_continue(back_slash, k, result_list_backSlash, compare_backSlash, p, size_list_backSlash);

                // 대각선의 마지막 요소 처리
                if (j == i || k == 0 || back_slash == 0) {
                    if (compare_slash.cnt >= 2) {
                        result_list_slash[size_list_slash++] = compare_slash;
                        if (size_list_slash >= 10) size_list_slash = 9;
                    }

                    if (compare_backSlash.cnt >= 2) {
                        result_list_backSlash[size_list_backSlash++] = compare_backSlash;
                        if (size_list_backSlash >= 10) size_list_backSlash = 9;
                    }
                }

            }
        }
        //if (i == 37)break;
        for (int k = 0; k < size_list_slash; ++k) {
            cout << " | /" << i << " cnt: " << result_list_slash[k].cnt << " shape: " << result_list_slash[k].shape << endl;
        }
        for (int k = 0; k < size_list_backSlash; ++k) {
            cout << " | b" << i << " cnt: " << result_list_backSlash[k].cnt << " shape: " << result_list_backSlash[k].shape << endl;
        }
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