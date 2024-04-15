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
	bool status = true;
};

template <typename T>
class List {
public:

	//기본생성자
	List() :data_(nullptr), size_(0), capacity_(0) {}

	~List() {
		delete[] data_;
	}
	// 초기에 List 생성 시, 초기 배열을 넣어주기 위한 생성자
	List(T* values, size_t size) :data_(nullptr), size_(0), capacity_(0) {
		reserve(size);
		for (size_t i = 0; i < size; ++i) {
			push_back(values[i]);
		}
	}

	T& operator[](size_t index) {
		return data_[index];
	}

	void push_back(Champion& new_champion) {
		// 빈 자리를 찾기 위한 재귀 함수 호출
		size_t index = find_inactive_index(0);
		if (index < size_) {
			// 빈 자리가 존재하는 경우 챔피언을 해당 위치에 넣는다.
			data_[index] = new_champion;
			data_[index].status = true;
		}
		else {
			// 빈 자리가 없는 경우 리스트의 끝에 챔피언을 추가한다.
			if (size_ >= capacity_) {
				reserve(capacity_ * 2 > 0 ? capacity_ * 2 : 1);
			}
			data_[size_++] = new_champion;
			data_[size_ - 1].status = true;
		}
	}

	//값을 삭제하기 위한 함수 : Delete (4)
	void drop(size_t index) {
		if (index < size_) {
			data_[index].status = false;
		}
	}

	//현재 데이터의 크기
	size_t size() {
		return size_;
	}

	//현재 데이터의 용량
	size_t capacity() {
		return capacity_;
	}

private:
	T* data_;
	size_t size_;
	size_t capacity_;

	// 만약에 현재 크기와 용량이 같아지면 용량을 늘리기 위한 함수
	void reserve(size_t new_capacity_) {
		T* new_data_ = new T[new_capacity_];
		for (size_t i = 0; i < size_; ++i) {
			new_data_[i] = data_[i];
		}

		delete[] data_;
		data_ = new_data_;
		capacity_ = new_capacity_;
	}

	size_t find_inactive_index(size_t index) {
		if (index >= size_) return index; // 재귀 탈출 조건
		if (!data_[index].status) return index; // 비활성화된 챔피언을 찾았을 때
		return find_inactive_index(index + 1); // 다음 인덱스로 재귀 호출
	}
};

enum ORDER_E {
	SEARCH_E,
	INSERT_E,
	DELETE_E,
	DELETE_ALL_E,
	PRINTALL_E,
	FINDMAXHP_E,
	SORTBYHP_E,
	NULL_E
};

ORDER_E order_v(string order_s);
void print_v(Champion c);
void recursive_search(List<Champion>& champion_list, const string& name, size_t index);
void delete_champion(List<Champion>& champion_list, const string& name, size_t index);
void printAll_champion(List<Champion>& champion_list, size_t index);
void deleteAll_position(List<Champion>& champion_list, const string& postion, size_t index);
void find_max_hp_recursive(List<Champion>& champion_list, size_t current_index, size_t& max_index, int& max_hp);
void recursive_bubble(List<Champion>& champion_list, size_t n);

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
		{"Taric",6158,7451,160,36,"bottom"},
	};

	// 초기값 
	List<Champion> champion_list(c_l, sizeof(c_l) / sizeof(c_l[0]));


	cout << "Search [챔피언이름] : 주어진 챔피언의 정보를 출력한다." << endl;
	cout << "Insert : 새로운 챔피언의 정보를 입력받아삽입한다." << endl;
	cout << "Delete [챔피언이름] : 주어진 챔피언에 대한 정보를 삭제한다." << endl;
	cout << "DeleteAll [position] : 주어진 위치의 모든 챔피언 정보를 삭제한다." << endl;
	cout << "PrintAll : 모든 챔피언의 정보를 배열에 저장된 순서대로 출력한다." << endl;
	cout << "FindMaxHp: 가장 체력이 큰 챔피언의 정보를 출력한다." << endl;
	cout << "SortByHp: 체력이 큰 챔피언부터 순서대로 저장한다." << endl;

	string order_s; //명령어
	Champion ch_1; //챔피언 정보
	string s1; //두글자 처리
	bool status = false; //유효성검사
	int maxHp{}; // hp 큰값

	while (true) {
		cout << "--------------------" << endl;
		cout << "명령어를 입력하세요 : ";
		cin >> order_s;

		if (cin.fail()) {
			cout << "잘못된 입력값 입니다." << endl;
			cin.clear();
			break;
		}

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

			if (cin.fail()) {
				cout << "잘못된 입력값 입니다." << endl;
				cin.clear();
				break;
			}

			recursive_search(champion_list, ch_1.name, 0);

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

			if (cin.fail()) {
				cout << "잘못된 입력값 입니다." << endl;
				cin.clear();
				break;
			}

			champion_list.push_back(ch_1);

			cout << ch_1.name << " 챔피언이 생성되었습니다." << endl;

			break;
		case DELETE_E:

			cin >> ch_1.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				ch_1.name += " ";
				ch_1.name += s1;
			}

			if (cin.fail()) {
				cout << "잘못된 입력값 입니다." << endl;
				cin.clear();
				break;
			}

			delete_champion(champion_list, ch_1.name, 0);

			break;
		case DELETE_ALL_E:
			cin >> ch_1.position;

			if (cin.fail()) {
				cout << "잘못된 입력값 입니다." << endl;
				cin.clear();
				break;
			}

			deleteAll_position(champion_list, ch_1.position, 0);
			
			break;
		case PRINTALL_E: 
			printAll_champion(champion_list, 0);
			break;
		case FINDMAXHP_E:
		{
			size_t max_index{};
			int max_hp = champion_list[0].hp;
			find_max_hp_recursive(champion_list, 0, max_index, max_hp);
			break;
		}
		case SORTBYHP_E:
			if (champion_list.size() > 0) {
				recursive_bubble(champion_list, champion_list.size());
				cout << "hp 의 내림차순으로 정렬이 완료되었습니다." << endl;
			}
			break;
		default:
			cout << "잘못된 명령어 입니다." << endl;
			break;
		}
	}

}

// 챔피언을 출력하기 위한 함수
void print_v(Champion c) {
	cout << c.name << " " << c.hp << " " << c.mp << " " << c.speed << " " << c.range << " " << c.position << endl;
}

// 명령어 처리 함수
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
		return PRINTALL_E;
	if (order_s == "FindMaxHp")
		return FINDMAXHP_E;
	if (order_s == "SortByHp")
		return SORTBYHP_E;

	return NULL_E;
}

// Search
void recursive_search(List<Champion>& champion_list, const string& name, size_t index) {
	// 재귀 탈출 조건: 모든 원소를 다 검색했거나 원하는 챔피언을 찾은 경우
	if (index >= champion_list.size()) {
		cout << "존재하지 않는 챔피언 입니다." << endl;
		return;
	}

	// 찾는 챔피언이 현재 인덱스의 챔피언과 동일한 경우 출력
	if (champion_list[index].name == name && champion_list[index].status == true) {
		print_v(champion_list[index]);
		return;
	}

	// 다음 챔피언을 검색하기 위해 재귀 호출
	recursive_search(champion_list, name, index + 1);
}

// Delete
void delete_champion(List<Champion>& champion_list, const string& name, size_t index) {
	// 재귀 탈출 조건: 모든 원소를 다 검색했거나 원하는 챔피언을 찾은 경우
	if (index >= champion_list.size()) {
		cout << "존재하지 않는 챔피언 입니다." << endl;
		return;
	}

	// 찾는 챔피언이 현재 인덱스의 챔피언과 동일한 경우 출력
	if (champion_list[index].name == name) {
		champion_list.drop(index);
		return;
	}

	// 다음 챔피언을 검색하기 위해 재귀 호출
	delete_champion(champion_list, name, index + 1);
}

//PrintAll
void printAll_champion(List<Champion>& champion_list, size_t index) {

	if (index >= champion_list.size()) {
		return;
	}

	if (champion_list[index].status == true) {
		print_v(champion_list[index]);
	}

	printAll_champion(champion_list, index + 1);
}


//deleteAll
void deleteAll_position(List<Champion>& champion_list,const string& postion ,size_t index) {

	if (index >= champion_list.size()) {
		return;
	}

	if (champion_list[index].status == true && champion_list[index].position == postion) {
		champion_list.drop(index);
	}

	deleteAll_position(champion_list, postion, index + 1);
}

//findMax
void find_max_hp_recursive(List<Champion>& champion_list, size_t current_index, size_t& max_index, int& max_hp) {
	// 재귀 탈출 조건: 모든 원소를 검사함
	if (current_index == champion_list.size()) {
		// 최대 체력을 가진 챔피언 출력
		for (size_t i = 0; i < champion_list.size(); ++i) {
			if (champion_list[i].hp == max_hp) {
				print_v(champion_list[i]);
			}
		}
		return;
	}

	// 현재 챔피언의 체력이 현재까지의 최대 체력보다 큰 경우, 최대값 갱신
	if (champion_list[current_index].hp > max_hp) {
		max_hp = champion_list[current_index].hp;
		max_index = current_index;
	}

	// 다음 챔피언을 검사하기 위해 재귀 호출
	find_max_hp_recursive(champion_list, current_index + 1, max_index, max_hp);
}

void recursive_bubble(List<Champion>& champion_list, size_t n) {
	// 탈출 조건: 만약 리스트의 크기가 1이거나, 버블을 위해 탐색할 원소가 없다면
	if (n == 1) return;

	for (size_t i = 0; i < n - 1; i++) {
		if (champion_list[i].hp < champion_list[i + 1].hp) {
			Champion temp = champion_list[i];
			champion_list[i] = champion_list[i + 1];
			champion_list[i + 1] = temp;
		}
	}

	// 재귀 호출: 다음 버블을 위해 크기를 줄임
	recursive_bubble(champion_list, n - 1);
}