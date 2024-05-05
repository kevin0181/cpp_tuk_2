#include <iostream>
#include <string>

using namespace std;

struct Champion
{
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
	List() :data_(nullptr), size_(0), capacity_(0) {};

	~List() {
		delete[] data_;
	};

	List(T* values, size_t size) :data_(nullptr), size_(0), capacity_(0) {
		reserve(size);
		for (int i = 0; i < size; ++i) {
			push_back(values[i]);
		}
	}

	T& operator[](size_t index) {
		return data_[index];
	}

	void push_back(Champion& champion) {
		size_t find_index = insert_R(0); //insert ����Լ� ���
		if (find_index < size_) {
			data_[find_index] = champion;
			data_[find_index].status = true;
		}
		else {
			if (size_ >= capacity_) {
				reserve(capacity_ * 2 > 0 ? capacity_ * 2 : 1);
			}

			data_[size_++] = champion;
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
		for (size_t i = 0; i < size_; ++i) {
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

enum order_E {
	SEARCH_E,
	INSERT_E,
	DELETE_E,
	DELETEALL_E,
	PRINTALL_E,
	FINDMAXHP_E,
	SORTBYHP_E,
	NULL_E
};

order_E order_V(string order_s);
void print_champion(Champion p);
void printAll_R(List<Champion>& champion_list, size_t index);
void search_R(List<Champion>& champion_list, string& name, size_t index);
void delete_R(List<Champion>& champion_list, string& name, size_t index);
void deleteAll_R(List<Champion>& champion_list, string& position, size_t index);
void findMaxHp_R(List<Champion>& champion_list, int& maxHp, size_t index);
int maxHp_v(List<Champion>& champion_list, int& maxHp, size_t index);
void sortByHp_R(List<Champion>& champion_list, size_t last_index);
void sortByHp_step(List<Champion>& champion_list, size_t start, size_t last_index);

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

	List<Champion> champion_list(c_l, sizeof(c_l) / sizeof(c_l[0]));

	


	cout << "Search [è�Ǿ��̸�] : �־��� è�Ǿ��� ������ ����Ѵ�." << endl;
	cout << "Insert : ���ο� è�Ǿ��� ������ �Է¹޾ƻ����Ѵ�." << endl;
	cout << "Delete [è�Ǿ��̸�] : �־��� è�Ǿ� ���� ������ �����Ѵ�." << endl;
	cout << "DeleteAll [position] : �־��� ��ġ�� ��� è�Ǿ� ������ �����Ѵ�." << endl;
	cout << "PrintAll : ��� è�Ǿ��� ������ �迭�� ����� ������� ����Ѵ�." << endl;
	cout << "FindMaxHp: ���� ü���� ū è�Ǿ��� ������ ����Ѵ�." << endl;
	cout << "SortByHp: ü���� ū è�Ǿ���� ������� �����Ѵ�." << endl;

	Champion c;
	string order_s;
	string order_s2;

	while (true) {

		cout << "-----------------------" << endl;
		cout << "��ɾ �Է��ϼ��� : ";
		cin >> order_s;

		if (cin.fail()) {
			cout << "�߸��� ��ɾ� �Դϴ�." << endl;
			cin.clear();
			break;
		}

		switch (order_V(order_s))
		{
		case SEARCH_E:
			cin >> c.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> order_s2;
				c.name += " ";
				c.name += order_s2;
			}

			if (cin.fail()) {
				cout << "�߸��� �Է°� �Դϴ�." << endl;
				cin.clear();
				break;
			}

			search_R(champion_list, c.name, 0);

			break;
		case INSERT_E:
			cout << "name : ";
			cin >> c.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> order_s2;
				c.name += " ";
				c.name += order_s2;
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
				cout << "�߸��� �Է°� �Դϴ�." << endl;
				cin.clear();
				break;
			}
			
			champion_list.push_back(c);

			cout << c.name << " è�Ǿ��� ���� �Ǿ����ϴ�." << endl;

			break;
		case DELETE_E:
			cin >> c.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> order_s2;
				c.name += " ";
				c.name += order_s2;
			}
			if (cin.fail()) {
				cout << "�߸��� �Է°� �Դϴ�." << endl;
				cin.clear();
				break;
			}
			delete_R(champion_list, c.name, 0);
			break;
		case DELETEALL_E:
			cin >> c.position;
			if (cin.fail()) {
				cout << "�߸��� �Է°� �Դϴ�." << endl;
				cin.clear();
				break;
			}
			deleteAll_R(champion_list, c.position, 0);
			break;
		case PRINTALL_E:
			printAll_R(champion_list, 0);
			break;
		case FINDMAXHP_E:
		{
			int maxHp{ 0 };
			findMaxHp_R(champion_list, maxHp, 0);
			break;
		}
		case SORTBYHP_E:
			sortByHp_R(champion_list, champion_list.size());
			cout << "������ �Ϸ��߽��ϴ�." << endl;
			break;
		default:
			cout << "�߸��� ��ɾ� �Դϴ�." << endl;
			break;
		}

	}
}

void sortByHp_R(List<Champion>& champion_list, size_t last_index) {
	if (last_index == 1)
		return;
	sortByHp_step(champion_list, 0, last_index);
	sortByHp_R(champion_list, last_index - 1);
}

void sortByHp_step(List<Champion>& champion_list, size_t start, size_t last_index) {
	if (start >= last_index - 1)
		return;

	if (champion_list[start].hp < champion_list[start + 1].hp) {
		Champion p;
		p = champion_list[start];
		champion_list[start] = champion_list[start + 1];
		champion_list[start + 1] = p;
	}

	sortByHp_step(champion_list, start + 1, last_index);

}
void findMaxHp_R(List<Champion>& champion_list, int& maxHp, size_t index) {

	if (index == 0) {
		maxHp = 0;
		maxHp_v(champion_list, maxHp, 0);
	}
	
	if (index >= champion_list.size()) {
		return;
	}

	if (champion_list[index].hp == maxHp && champion_list[index].status) {
		print_champion(champion_list[index]);
	}

	findMaxHp_R(champion_list, maxHp, index + 1);
}

int maxHp_v(List<Champion>& champion_list, int& maxHp, size_t index) {
	if (index >= champion_list.size())
		return maxHp;
	
	// è�Ǿ��� status�� true�� ��쿡�� �ִ� hp�� ��
	if (champion_list[index].status) {
		if (champion_list[index].hp > maxHp) {
			maxHp = champion_list[index].hp; // �ִ� hp ������Ʈ
		}
	}

	// ��� ȣ���� ���� �ε�����
	return maxHp_v(champion_list, maxHp, index + 1);
}

void deleteAll_R(List<Champion>& champion_list, string& position, size_t index) {
	static int status = false;

	if (index == 0)
		status = false;

	if (index >= champion_list.size()) {
		if (!status)
			cout << "�������� �ʴ� ������ �Դϴ�." << endl;
		return;
	}

	if (champion_list[index].position == position && champion_list[index].status) {
		champion_list[index].status = false;
		status = true;
	}

	deleteAll_R(champion_list, position, index + 1);
}

void delete_R(List<Champion>& champion_list, string& name, size_t index) {
	static int status = false;

	if (index == 0)
		status = false;

	if (index >= champion_list.size()) {
		if (!status)
			cout << "�������� �ʴ� è�Ǿ� �Դϴ�." << endl;
		return;
	}

	if (champion_list[index].name == name && champion_list[index].status) {
		champion_list[index].status = false;
		status = true;
	}

	delete_R(champion_list, name, index + 1);
}

void search_R(List<Champion>& champion_list, string& name, size_t index) {
	static int status = false;

	if (index == 0)
		status = false;

	if (index >= champion_list.size()) {
		if (!status)
			cout << "�������� �ʴ� è�Ǿ� �Դϴ�." << endl;
		return;
	}

	if (champion_list[index].name == name && champion_list[index].status) {
		print_champion(champion_list[index]);
		status = true;
	}

	search_R(champion_list, name, index + 1);
}

void printAll_R(List<Champion>& champion_list, size_t index) {

	if (index >= champion_list.size()) {
		return;
	}

	if (champion_list[index].status) {
		print_champion(champion_list[index]);
	}

	printAll_R(champion_list, index + 1);
}

void print_champion(Champion p) {
	cout << p.name << " " << p.hp << " " << p.mp << " " << p.speed << " " << p.range << " " << p.position << endl;
}

order_E order_V(string order_s) {
	if (order_s == "Search")
		return SEARCH_E;
	if (order_s == "Insert")
		return INSERT_E;
	if (order_s == "Delete")
		return DELETE_E;
	if (order_s == "DeleteAll")
		return DELETEALL_E;
	if (order_s == "PrintAll")
		return PRINTALL_E;
	if (order_s == "FindMaxHp")
		return FINDMAXHP_E;
	if (order_s == "SortByHp")
		return SORTBYHP_E;
	return NULL_E;
}