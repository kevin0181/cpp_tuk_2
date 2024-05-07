#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define COL 19
#define LOW 19

struct Point {
    int x;
    int y;
    string shape;
    Point() :x(-1), y(-1), shape("·") {};
};

struct Count_p {
    int x = -1;
    int cnt = 0;
    string shape;
};

Point moveHistory[COL * LOW];
int moveCount{};

Point redoHistory[COL * LOW];
int redoCount{};

bool undo(Point p[COL][LOW]) {

    if (moveCount <= 0) {
        cout << "무르기를 할 돌이 없습니다." << endl;
        return false;
    }

    Point lastMove = moveHistory[--moveCount];

    redoHistory[redoCount++] = lastMove;
   
    p[lastMove.y][lastMove.x].shape = "·";
    p[lastMove.y][lastMove.x].x = -1;
    p[lastMove.y][lastMove.x].y = -1;
    return true;
}

bool redo(Point p[COL][LOW]) {
    if (redoCount <= 0) {
        cout << "되돌릴 수 있는 돌이 없습니다." << endl;
        return false;
    }

    Point lastRedoMove = redoHistory[--redoCount];
    p[lastRedoMove.y][lastRedoMove.x] = lastRedoMove;
    
    moveHistory[moveCount++] = lastRedoMove;

    return true;
}

void saveFile(Point p[COL][LOW], string fileName) {
    ofstream file(fileName);

    if (!file.is_open()) {
        cout << "파일을 열지 못했습니다." << endl;
        return;
    }

    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            if (p[i][j].shape == "·")
                file << ".";
            else if (p[i][j].shape == "○")
                file << "1";
            else if (p[i][j].shape == "●")
                file << "2";
        }
        file << endl;
    }
    file.close();
}

void loadFile(Point p[COL][LOW], string fileName, int& user_status) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "파일을 열지 못했습니다." << endl;
        return;
    }

    int w{};
    int b{};
    string s;
    for (int i = 0; i < COL; ++i) {
        file >> s;
        for (int j = 0; j < s.length(); ++j) {
            if (s[j] == '.') {
                p[i][j].shape = "·";
                p[i][j].x = -1;
                p[i][j].y = -1;
            }
            else if (s[j] == '1') {
                p[i][j].shape = "○";
                p[i][j].x = j;
                p[i][j].y = i;
                w++;
            }
            else if (s[j] == '2') {
                p[i][j].shape = "●";
                p[i][j].x = j;
                p[i][j].y = i;
                b++;
            }
        }
    }

    user_status = w > b ? 1 : 0;

    file.close();

}

void count_continue(int i, int j, Count_p result[10], Count_p& compare, Point p[COL][LOW], int& size) { //연속된 돌의 개수를 반환하는 함수 (연속된 돌의 개수가 같을 경우 배열에 담아 모든 연속된 돌의 개수를 출력)
    if (p[i][j].shape != "·") {
        if (p[i][j].shape == compare.shape) {
            compare.cnt++;
            compare.shape = p[i][j].shape;
            compare.x = j; // 가로 좌표
        }
        else {
            compare.cnt = 1;
            compare.shape = p[i][j].shape;
        }
    }
    else {
        compare.cnt = 0;
    }

    if (compare.cnt >= result[size].cnt && compare.cnt > 1) { // 비교 검사
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
            count_continue(i, j, result_w, compare_w, p, result_size_w);
            count_continue(j, i, result_h, compare_h, p, result_size_h);

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
                count_continue(j, k, result_slash, compare_slash, p, result_size_slash);
                count_continue(back_slash, k, result_backSlash, compare_backSlash, p, result_size_backSlash);
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
    if (p[y][x].x != -1 && p[y][x].y != -1) {
        cout << "이미 돌이 놓여 있습니다" << endl;
        return false;
    }
    else {
        p[y][x].x = x;
        p[y][x].y = y;
        p[y][x].shape = shape[user_status];

        redoCount = 0;
        moveHistory[moveCount++] = p[y][x];

        print_v(p);

        return true;
    }
}

bool isNumeric(string str) {
    if (str.empty()) {
        return false;
    }

    for (char c : str) {
        if (c < '0' || c > '9') {
            return false;
        }
    }
    return true;
}

int main() {

    string x;
    string y;
    string shape[2]{
       "○",
       "●"
    };
    int user_status{};
    Point p[COL][LOW];
    loadFile(p, "prac.txt", user_status);
    print_v(p);

    while (true) {
        cout << "좌표 값 두 곳을 입력하세요 : ";

        cin >> x;

        if (x == "u") {
            if (undo(p)) {
                user_status = user_status == 0 ? 1 : 0;
                print_v(p);
            }
            continue;
        }

        if (x == "r") {
            if (redo(p)) {
                user_status = user_status == 0 ? 1 : 0;
                print_v(p);
            }
            continue;
        }

        if (!isNumeric(x)) {
            cerr << "문자는 u 또는 r만 가능합니다." << endl;
            continue;
        }

        cin >> y;

        if (!isNumeric(y)) {
            cerr << "문자는 u 또는 r만 가능합니다." << endl;
            continue;
        }

        if (cin.fail()) {
            cout << "잘못된 입력값 입니다." << endl;
            cin.clear();
            break; // 잘못된 입력값이 들어오면 종료.
        }
        else {
            if (insert_c(stoi(x), stoi(y), shape, user_status, p)) {
                if (user_status)
                    user_status = 0;
                else
                    user_status = 1;

            }
        }
        saveFile(p, "prac.txt");
    }
}