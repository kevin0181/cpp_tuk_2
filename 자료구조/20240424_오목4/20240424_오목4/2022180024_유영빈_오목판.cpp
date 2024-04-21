#include <iostream>
#include <string>

using namespace std;

#define COL 19
#define LOW 19

struct Point {
    int* x;
    int* y;
    string shape;
    Point() :x(nullptr), y(nullptr), shape("��") {};
};

void print_v(Point p[COL][LOW]) {

    cout << "(x,y) x ��  y��" << endl;

    int size_w_w{};
    int size_w_b{};

    for (int i = 0; i < COL; ++i) {
        for (int j = 0; j < LOW; ++j) {
            if (p[i][j].shape == "��")
                size_w_w++;
            else if (p[i][j].shape == "��")
                size_w_b++;
            cout << p[i][j].shape;
        }
        cout << "|" << endl;
    }

    for (size_t i = 0; i < LOW; ++i) {
        cout << "��";
    }
    cout << endl << "�� : " << size_w_w << " �� : " << size_w_b << endl << endl;
}

bool insert_c(int x, int y, string shape[2], int user_status, Point p[COL][LOW]) {

    if (x > 18 || y > 18 || x < 0 || y < 0) {
        cout << "0~18 ������ ��ǥ�� �Է����ּ���." << endl;
        return false;
    }
    if (p[y][x].x != nullptr && p[y][x].y != nullptr) {
        cout << "�̹� ���� ���� �ֽ��ϴ�" << endl;
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
       "��",
       "��"
    };
    int user_status{};
    Point p[COL][LOW];

    while (true) {
        cout << "��ǥ �� �� ���� �Է��ϼ��� : ";

        cin >> x >> y;

        if (cin.fail()) {
            cout << "�߸��� �Է°� �Դϴ�." << endl;
            cin.clear();
            break; // �߸��� �Է°��� ������ ����.
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