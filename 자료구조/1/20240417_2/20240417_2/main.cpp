#include <iostream>
#include <string>

using namespace std;

struct Champion {
	string name;
	int hp;
	int mp;
	int speed;
	int range;
	string position;
	bool status = false; //상태값
};

template <typename T>
class List {
public:
	List() :data_(nullptr), size_(0), capacity_(0) {};
	~List() {
		delete[] data_;
	}

	List(T* values, size_t size) : data_(nullptr), size_(0), capacity_(0) {
		reserve(size);
		for (int i = 0; i < size; ++i) {
			push_back(values[i]);
		}
	};

	T& operator[](size_t index) {
		return data_[index];
	}

	void push_back(T& value) {
		size_t find_index = insert_R(0);
		if (find_index < size_) {
			data_[find_index] = value;
			data_[find_index].status = true;
		}
		else {
			if (size_ >= capacity_) {
				reserve(capacity_ * 2 > 0 ? capacity_ * 2 : 1);
			}
			data_[size_++] = value;
			data_[size_ - 1].status = true;
		}
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
		for (int i = 0; i < size_; ++i) {
			new_data_[i] = data_[i];
		}

		delete[] data_;
		data_ = new_data_;
		capacity_ = new_capacity_;
	}

	size_t insert_R(size_t index) {
		if (index >= size_) return index;
		else if (!data_[index].status) return index;
		return insert_R(index + 1);
	}
};

enum ORDER_E {
	SEARCH_E,
	INSERT_E,
	DELETE_E,
	DELETEALL_E,
	PRINTALL_E,
	FINDMAXHP_E,
	SORTBYHP_E,
	NULL_E
};

// --
ORDER_E order_v(string order);
void print_p(Champion p);
void printAll_R(List<Champion>& c_list, size_t index);
void search_R(List<Champion>& c_list, string& name, size_t index);
void delete_R(List<Champion>& c_list, string& name, size_t index);
void deleteAll_R(List<Champion>& c_list, string& position, size_t index);
void findMaxHp_R(List<Champion>& c_list, int& maxHp, size_t index);
void findMaxHp_R_step(List<Champion>& c_list, int& maxHp, size_t index);
void sortByHp_R(List<Champion>& c_list, size_t last_index);
void sortByHp_R_step(List<Champion>& c_list, size_t start, size_t last_index);

int main() {
	Champion c[] = {
		{"Jayce",3466,8307,452,73,"mid"},
		{"Vayne",3318,2790,390,60,"support"},
		{"Lee Sin",9864,6257,747,19,"support"},
		{"Teemo",1915,1313,	700	,68,"mid"},
		{"Nami",2968,2153,803,74,"support"},
		{"Jinx",2583,3042,123,53,"jungle"},
		{"Cassiopeia",6067,7330,683,24,"top"},
		{"Ahri",4573,7079,177,79,"top"},
		{"Fiora",2591,9329,337,81,"jungle"},
		{"Zac",9864,7272,405,49,"bottom"},
		{"Janna",4922,2275,690,66,"jungle"},
		{"Ashe"	,7421,5549,428,31,"top"},
		{"Twitch",2980,7703,204,91,"mid"},
		{"Ezreal",3660,9835,274,44,"mid"},
		{"Sona"	,5655,8442,252,26,"bottom"},
		{"Lulu"	,3231,2014,503,32,"support"},
		{"Amumu",6763,3994,427,19,"jungle"},
		{"Master Yi",5737,7168,434,61,"top"},
		{"Jarvan",	1895,3070,323,80,"mid"},
		{"Shen"	,4794,9220,783,42,"mid"},
		{"Fizz"	,9864,1913,433,38,"bottom"},
		{"Taric",6158,7451,160,36,"bottom"}
	};

	List<Champion> c_list(c, sizeof(c) / sizeof(c[0]));

	Champion champion;
	string order;
	string s1;

	cout << "Search [챔피언이름] : 주어진 챔피언의 정보를 출력한다." << endl;
	cout << "Insert : 새로운 챔피언의 정보를 입력받아삽입한다." << endl;
	cout << "Delete [챔피언이름] : 주어진 챔피언에 대한 정보를 삭제한다." << endl;
	cout << "DeleteAll [position] : 주어진 위치의 모든 챔피언 정보를 삭제한다." << endl;
	cout << "PrintAll : 모든 챔피언의 정보를 배열에 저장된 순서대로 출력한다." << endl;
	cout << "FindMaxHp: 가장 체력이 큰 챔피언의 정보를 출력한다." << endl;
	cout << "SortByHp: 체력이 큰 챔피언부터 순서대로 저장한다." << endl;


	while (true) {
		cout << "-------------------" << endl;
		cout << "order : ";
		cin >> order;
		if (cin.fail()) {
			cout << "잘못된 명령어 입니다." << endl;
			cin.clear();
			break;
		}

		switch (order_v(order))
		{
		case SEARCH_E:
			cin >> champion.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				champion.name += " ";
				champion.name += s1;
			}
			if (cin.fail()) {
				cout << "잘못된 명령어 입니다." << endl;
				cin.clear();
				break;
			}

			search_R(c_list, champion.name, 0);
			break;
		case INSERT_E:
			cout << "name : ";
			cin >> champion.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				champion.name += " ";
				champion.name += s1;
			}
			cout << "hp : ";
			cin >> champion.hp;
			cout << "mp : ";
			cin >> champion.mp;
			cout << "speed : ";
			cin >> champion.speed;
			cout << "range : ";
			cin >> champion.range;
			cout << "position : ";
			cin >> champion.position;

			if (cin.fail()) {
				cout << "잘못된 명령어 입니다." << endl;
				cin.clear();
				break;
			}

			c_list.push_back(champion);

			break;
		case DELETE_E:
			cin >> champion.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				champion.name += " ";
				champion.name += s1;
			}
			if (cin.fail()) {
				cout << "잘못된 명령어 입니다." << endl;
				cin.clear();
				break;
			}
			delete_R(c_list, champion.name, 0);
			break;
		case DELETEALL_E:
			cin >> champion.position;
			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				champion.position += " ";
				champion.position += s1;
			}
			if (cin.fail()) {
				cout << "잘못된 명령어 입니다." << endl;
				cin.clear();
				break;
			}
			deleteAll_R(c_list, champion.position, 0);
			break;
		case PRINTALL_E:
			printAll_R(c_list, 0);
			break;
		case FINDMAXHP_E:
		{
			int maxHp{};
			findMaxHp_R(c_list, maxHp, 0);
			break;
		}
		case SORTBYHP_E:
			sortByHp_R(c_list, c_list.size());
			cout << "내림차순 정렬 되었습니다." << endl;
			break;
		default:
			cout << "잘못된 명령어 입니다." << endl;
			break;
		}

	}

}

void sortByHp_R_step(List<Champion>& c_list, size_t start, size_t last_index) {
	if (start >= last_index - 1)
		return;
	if (c_list[start].hp < c_list[start + 1].hp) {
		Champion p;
		p = c_list[start];
		c_list[start] = c_list[start + 1];
		c_list[start + 1] = p;
	}

	sortByHp_R_step(c_list, start + 1, last_index);
}

void sortByHp_R(List<Champion>& c_list, size_t last_index) {
	if (last_index == 1)
		return;
	sortByHp_R_step(c_list, 0, last_index);
	sortByHp_R(c_list, last_index - 1);
}

void findMaxHp_R_step(List<Champion>& c_list, int& maxHp, size_t index) {
	if (index >= c_list.size())
		return;

	if (c_list[index].status) {
		if (c_list[index].hp > maxHp) {
			maxHp = c_list[index].hp;
		}
	}
	findMaxHp_R_step(c_list, maxHp, index + 1);
}

void findMaxHp_R(List<Champion>& c_list,int& maxHp, size_t index) {

	if (index == 0) {
		maxHp = 0;
		findMaxHp_R_step(c_list, maxHp, 0);
	}

	if (index >= c_list.size()) {
		return;
	}

	if (c_list[index].status && c_list[index].hp == maxHp) {
		print_p(c_list[index]);
	}

	findMaxHp_R(c_list, maxHp, index + 1);
}

void deleteAll_R(List<Champion>& c_list, string& position, size_t index) {

	static bool status = false;

	if (index == 0)
		status = false;

	if (index >= c_list.size()) {
		if (!status)
			cout << "존재하지 않는 포지션 입니다." << endl;

		return;
	}

	if (c_list[index].status && c_list[index].position == position) {
		c_list[index].status = false;
		status = true;
	}
	deleteAll_R(c_list, position, index + 1);

}
void delete_R(List<Champion>& c_list, string& name, size_t index) {
	static bool status = false;

	if (index == 0)
		status = false;

	if (index >= c_list.size()) {
		if (!status)
			cout << "존재하지 않는 챔피언 입니다." << endl;

		return;
	}

	if (c_list[index].status && c_list[index].name == name) {
		c_list[index].status = false;
		status = true;
	}
	delete_R(c_list, name, index + 1);
}

void search_R(List<Champion>& c_list, string& name, size_t index) {

	static bool status = false;

	if (index == 0)
		status = false;

	if (index >= c_list.size()) {
		if (!status)
			cout << "존재하지 않는 챔피언 입니다." << endl;
		
		return;
	}

	if (c_list[index].status && c_list[index].name == name) {
		print_p(c_list[index]);
		status = true;
	}
	search_R(c_list, name, index + 1);
}

void printAll_R(List<Champion>& c_list, size_t index) {
	if (index >= c_list.size())
		return;

	if (c_list[index].status)
		print_p(c_list[index]);

	printAll_R(c_list, index + 1);
}

void print_p(Champion p) {
	cout << p.name << " " << p.hp << " " << p.mp << " " << p.speed << " " << p.range << " " << p.position << endl;
}

ORDER_E order_v(string order) {
	if (order == "Search")
		return SEARCH_E;
	if (order == "Insert")
		return INSERT_E;
	if (order == "Delete")
		return DELETE_E;
	if (order == "DeleteAll")
		return DELETEALL_E;
	if (order == "PrintAll")
		return PRINTALL_E;
	if (order == "FindMaxHp")
		return FINDMAXHP_E;
	if (order == "SortByHp")
		return SORTBYHP_E;
	return NULL_E;
}