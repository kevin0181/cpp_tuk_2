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

	//�⺻������
	List() :data_(nullptr), size_(0), capacity_(0) {}

	~List() {
		delete[] data_;
	}
	// �ʱ⿡ List ���� ��, �ʱ� �迭�� �־��ֱ� ���� ������
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
		// �� �ڸ��� ã�� ���� ��� �Լ� ȣ��
		size_t index = find_inactive_index(0);
		if (index < size_) {
			// �� �ڸ��� �����ϴ� ��� è�Ǿ��� �ش� ��ġ�� �ִ´�.
			data_[index] = new_champion;
			data_[index].status = true;
		}
		else {
			// �� �ڸ��� ���� ��� ����Ʈ�� ���� è�Ǿ��� �߰��Ѵ�.
			if (size_ >= capacity_) {
				reserve(capacity_ * 2 > 0 ? capacity_ * 2 : 1);
			}
			data_[size_++] = new_champion;
			data_[size_ - 1].status = true;
		}
	}

	//���� �����ϱ� ���� �Լ� : Delete (4)
	void drop(size_t index) {
		if (index < size_) {
			data_[index].status = false;
		}
	}

	//���� �������� ũ��
	size_t size() {
		return size_;
	}

	//���� �������� �뷮
	size_t capacity() {
		return capacity_;
	}

private:
	T* data_;
	size_t size_;
	size_t capacity_;

	// ���࿡ ���� ũ��� �뷮�� �������� �뷮�� �ø��� ���� �Լ�
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
		if (index >= size_) return index; // ��� Ż�� ����
		if (!data_[index].status) return index; // ��Ȱ��ȭ�� è�Ǿ��� ã���� ��
		return find_inactive_index(index + 1); // ���� �ε����� ��� ȣ��
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

	// �ʱⰪ 
	List<Champion> champion_list(c_l, sizeof(c_l) / sizeof(c_l[0]));


	cout << "Search [è�Ǿ��̸�] : �־��� è�Ǿ��� ������ ����Ѵ�." << endl;
	cout << "Insert : ���ο� è�Ǿ��� ������ �Է¹޾ƻ����Ѵ�." << endl;
	cout << "Delete [è�Ǿ��̸�] : �־��� è�Ǿ� ���� ������ �����Ѵ�." << endl;
	cout << "DeleteAll [position] : �־��� ��ġ�� ��� è�Ǿ� ������ �����Ѵ�." << endl;
	cout << "PrintAll : ��� è�Ǿ��� ������ �迭�� ����� ������� ����Ѵ�." << endl;
	cout << "FindMaxHp: ���� ü���� ū è�Ǿ��� ������ ����Ѵ�." << endl;
	cout << "SortByHp: ü���� ū è�Ǿ���� ������� �����Ѵ�." << endl;

	string order_s; //��ɾ�
	Champion ch_1; //è�Ǿ� ����
	string s1; //�α��� ó��
	bool status = false; //��ȿ���˻�
	int maxHp{}; // hp ū��

	while (true) {
		cout << "--------------------" << endl;
		cout << "��ɾ �Է��ϼ��� : ";
		cin >> order_s;

		if (cin.fail()) {
			cout << "�߸��� �Է°� �Դϴ�." << endl;
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
				cout << "�߸��� �Է°� �Դϴ�." << endl;
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
				cout << "�߸��� �Է°� �Դϴ�." << endl;
				cin.clear();
				break;
			}

			champion_list.push_back(ch_1);

			cout << ch_1.name << " è�Ǿ��� �����Ǿ����ϴ�." << endl;

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
				cout << "�߸��� �Է°� �Դϴ�." << endl;
				cin.clear();
				break;
			}

			delete_champion(champion_list, ch_1.name, 0);

			break;
		case DELETE_ALL_E:
			cin >> ch_1.position;

			if (cin.fail()) {
				cout << "�߸��� �Է°� �Դϴ�." << endl;
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
				cout << "hp �� ������������ ������ �Ϸ�Ǿ����ϴ�." << endl;
			}
			break;
		default:
			cout << "�߸��� ��ɾ� �Դϴ�." << endl;
			break;
		}
	}

}

// è�Ǿ��� ����ϱ� ���� �Լ�
void print_v(Champion c) {
	cout << c.name << " " << c.hp << " " << c.mp << " " << c.speed << " " << c.range << " " << c.position << endl;
}

// ��ɾ� ó�� �Լ�
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
	// ��� Ż�� ����: ��� ���Ҹ� �� �˻��߰ų� ���ϴ� è�Ǿ��� ã�� ���
	if (index >= champion_list.size()) {
		cout << "�������� �ʴ� è�Ǿ� �Դϴ�." << endl;
		return;
	}

	// ã�� è�Ǿ��� ���� �ε����� è�Ǿ�� ������ ��� ���
	if (champion_list[index].name == name && champion_list[index].status == true) {
		print_v(champion_list[index]);
		return;
	}

	// ���� è�Ǿ��� �˻��ϱ� ���� ��� ȣ��
	recursive_search(champion_list, name, index + 1);
}

// Delete
void delete_champion(List<Champion>& champion_list, const string& name, size_t index) {
	// ��� Ż�� ����: ��� ���Ҹ� �� �˻��߰ų� ���ϴ� è�Ǿ��� ã�� ���
	if (index >= champion_list.size()) {
		cout << "�������� �ʴ� è�Ǿ� �Դϴ�." << endl;
		return;
	}

	// ã�� è�Ǿ��� ���� �ε����� è�Ǿ�� ������ ��� ���
	if (champion_list[index].name == name) {
		champion_list.drop(index);
		return;
	}

	// ���� è�Ǿ��� �˻��ϱ� ���� ��� ȣ��
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
	// ��� Ż�� ����: ��� ���Ҹ� �˻���
	if (current_index == champion_list.size()) {
		// �ִ� ü���� ���� è�Ǿ� ���
		for (size_t i = 0; i < champion_list.size(); ++i) {
			if (champion_list[i].hp == max_hp) {
				print_v(champion_list[i]);
			}
		}
		return;
	}

	// ���� è�Ǿ��� ü���� ��������� �ִ� ü�º��� ū ���, �ִ밪 ����
	if (champion_list[current_index].hp > max_hp) {
		max_hp = champion_list[current_index].hp;
		max_index = current_index;
	}

	// ���� è�Ǿ��� �˻��ϱ� ���� ��� ȣ��
	find_max_hp_recursive(champion_list, current_index + 1, max_index, max_hp);
}

void recursive_bubble(List<Champion>& champion_list, size_t n) {
	// Ż�� ����: ���� ����Ʈ�� ũ�Ⱑ 1�̰ų�, ������ ���� Ž���� ���Ұ� ���ٸ�
	if (n == 1) return;

	for (size_t i = 0; i < n - 1; i++) {
		if (champion_list[i].hp < champion_list[i + 1].hp) {
			Champion temp = champion_list[i];
			champion_list[i] = champion_list[i + 1];
			champion_list[i + 1] = temp;
		}
	}

	// ��� ȣ��: ���� ������ ���� ũ�⸦ ����
	recursive_bubble(champion_list, n - 1);
}