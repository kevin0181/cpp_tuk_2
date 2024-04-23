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
    int cnt{};
    string shape;
};

void count_continue(int i, int j, Count_p result[10], Count_p& compare, Point p[COL][LOW], int& size) {
    if (p[i][j].shape != "��") {
        if (compare.shape == p[i][j].shape) {
            compare.cnt++;
            compare.shape = p[i][j].shape;
            compare.x = j; // ��ǥ �������� ���ؼ�
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

        int size_result_w{};
        int size_result_h{};

        for (int j = 0; j < LOW; ++j) {
            if (p[i][j].shape == "��") { size_w_w++; size_w++; }
            else if (p[i][j].shape == "��") { size_w_b++; size_b++; };
                
            if (p[j][i].shape == "��") size_h_w++;
            else if (p[j][i].shape == "��") size_h_b++;

            count_continue(i, j, result_w, compare_w, p, size_result_w); //���� ���ӵ� �� ���ϱ�
            count_continue(j, i, result_h, compare_h, p, size_result_h); //���� ���ӵ� �� ���ϱ�

            cout << p[i][j].shape;
        }
        cout << "| (��: " << size_w_w << ") (��: " << size_w_b << ")"; //����
        cout << "| (��: " << size_h_w << ") (��: " << size_h_b << ")"; //����

        //���� 2��, 4�� ������ �����ؼ� �ּ� ó����
        //for (int k = 0; k < size_result_w + 1; ++k) {
        //    cout << " | w" << i << " cnt:" << result_w[k].cnt << " shape:" << result_w[k].shape; //���� ���ӵ� �� ���� �� �� ��� (�� ������ ������ �ΰ� ���)
        //}

        for (int k = 0; k < size_result_h + 1; ++k) {
            cout << " | h" << i << " cnt:" << result_h[k].cnt << " shape:" << result_h[k].shape; //���� ���ӵ� �� ���� �� �� ��� (�� ������ ������ �ΰ� ���)
        }
        
        for (int k = 0; k < size_result_w + 1; ++k) {
            if (result_w[k].x > 0 && (result_w[k].x - result_w[k].cnt + 1) != -1)
                cout << " | " << result_w[k].shape << ":(" << result_w[k].x - result_w[k].cnt + 1 << ", " << i << ") ~ (" << result_w[k].x << ", " << i << ")";
        }

        cout << endl;
    }

    for (size_t i = 0; i < LOW; ++i) {
        cout << "��";
    }
    cout << endl;

    for (int i = 0; i < COL * 2; ++i) {

        Count_p result_slash[10];
        Count_p compare_slash;
        Count_p result_backSlash[10];
        Count_p compare_backSlash;

        int size_result_slash{};
        int size_result_backSlash{};

        for (int j = 0; j <= i; ++j) {
            int k = i - j; // slash -> ���� ��� -> ������ �ϴ�
            int back_slash = COL - 1 - j; // backSlash -> �����ϴ� -> ������ ���

            if (j < COL && k < LOW) {
                count_continue(j, k, result_slash, compare_slash, p, size_result_slash);
                count_continue(back_slash, k, result_backSlash, compare_backSlash, p, size_result_backSlash);
            }
        }
        if (i == 37)
            break;
        for (int k = 0; k < size_result_slash + 1; ++k) {
            cout << " | /" << i << " cnt:" << result_slash[k].cnt << " shape:" << result_slash[k].shape; // �밢�� / ���ӵ� �� ���� �� �� ��� (�� ������ ������ �ΰ� ���)
        }
        for (int k = 0; k < size_result_backSlash + 1; ++k) {
            cout << " | ��" << i << " cnt:" << result_backSlash[k].cnt << " shape:" << result_backSlash[k].shape; // �밢�� �� ���ӵ� �� ���� �� �� ��� (�� ������ ������ �ΰ� ���)
        }
        cout << endl;
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