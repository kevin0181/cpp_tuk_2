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

struct Node{
	Champion champion;
	Node* next;
	Node(Champion p) : champion(p), next(nullptr) {};
};

class LinkedList {
private:
	Node* head;
	Node* tail;
	bool sort_status = false;
public:
	LinkedList() {};
	~LinkedList() {};

	void Insert_SL(Champion p) {
		Node* newNode = new Node(p);
		if(head == nullptr){
			head = tail = newNode;
			tail->next = head;
		}
		else {
			tail->next = newNode;
			newNode->next = head;
			tail = newNode;
		}
	}

	void printAll_SL() {
		Node* current = head;
		do {
			print_v(current->champion);
			current = current->next;
		} while (current != head);
	}


private:
	void print_v(Champion p) {
		cout << p.name << " " << p.hp << " " << p.mp << " " << p.speed << " " << p.range << " " << p.position << endl;
	}

	void swap(Champion& a, Champion& b) {
		Champion temp;
		temp = a;
		a = b;
		b = temp;
	}
};

void Array2List(LinkedList& list, Champion *c,int size) {
	for (int i = 0; i < size; ++i) {
		list.Insert_SL(c[i]);
	}
}

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


ORDER_E order_v(string order) {
	if (order == "search")
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
	
	cout << "Search [è�Ǿ��̸�] : �־��� è�Ǿ��� ������ ����Ѵ�." << endl;
	cout << "Insert : ���ο� è�Ǿ��� ������ �Է¹޾ƻ����Ѵ�." << endl;
	cout << "Delete [è�Ǿ��̸�] : �־��� è�Ǿ� ���� ������ �����Ѵ�." << endl;
	cout << "DeleteAll [position] : �־��� ��ġ�� ��� è�Ǿ� ������ �����Ѵ�." << endl;
	cout << "PrintAll : ��� è�Ǿ��� ������ �迭�� ����� ������� ����Ѵ�." << endl;
	cout << "FindMaxHp: ���� ü���� ū è�Ǿ��� ������ ����Ѵ�." << endl;
	cout << "SortByHp: ü���� ū è�Ǿ���� ������� �����Ѵ�." << endl;

	LinkedList list;
	string order, s1;
	Champion c1;

	Array2List(list, c_l, sizeof(c_l) / sizeof(c_l[0]));

	while (true)
	{
		cout << "---------------" << endl;
		cout << "��ɾ �Է��ϼ��� : ";
		cin >> order;
		if (cin.fail()) {
			cout << "�߸��� �Է°� �Դϴ�." << endl;
			cin.clear();
			exit(0);
		}
		switch (order_v(order))
		{
		case SEARCH_E:
			break;
		case INSERT_E:
			break;
		case DELETE_E:
			break;
		case DELETEALL_E:
			break;
		case PRINTALL_E:
			list.printAll_SL();
			break;
		case FINDMAXHP_E:
			break;
		case SORTBYHP_E:
			break;
		default:
			cout << "�߸��� ��ɾ� �Դϴ�." << endl;
			break;
		}
	}

}