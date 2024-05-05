#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

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

class custom_file {
private:
    string name;
    Point(*points)[COL][LOW];
public:
    custom_file(string name, Point(*p)[COL][LOW]) :name(name), points(p) {};

    void saveFile() {
        std::ofstream file(name);
        if (!file.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            return;
        }

        for (int i = 0; i < COL; ++i) {
            for (int j = 0; j < LOW; ++j) {
                file << (*points)[i][j].shape; // �����͸� ����Ͽ� ������ ����
                if (j != LOW - 1)
                    file << ",";
            }
            file << std::endl;
        }

        file.close();
    }

    void loadFile() {
        std::ifstream file(name);
        if (!file.is_open()) {
            std::cerr << "Error opening file for loading!" << std::endl;
            return;
        }

        std::string line;
        int row = 0;

        while (std::getline(file, line) && row < COL) {
            std::istringstream ss(line);
            std::string cell;
            int col = 0;

            while (std::getline(ss, cell, ',') && col < LOW) {
                (*points)[row][col].shape = cell;
                if (cell != "��") {
                    (*points)[row][col].x = new int(col);
                    (*points)[row][col].y = new int(row);
                }
                else {
                    (*points)[row][col].x = nullptr;
                    (*points)[row][col].y = nullptr;
                }
                col++;
            }
            row++;
        }

        file.close();
    }
};

void count_continue(int i, int j, Count_p result[10], Count_p& compare, Point p[COL][LOW], int& size) { //���ӵ� ���� ������ ��ȯ�ϴ� �Լ� (���ӵ� ���� ������ ���� ��� �迭�� ��� ��� ���ӵ� ���� ������ ���)
    if (p[i][j].shape != "��") {
        if (p[i][j].shape == compare.shape) {
            compare.cnt++;
            compare.shape = p[i][j].shape;
            compare.x = j; // ���� ��ǥ
        }
        else {
            compare.cnt = 1;
            compare.shape = p[i][j].shape;
        }
    }
    else {
        compare.cnt = 0;
    }

    if (compare.cnt >= result[size].cnt && compare.cnt > 1) { // �� �˻�
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

    int size_w{}; // ���� ��ü ����
    int size_b{}; // �浹�� ��ü ����

    for (int i = 0; i < COL; ++i) {

        int size_w_w{}; // ������ �� ����
        int size_w_b{}; // ������ �浹 ����
        int size_h_w{}; // ������ �� ����
        int size_h_b{}; // ������ �浹 ����

        Count_p result_w[10]; // ���ӵ� ������ ���� �� ����� ��� �迭 (����)
        Count_p compare_w; // ���� ���ϱ� ���� ����
        Count_p result_h[10]; // ���ӵ� ������ ���� �� ����� ��� �迭 (����)
        Count_p compare_h;

        int result_size_w{}; // �迭�� ũ�⸦ �����ϱ� ���� ���� (����)
        int result_size_h{}; // �迭�� ũ�⸦ �����ϱ� ���� ���� (����)

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
            count_continue(i, j, result_w, compare_w, p, result_size_w);
            count_continue(j, i, result_h, compare_h, p, result_size_h);

            cout << p[i][j].shape;
        }
        cout << "| w" << i << "(��: " << size_w_w << ") (��: " << size_w_b << ") "; // �������� �浹�� �鵹�� ���� (1)
        cout << "| h" << i << "(��: " << size_h_w << ") (��: " << size_h_b << ") "; // �������� �浹�� �鵹�� ���� (1)

        for (int k = 0; k < result_size_w + 1; ++k) { // �������� ���ӵ� ���� (2)
            cout << " | w" << i << " cnt: " << result_w[k].cnt << " shape:" << result_w[k].shape;
        }

        for (int k = 0; k < result_size_h + 1; ++k) { // �������� ���ӵ� ���� (2)
            cout << " | h" << i << " cnt: " << result_h[k].cnt << " shape:" << result_h[k].shape;
        }

        for (int k = 0; k < result_size_w + 1; ++k) { // �������� ���ӵ� ������ ��ǥ (4)
            if (result_w[k].x >= 1 && (result_w[k].x - result_w[k].cnt + 1) != -1)
                cout << " | " << result_w[k].shape << ":(" << result_w[k].x - result_w[k].cnt + 1 << ", " << i << ") ~ (" << result_w[k].x << ", " << i << ") ";
        }
        cout << endl;
    }

    for (size_t i = 0; i < LOW; ++i) {
        cout << "��";
    }
    cout << endl;

    for (int i = 0; i < COL * 2; ++i) {

        Count_p result_slash[10]; // /
        Count_p compare_slash; // ��
        Count_p result_backSlash[10];
        Count_p compare_backSlash;

        int result_size_slash{};
        int result_size_backSlash{};


        for (int j = 0; j <= i; ++j) {
            int k = i - j; // / -> ���� ��ܺ��� ������ �ϴ����� Ž��
            int back_slash = COL - 1 - j; // �� -> ���� �ϴܺ��� ������ ������� Ž��
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
            cout << " | ��" << i << " cnt: " << result_backSlash[k].cnt << " shape:" << result_backSlash[k].shape;// (3)
        }
        cout << endl;
    }


    cout << endl << "�� : " << size_w << " �� : " << size_b << endl << endl; // ��ü �ٵϵ��� ���� ���� ����
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

    string x;
    string y;
    string shape[2]{
       "��",
       "��"
    };
    int user_status{};
    Point p[COL][LOW];
    custom_file customFile("prac.txt", &p);
    customFile.loadFile();
    print_v(p);

    while (true) {
        cout << "��ǥ �� �� ���� �Է��ϼ��� : ";

        cin >> x >> y;

        if (x == "u") { // ������

        }
        
        if (x == "r") { // ������ ���

        }

        if (cin.fail()) {
            cout << "�߸��� �Է°� �Դϴ�." << endl;
            cin.clear();
            break; // �߸��� �Է°��� ������ ����.
        }
        else {
            if (insert_c(stoi(x), stoi(y), shape, user_status, p)) {
                if (user_status)
                    user_status = 0;
                else
                    user_status = 1;

            }
        }
        customFile.saveFile();
    }
}