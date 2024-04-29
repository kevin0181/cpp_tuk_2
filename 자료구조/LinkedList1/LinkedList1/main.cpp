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
};

// ���� ȯ�� ���� ����Ʈ�� ��带 �����ϴ� ����ü
struct Node {
	Champion data; // è�Ǿ� ������
	Node* next; // ���� ��带 ����Ű�� ������

	Node(Champion c) : data(c), next(nullptr) {} // ������
};

// ���� ȯ�� ���� ����Ʈ Ŭ����
class SingleCircularLinkedList {
private:
	Node* head; // ����Ʈ�� ������ ����Ű�� ��� ������

public:
	SingleCircularLinkedList() : head(nullptr) {} // ������

	// �Ҹ���
	~SingleCircularLinkedList() {
		if (head == nullptr) return;

		Node* current = head->next;
		head->next = nullptr; // ��ȯ ���� ����
		while (current != nullptr) {
			Node* next = current->next;
			delete current;
			current = next;
		}
	}

	void add(Champion p) {
		Node* newNode = new Node(p);
		if (head == nullptr) {
			newNode->next = newNode; // �� ��尡 �ڽ��� ����Ű���� ����
			head = newNode; // �� ��带 ���� ����
		}
		else {
			Node* current = head;
			while (current->next != head) { // ������ ��� ã��
				current = current->next;
			}
			current->next = newNode; // ������ ��尡 �� ��带 ����Ű���� ����
			newNode->next = head; // �� ��尡 ù ��° ��带 ����Ű���� ����
		}
	}

	void printAll() {
		if (head == nullptr) return;
		
		Node* current = head->next;
		do {
			printChampion(current->data);
			current = current->next;
		} while (current != head->next); // ���� ���� ���ƿ� ������ �ݺ�
	}

	void delete_L(string name) {
		if (head == nullptr) return;

		Node* current = head->next;
		Node* delete_data = nullptr;
		do {
			if (current->data.name == name) {
				
			}
		} while (current != head->next);

	}

private:
	static void printChampion(const Champion& c) {
		cout << c.name << " " << c.hp << " " << c.mp << " " << c.speed << " " << c.range << " " << c.position << endl;
	}

	static void swap(Champion& a, Champion& b) {
		Champion temp = a;
		a = b;
		b = temp;
	}
};

// �迭�� �����͸� ���� ȯ�� ���� ����Ʈ�� ��ȯ�ϴ� �Լ�
void Array2LinkedList(Champion* array, size_t size, SingleCircularLinkedList& list) {
	for (size_t i = 0; i < size; ++i) {
		list.add(array[i]);
	}
}

enum ORDER {
	SEARCH_O,
	INSERT_O,
	DELETE_O,
	DELETE_ALL_O,
	PRINT_ALL_O,
	FINDMAXHP_O,
	SORTBYHP_O,
	NULL_O
};

ORDER order_v(string order_s);

int main() {

	Champion c_l[] = {
		{"Jayce",3466,8307,452,73,"mid"},
		{"Vayne",3318,2790,390,60,"support"},
		{"Lee Sin",9864,6257,747,19,"support"},
		{"Teemo",1915,1313,700,68,"mid"},
		{"Nami",2968,2153,803,74,"support"},
		{"Jinx",2583,3042,123,53,"jungle"},
		{"Cassiopeia",6067,7330,683,24,"top"},
		{"Ahri",4573,7079,177,79,"top"},
		{"Fiora",2591,9329,337,81,"jungle"},
		{"Zac",9864,7272,405,49,"bottom"},
		{"Janna",4922,2275,690,66,"jungle"},
		{"Ashe",7421,5549,428,31,"top"},
		{"Twitch",2980,7703,204,91,"mid"},
		{"Ezreal",3660,9835,274,44,"mid"},
		{"Sona",5655,8442,252,26,"bottom"},
		{"Lulu",3231,2014,503,32,"support"},
		{"Amumu",6763,3994,427,19,"jungle"},
		{"Master Yi",5737,7168,434,61,"top"},
		{"Jarvan",1895,3070,323,80,"mid"},
		{"Shen",4794,9220,783,42,"mid"},
		{"Fizz",9864,1913,433,38,"bottom"},
		{"Taric",6158,7451,160,36,"bottom"},
	};

	SingleCircularLinkedList list;
	Array2LinkedList(c_l, sizeof(c_l) / sizeof(c_l[0]), list);

	cout << "Search [è�Ǿ��̸�] : �־��� è�Ǿ��� ������ ����Ѵ�." << endl;
	cout << "Insert : ���ο� è�Ǿ��� ������ �Է¹޾ƻ����Ѵ�." << endl;
	cout << "Delete [è�Ǿ��̸�] : �־��� è�Ǿ� ���� ������ �����Ѵ�." << endl;
	cout << "DeleteAll [position] : �־��� ��ġ�� ��� è�Ǿ� ������ �����Ѵ�." << endl;
	cout << "PrintAll : ��� è�Ǿ��� ������ �迭�� ����� ������� ����Ѵ�." << endl;
	cout << "FindMaxHp: ���� ü���� ū è�Ǿ��� ������ ����Ѵ�." << endl;
	cout << "SortByHp: ü���� ū è�Ǿ���� ������� �����Ѵ�." << endl;

	string order_s;
	string s1;
	Champion c1;
	bool status = false;
	int maxHp{};

	while (true) {
		cout << "----------------------------" << endl;
		cout << "��ɾ �Է��ϼ��� : ";
		cin >> order_s;

		switch (order_v(order_s))
		{
		case SEARCH_O:
			cin >> c1.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				c1.name += " ";
				c1.name += s1;
			}
			if (cin.fail()) {
				cout << "�߸��� �Է°� �Դϴ�." << endl;
				break;
			}
			break;
		case INSERT_O:
			cout << "name : ";
			cin >> c1.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				c1.name += " ";
				c1.name += s1;
			}
			cout << "hp : ";
			cin >> c1.hp;
			cout << "mp : ";
			cin >> c1.mp;
			cout << "speed : ";
			cin >> c1.speed;
			cout << "range : ";
			cin >> c1.range;
			cout << "position : ";
			cin >> c1.position;
			list.add(c1);
			cout << c1.name << " è�Ǿ��� ���� �Ͽ����ϴ�." << endl;
			break;
		case DELETE_O:
			cin >> c1.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				c1.name += " ";
				c1.name += s1;
			}
			list.delete_L(c1.name);
			break;
		case DELETE_ALL_O:
			cin >> c1.position;
			break;
		case PRINT_ALL_O:
			list.printAll();
			break;
		case FINDMAXHP_O:
			break;
		case SORTBYHP_O:
			cout << "champion�� hp�� �������� ���� �߽��ϴ�." << endl;
			break;
		default:
			cout << "�߸��� ��ɾ� �Դϴ�." << endl;
			break;
		}


	}

}

ORDER order_v(string order_s) {
	if (order_s == "Search")
		return SEARCH_O;
	if (order_s == "Insert")
		return INSERT_O;
	if (order_s == "Delete")
		return DELETE_O;
	if (order_s == "DeleteAll")
		return DELETE_ALL_O;
	if (order_s == "PrintAll")
		return PRINT_ALL_O;
	if (order_s == "FindMaxHp")
		return FINDMAXHP_O;
	if (order_s == "SortByHp")
		return SORTBYHP_O;
	return NULL_O;
}