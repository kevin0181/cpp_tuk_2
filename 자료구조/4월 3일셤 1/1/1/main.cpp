#include <iostream>
#include <string>

using namespace std;

template <typename T>
class List {
public:
	List() :data_(nullptr), size_(0), capacity_(0) {}
	~List() {
		delete[] data_;
	}

	List(T* values, size_t size) : data_(nullptr), size_(0), capacity_(0) {
		reserve(size);
		for (size_t i = 0; i < size; ++i) {
			push_back(values[i]);
		}
	}

	T& operator [](size_t index) {
		return data_[index];
	}

	void push_back(T& value) {
		if (size_ >= capacity_) {
			reserve(capacity_ * 2 > 0 ? capacity_ * 2 : 1);
		}
		data_[size_++] = value;
	}

	void drop(size_t index) {
		if (index < size_) {
			for (size_t i = index; i < size_ - 1; ++i) {
				data_[i] = data_[i + 1];
			}
		}
		size_ --;
	}

	size_t size() {
		return size_;
	}

	size_t capacity() {
		return capacity_;
	}

private:
	T* data_;
	size_t size_;
	size_t capacity_;
	void reserve(size_t new_capacity_) {
		T* new_data_ = new T[new_capacity_];

		for (size_t i = 0; i < size_; ++i) {
			new_data_[i] = data_[i];
		}

		delete[] data_;

		data_ = new_data_;
		capacity_ = new_capacity_;
	}
};

struct Champion
{
	string name;
	int hp;
	int mp;
	int speed;
	int range;
	string position;
};

enum ORDER_E {
	SEARCH_E,
	INSERT_E,
	DELETE_E,
	DELETE_ALL_E,
	PRINT_ALL_E,
	FINDMAXHP_E,
	SORTBYHP_E,
	NULL_E
};

ORDER_E order_v(string order_s);
void print_v(Champion c);

int main() {
	Champion c_l[] = {
		{"Jayce", 3466, 8307, 452, 73, "mid"},
		{"Vayne", 3318, 2790, 390, 60, "support"},
		{"Lee Sin", 9864, 6257, 747, 19, "support"},
		{"Teemo", 1915, 1313, 700, 68, "mid"},
		{"Nami", 2968, 2153, 803, 74, "support"},
		{"Jinx", 2583, 3042, 123, 53, "jungle"},
		{"Cassiopeia", 6067, 7330, 683, 24, "top"},
		{"Ahri", 4573, 7079, 177, 79, "top"},
		{"Fiora", 2591, 9329, 337, 81, "jungle"},
		{"Zac", 9864, 7272, 405, 49, "bottom"},
		{"Janna", 4922, 2275, 690, 66, "jungle"},
		{"Ashe", 7421, 5549, 428, 31, "top"},
		{"Twitch", 2980, 7703, 204, 91, "mid"},
		{"Ezreal", 3660, 9835, 274, 44, "mid"},
		{"Sona", 5655, 8442, 252, 26, "bottom"},
		{"Lulu", 3231, 2014, 503, 32, "support"},
		{"Amumu", 6763, 3994, 427, 19, "jungle"},
		{"Master Yi", 5737, 7168, 434, 61, "top"},
		{"Jarvan", 1895, 3070, 323, 80, "mid"},
		{"Shen", 4794, 9220, 783, 42, "mid"},
		{"Fizz", 9864, 1913, 433, 38, "bottom"},
		{"Taric", 6158, 7451, 160, 36, "bottom"} };

	List<Champion> champion_list(c_l, sizeof(c_l) / sizeof(c_l[0]));

	cout << "Search [챔피언이름] : 주어진 챔피언의 정보를 출력한다." << endl;
	cout << "Insert : 새로운 챔피언의 정보를 입력받아삽입한다." << endl;
	cout << "Delete [챔피언이름] : 주어진 챔피언에 대한 정보를 삭제한다." << endl;
	cout << "DeleteAll [position] : 주어진 위치의 모든 챔피언 정보를 삭제한다." << endl;
	cout << "PrintAll : 모든 챔피언의 정보를 배열에 저장된 순서대로 출력한다." << endl;
	cout << "FindMaxHp: 가장 체력이 큰 챔피언의 정보를 출력한다." << endl;
	cout << "SortByHp: 체력이 큰 챔피언부터 순서대로 저장한다." << endl;

	string order_s;
	string s1;
	Champion ch_1;
	bool status = false;
	int maxHp{};
	while (true) {
		cout << "------------------" << endl;
		cout << "명령어를 입력하세요 : ";
		cin >> order_s;
		switch (order_v(order_s))
		{
		case SEARCH_E:

			cin >> ch_1.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				ch_1.name += " ";
				ch_1.name += s1;
			}

			for (size_t i = 0; i < champion_list.size(); ++i) {
				if (champion_list[i].name == ch_1.name) {
					print_v(champion_list[i]);
					status = true;
				}
			}

			if (!status) {
				cout << "존재하지 않는 챔피언 입니다." << endl;
			}

			status = false;
			break;
		case INSERT_E:
			cout << "name : ";
			cin >> ch_1.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				ch_1.name += " ";
				ch_1.name += s1;
			}
			cout << "hp : ";
			cin >> ch_1.hp;
			cout << "mp : ";
			cin >> ch_1.mp;
			cout << "speed : ";
			cin >> ch_1.speed;
			cout << "range : ";
			cin >> ch_1.range;
			cout << "position : ";
			cin >> ch_1.position;
			
			champion_list.push_back(ch_1);

			break;
		case DELETE_E:
			cin >> ch_1.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				ch_1.name += " ";
				ch_1.name += s1;
			}

			for (size_t i = 0; i < champion_list.size(); ++i) {
				if (champion_list[i].name == ch_1.name) {
					champion_list.drop(i);
					i = 0;
					status = true;
				}
			}

			if (!status) {
				cout << "존재하지 않는 챔피언 입니다." << endl;
			}

			status = false;
			break;
		case DELETE_ALL_E:
			cin >> ch_1.position;

			for (size_t i = 0; i < champion_list.size(); ++i) {
				if (champion_list[i].position == ch_1.position) {
					champion_list.drop(i);
					i = 0;
					status = true;
				}
			}

			if (!status) {
				cout << "존재하지 않는 포지션 입니다." << endl;
			}

			status = false;
			break;
		case PRINT_ALL_E:
			for (size_t i = 0; i < champion_list.size(); ++i) {
				print_v(champion_list[i]);
			}
			break;
		case FINDMAXHP_E:
			for (size_t i = 0; i < champion_list.size(); ++i) {
				if (maxHp < champion_list[i].hp) {
					maxHp = champion_list[i].hp;
				}
			}

			for (size_t i = 0; i < champion_list.size(); ++i) {
				if (maxHp == champion_list[i].hp) {
					print_v(champion_list[i]);
				}
			}
			break;
		case SORTBYHP_E:
			for (size_t i = 0; i < champion_list.size(); ++i) {
				for (size_t j = 0; j < champion_list.size(); ++j) {
					if (champion_list[i].hp > champion_list[j].hp) {
						ch_1 = champion_list[i];
						champion_list[i] = champion_list[j];
						champion_list[j] = ch_1;
					}
				}
			}

			break;
		default:
			cout << "잘못된 명령어 입니다." << endl;
			break;
		}
	}

}

void print_v(Champion c) {
	cout << c.name << " " << c.hp << " " << c.mp << " " << c.speed << " " << c.range << " " << c.position << endl;
}

ORDER_E order_v(string order_s) {
	if (order_s == "Search")
		return SEARCH_E;
	if (order_s == "Insert")
		return INSERT_E;
	if (order_s == "Delete")
		return DELETE_E;
	if (order_s == "DeleteAll")
		return DELETE_ALL_E;
	if (order_s == "PrintAll")
		return PRINT_ALL_E;
	if (order_s == "FindMaxHp")
		return FINDMAXHP_E;
	if (order_s == "SortByHp")
		return SORTBYHP_E;
	return NULL_E;
}