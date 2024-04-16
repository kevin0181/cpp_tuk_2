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
	bool status = false;
};

template <typename T>
class List {
public:
	List() :data_(nullptr), size_(0), capacity_(0) {};
	~List() {
		delete[] data_;
	}

	List(T* values, size_t size) :data_(nullptr), size_(0), capacity_(0) {
		reserve(size);
		for (int i = 0; i < size; ++i) {
			push_back(values[i]);
		}
	};

	T& operator[](size_t index) {
		return data_[index];
	}

	void push_back(T& value) {
		int find_index = insert_R(0);
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
		if (index >= size_)return index; //마지막까지 갔을때
		else if (!data_[index].status) return index; //중간에 빈 값이 존재할때
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

ORDER_E order_v(string order);
void print_p(Champion p);
void printAll_R(List<Champion>& championList, size_t index);
void search_R(List<Champion>& championList, string& name, size_t index);
void delete_R(List<Champion>& championList, string& name, size_t index);
void deleteAll_R(List<Champion>& championList, string& position, size_t index);
void findMaxHp_R(List<Champion>& championList, int& maxHp, size_t index);
void sortByHp_R(List<Champion>& championList, size_t start, size_t index);

int main() {
	Champion c_l[] = {
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

	cout << "Search [챔피언이름] : 주어진 챔피언의 정보를 출력한다." << endl;
	cout << "Insert : 새로운 챔피언의 정보를 입력받아삽입한다." << endl;
	cout << "Delete [챔피언이름] : 주어진 챔피언에 대한 정보를 삭제한다." << endl;
	cout << "DeleteAll [position] : 주어진 위치의 모든 챔피언 정보를 삭제한다." << endl;
	cout << "PrintAll : 모든 챔피언의 정보를 배열에 저장된 순서대로 출력한다." << endl;
	cout << "FindMaxHp: 가장 체력이 큰 챔피언의 정보를 출력한다." << endl;
	cout << "SortByHp: 체력이 큰 챔피언부터 순서대로 저장한다." << endl;

	List<Champion> championList(c_l, sizeof(c_l) / sizeof(c_l[0]));

	Champion c;
	string order;
	string s1;

	while (true) {
		cout << "----------------------" << endl;
		cout << "명령어를 입력하세요 : ";
		cin >> order;

		if (cin.fail()) {
			cout << "잘못된 명령어 입니다." << endl;
			cin.clear();
			break;
		}

		switch (order_v(order))
		{
		case SEARCH_E:

			cin >> c.name;

			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				c.name += " ";
				c.name += s1;
			}

			if (cin.fail()) {
				cout << "잘못된 명령어 입니다." << endl;
				cin.clear();
				break;
			}

			search_R(championList, c.name, 0);
			break;
		case INSERT_E:
			cout << "name : ";
			cin >> c.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				c.name += " ";
				c.name += s1;
			}
			cout << "hp : ";
			cin >> c.hp;
			cout << "mp : ";
			cin >> c.mp;
			cout << "speed : ";
			cin >> c.speed;
			cout << "range : ";
			cin >> c.range;
			cout << "position : ";
			cin >> c.position;

			if (cin.fail()) {
				cout << "잘못된 명령어 입니다." << endl;
				cin.clear();
				break;
			}
			championList.push_back(c);
			break;
		case DELETE_E:

			cin >> c.name;

			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				c.name += " ";
				c.name += s1;
			}
			if (cin.fail()) {
				cout << "잘못된 명령어 입니다." << endl;
				cin.clear();
				break;
			}

			delete_R(championList, c.name, 0);
			break;
		case DELETEALL_E:

			cin >> c.position;

			if (cin.fail()) {
				cout << "잘못된 명령어 입니다." << endl;
				cin.clear();
				break;
			}

			deleteAll_R(championList, c.position, 0);

			break;
		case PRINTALL_E:
			printAll_R(championList, 0);
			break;
		case FINDMAXHP_E:
		{
			int maxHp{};

			findMaxHp_R(championList, maxHp, 0);
			break;
		}
		case SORTBYHP_E:
			sortByHp_R(championList, 0, championList.size());
			cout << "정렬이 완료되었습니다." << endl;
			break;
		default:
			cout << "잘못된 명령어 입니다." << endl;
			break;
		}
		
	}

}

void sortByHp_R_step(List<Champion>& championList, size_t start, size_t index) {
	if (start >= index - 1) {
		return;
	}

	if (championList[start].hp < championList[start + 1].hp) {
		Champion c;
		c = championList[start];
		championList[start] = championList[start + 1];
		championList[start + 1] = c;
	}

	sortByHp_R_step(championList, start + 1, index);
}

void sortByHp_R(List<Champion>& championList, size_t start, size_t index) {
	if (index == 1) {
		return;
	}

	sortByHp_R_step(championList, 0, index);

	sortByHp_R(championList, start + 1, index);
}


void findMaxHp_R_step(List<Champion>& championList, int& maxHp, size_t index) {
	if (index >= championList.size())
		return;

	if (championList[index].status) {
		if (championList[index].hp > maxHp) {
			maxHp = championList[index].hp;
		}
	}

	findMaxHp_R_step(championList, maxHp, index + 1);
}

void findMaxHp_R(List<Champion>& championList, int& maxHp, size_t index) {

	if (index == 0) {
		maxHp = 0;
		findMaxHp_R_step(championList, maxHp, 0);
	}

	if (index >= championList.size())
		return;

	if (championList[index].hp == maxHp && championList[index].status) {
		print_p(championList[index]);
	}

	findMaxHp_R(championList, maxHp, index + 1);
}

void deleteAll_R(List<Champion>& championList, string& position, size_t index) {
	static bool status = false;

	if (index == 0)
		status = false;


	if (index >= championList.size()) {
		if (!status)
			cout << position << " 포지션에 해당하는 챔피언이 존재하지 않습니다." << endl;
		return;
	}


	if (championList[index].position == position && championList[index].status) {
		championList[index].status = false;
		status = true;
	}
	deleteAll_R(championList, position, index + 1);
}
void delete_R(List<Champion>& championList, string& name, size_t index) {
	static bool status = false;

	if (index == 0)
		status = false;


	if (index >= championList.size()) {
		if (!status)
			cout << name << "에 해당하는 챔피언이 존재하지 않습니다." << endl;
		return;
	}


	if (championList[index].name == name && championList[index].status) {
		championList[index].status = false;
		status = true;
	}
	delete_R(championList, name, index + 1);
}

void search_R(List<Champion>& championList, string& name, size_t index) {
	static bool status = false;

	if (index == 0)
		status = false;
	

	if (index >= championList.size()) {
		if (!status)
			cout << name << "에 해당하는 챔피언이 존재하지 않습니다." << endl;
		return;
	}


	if (championList[index].name == name && championList[index].status) {
		print_p(championList[index]);
		status = true;
	}
	search_R(championList, name, index + 1);
}

void printAll_R(List<Champion>& championList, size_t index) {
	if (index >= championList.size())
		return;

	if (championList[index].status)
		print_p(championList[index]);

	printAll_R(championList, index + 1);
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