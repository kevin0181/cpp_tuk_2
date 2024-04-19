#include <iostream>

using namespace std;

#define COL 19
#define LOW 19

struct Point{
	int* x;
	int* y;
	string shape;

	// □
	Point() :x(nullptr), y(nullptr), shape("+") {};
};

struct Cnt_P {
	int cnt = 0;
	string shape;
	Cnt_P() :cnt(0), shape(" ") {};
};

void cnt_point(Point p[COL][LOW]) { // 가로 세로 돌 개수 새기
	for (size_t i = 0; i < COL; ++i) {

		int size_w{};
		int size_b{};

		Cnt_P result_p;
		Cnt_P compare_p;

		for (size_t j = 0; j < LOW; ++j) {

			if (p[i][j].shape != "+") {
				compare_p.cnt++;
				compare_p.shape = p[i][j].shape;
			}

			if (p[i][j].shape == "○") {
				size_w++;

				if (compare_p.shape == "●") {
					if (result_p.cnt < compare_p.cnt) {
						result_p = compare_p;
						compare_p.cnt = 1;
					}
				}

			}
			else if (p[i][j].shape == "●") {
				size_b++;

				if (compare_p.shape == "○") {
					if (result_p.cnt < compare_p.cnt) {
						result_p = compare_p;
						compare_p.cnt = 1;
					}
				}
			}

			cout << p[i][j].shape << " ";
		}

		cout << " | w cnt: " << result_p.cnt << " shape: " << result_p.shape;

		cout << " | w" << i << " (○:" << size_w << ") (●:" << size_b << ")";

		size_w = 0;
		size_b = 0;

		for (size_t j = 0; j < LOW; ++j) {
			if (p[j][i].shape == "○")
				size_w++;
			else if (p[j][i].shape == "●")
				size_b++;
		}

		cout << " | h" << i << " (○:" << size_w << ") (●: " << size_b << ")" << endl;



	}


	for (size_t i = 0; i < COL; ++i) { // 아래 줄 긋기
		cout << "ㅡ";
	}
	cout << endl;
}


bool print(int x, int y, string shape[2], int user_status, Point p[COL][LOW]) {

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

		cnt_point(p);

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

		if (cin.fail()){
			cout << "잘못된 입력값 입니다." << endl;
			cin.clear();
			break; // 잘못된 입력값이 들어오면 종료.
		}
		else {

			if (print(x, y, shape, user_status, p)) {

				if (user_status)
					user_status = 0;
				else
					user_status = 1;

			}
		}

	}
}
