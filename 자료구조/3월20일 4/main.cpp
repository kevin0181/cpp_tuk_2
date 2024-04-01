#include <iostream>

using namespace std;

#define COL 19
#define LOW 19

struct point {
    int *x = nullptr;
    int *y = nullptr;
    char shape{'+'};
};

bool print(int x, int y, char shape[2], int userStatus, point p[COL][LOW]) {
    int size_x{};
    int size_o{};

    if (x > 18 || y > 18) {
        cout << "범위 밖입니다." << endl;
        return false;
    }

    if (p[x][y].x != nullptr && p[x][y].y != nullptr) {
        cout << "이미 돌이 놓여있습니다." << endl;
        return false;
    } else {
        p[x][y].x = &x;
        p[x][y].y = &y;
        p[x][y].shape = shape[userStatus];
        for (size_t i = 0; i < COL; ++i) {
            for (size_t j = 0; j < LOW; ++j) {
                if (p[i][j].shape == 'o')
                    size_o++;
                else if (p[i][j].shape == 'x')
                    size_x++;

                cout << p[i][j].shape << " ";
            }
            cout << endl;
        }
        cout << "o 돌 개수 : " << size_o << "개, x 돌 개수 : " << size_x << "개" << endl;
        return true;
    }
}

int main() {
    int x;
    int y;
    char shape[2]{
        'o',
        'x'
    };
    int userStatus{};
    point p[COL][LOW];

    while (true) {
        cout << "좌표를 입력하세요 x y : ";
        cin >> x >> y;

        if (print(x, y, shape, userStatus, p)) {
            if (userStatus)
                userStatus = 0;
            else
                userStatus = 1;
        }
    }
}
