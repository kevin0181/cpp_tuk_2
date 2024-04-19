#include <iostream>

using namespace std;

#define COL 19
#define LOW 19

struct Point{
	int* x;
	int* y;
	string shape;

	// □
	Point() :x(nullptr), y(nullptr), shape("·") {};
};

struct Cnt_P {
	int cnt = 0;
	string shape;
	int *x;
	int *y;
	Cnt_P() :cnt(0), shape(" "), x(nullptr), y(nullptr) {};
};

void print_col_continue(size_t& i, size_t& j, Cnt_P& result_p, Cnt_P& compare_p, Point p[COL][LOW]) {
	// 현재 위치에 돌이 놓여 있지 않은 경우, 연속된 돌이 끊긴 것으로 처리합니다.
	if (p[i][j].shape == "·") {
		if (compare_p.cnt > result_p.cnt) {
			result_p = compare_p;
		}
		// compare_p를 초기화합니다.
		compare_p.cnt = 0;
		compare_p.shape = " ";
	}
	// 현재 위치에 돌이 있는 경우
	else {
		// compare_p에 돌이 설정되어 있지 않거나, 현재 돌이 compare_p의 돌과 같은 경우 카운트를 증가시킵니다.
		if (compare_p.shape == " " || compare_p.shape == p[i][j].shape) {
			compare_p.x = p[i][j].x;
			compare_p.y = p[i][j].y;
			compare_p.shape = p[i][j].shape;
			compare_p.cnt++;
		}
		// 현재 돌이 compare_p의 돌과 다른 경우, 연속이 끊긴 것으로 처리합니다.
		else {
			if (compare_p.cnt > result_p.cnt) {
				result_p = compare_p;
			}
			// compare_p를 현재 돌로 초기화합니다.
			compare_p.shape = p[i][j].shape;
			compare_p.cnt = 1;
		}
	}
}

void print_low_continue(size_t& i, size_t& j, Cnt_P& result_p, Cnt_P& compare_p, Point p[COL][LOW]) {
	// 현재 위치에 돌이 놓여 있지 않은 경우, 연속된 돌이 끊긴 것으로 처리합니다.
	if (p[j][i].shape == "·") {
		if (compare_p.cnt > result_p.cnt) {
			result_p = compare_p;
		}
		// compare_p를 초기화합니다.
		compare_p.cnt = 0;
		compare_p.shape = " ";
	}
	// 현재 위치에 돌이 있는 경우
	else {
		// compare_p에 돌이 설정되어 있지 않거나, 현재 돌이 compare_p의 돌과 같은 경우 카운트를 증가시킵니다.
		if (compare_p.shape == " " || compare_p.shape == p[j][i].shape) {
			compare_p.x = p[j][i].x;
			compare_p.y = p[j][i].y;
			compare_p.shape = p[j][i].shape;
			compare_p.cnt++;
		}
		// 현재 돌이 compare_p의 돌과 다른 경우, 연속이 끊긴 것으로 처리합니다.
		else {
			if (compare_p.cnt > result_p.cnt) {
				result_p = compare_p;
			}
			// compare_p를 현재 돌로 초기화합니다.
			compare_p.shape = p[j][i].shape;
			compare_p.cnt = 1;
		}
	}
}

void cnt_point(Point p[COL][LOW]) { // 가로 세로 돌 개수 새기
	for (size_t i = 0; i < COL; ++i) {

		int size_w{};
		int size_b{};
		int size_w_l{};
		int size_b_l{};

		Cnt_P result_p;
		Cnt_P compare_p;
		Cnt_P result_p_l;
		Cnt_P compare_p_l;
		for (size_t j = 0; j < LOW; ++j) {

			// 라인에 돌 개수 세는 부분
			if (p[i][j].shape == "○")
				size_w++;
			else if (p[i][j].shape == "●")
				size_b++;

			if (p[j][i].shape == "○")
				size_w_l++;
			else if (p[j][i].shape == "●")
				size_b_l++;

			cout << p[i][j].shape;

			print_col_continue(i, j, result_p, compare_p, p);
			print_low_continue(i, j, result_p_l, compare_p_l, p);

		}

		cout << " | w cnt: " << result_p.cnt << " shape: " << result_p.shape;
		cout << " | h cnt: " << result_p_l.cnt << " shape: " << result_p_l.shape;
		cout << " | w" << i << " (○:" << size_w << ") (●:" << size_b << ")";
		cout << " | h" << i << " (○:" << size_w_l << ") (●: " << size_b_l << ")" << endl;
		// 다음 가로 줄을 위해 compare_p와 result_p 초기화
		compare_p.cnt = 0;
		compare_p.shape = " ";
		result_p.cnt = 0;
		result_p.shape = " ";
		compare_p_l.cnt = 0;
		compare_p_l.shape = " ";
		result_p_l.cnt = 0;
		result_p_l.shape = " ";

		size_w = 0;
		size_b = 0;
		size_w_l = 0;
		size_b_l = 0;
	}


	for (size_t i = 0; i < LOW; ++i) { // 아래 줄 긋기
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
