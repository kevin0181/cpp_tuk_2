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

struct Count_p {
    int x = -1;
    int cnt = 0;
    string shape;
};

void count_continue(int i, int j, Count_p result[10], Count_p& compare, Point p[COL][LOW], int& size) {
    if (p[i][j].shape != "��") {
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

    cout << "(x,y) x ��  y��" << endl;

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
            if (p[i][j].shape == "��") {
                size_w++;
                size_w_w++;
            }
            else if (p[i][j].shape == "��") {
                size_b++;
                size_w_b++;
            }

            if (p[j][i].shape == "��") {
                size_h_w++;
            }
            else if (p[j][i].shape == "��") {
                size_h_b++;
            }

            cout << p[i][j].shape;

            count_continue(i, j, result_w, compare_w, p, size1);
            count_continue(j, i, result_h, compare_h, p, size2);

        }
        cout << "| w" << i << " (�� : " << size_w_w << ") (�� : " << size_w_b << ") " << endl;
        cout << "| h" << i << " (�� : " << size_h_w << ") (�� : " << size_h_b << ") " << endl;

        for (int k = 0; k < size1; ++k) {
            cout << " | w" << i << " cnt:" << result_w[k].cnt << " shape: " << result_w[k].shape;
        }
    }

    for (size_t i = 0; i < LOW; ++i) {
        cout << "��";
    }
    cout << endl << "�� : " << size_w << " �� : " << size_b << endl << endl;
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