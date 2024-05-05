#include<iostream>
#include<string>

using namespace std;

struct Champion {
	string name;
	int hp;
	int mp;
	int speed;
	int range;
	string position;
};

struct Node {
	Champion champion;
	Node* next;
	Node(Champion p) :champion(p), next(nullptr) {};
};

class LinkedList {
private:
	Node* head;
	Node* tail;
	bool sort_status = false;
public:
	void Search_SL(string name) {
		if (head == nullptr) return;
		Node* current = head;
		do {
			if (current->champion.name == name) {
				print_v(current->champion);
			}
			current = current->next;
		} while (current != head);
	}
	void Insert_SL(Champion p) {
		Node* newNode = new Node(p);
		if (head == nullptr) {
			head = tail = newNode;
			tail->next = head;
		}
		else {
			tail->next = newNode;
			newNode->next = head;
			tail = newNode;
		}

		if (sort_status)
			SortByHp_SL();
	}
	void Delete_SL(string name) {
		if (head == nullptr) return;

		Node* current = head;
		Node* prev = tail;
		do {
			if (current->champion.name == name) {
				if (current == head && current == tail) {
					head = tail = nullptr;
				}
				else if (current == head) {
					head = head->next;
					tail->next = head;
				}
				else if (current == tail) {
					tail = prev;
					tail->next = head;
				}
				else {
					prev->next = current->next;
				}
				delete current;
				break;
			}
			prev = current;
			current = current->next;
		} while (current != head);
	}
	void DeleteAll_SL(string position) {
		if (head == nullptr)return;
		
		Node* current = head;
		Node* prev = tail;
		Node* toDelete = nullptr;
		bool loop_status = true;

		while (loop_status) {
			if (current->champion.position == position) {
				if (current == head && current == tail) {
					toDelete = current;
					head = tail = nullptr;
					loop_status = false;
				}
				else if (current == head) {
					toDelete = current;
					head = head->next;
					tail->next = head;
					current = head;
				}
				else if (current == tail) {
					toDelete = current;
					tail = prev;
					tail->next = head;
					loop_status = false;
				}
				else {
					toDelete = current;
					prev->next = current->next;
					current = current->next;
				}
				delete toDelete;
				if (head == nullptr) break;
			}
			else {
				prev = current;
				current = current->next;
				if (current == head) break;
			}
		}

	}
	void PrintAll_SL() {
		if (head == nullptr) return;
		Node* current = head;
		do {
			print_v(current->champion);
			current = current->next;
		} while (current != head);
	}
	void FindMaxHp_SL() {
		if (head == nullptr) return;

		Node* current = head;
		int maxHp = current->champion.hp;
		do {
			if (current->champion.hp > maxHp) {
				maxHp = current->champion.hp;
			}
			current = current->next;
		} while (current != head);

		current = head;

		do {
			if (current->champion.hp == maxHp) {
				print_v(current->champion);
			}
			current = current->next;
		} while (current != head);
	}
	void SortByHp_SL() {
		if (head == nullptr) return;
		sort_status = true;
		bool swap_status;
		do {
			swap_status = false;
			Node* current = head;
			Node* next = head->next;
			do {
				if (current->champion.hp < next->champion.hp) {
					swap(current->champion, next->champion);
					swap_status = true;
				}
				current = next;
				next = current->next;
			} while (current != tail);
		} while (swap_status);
	}
private:
	void print_v(Champion p) {
		cout << p.name << " " << p.hp << " " << p.mp << " " << p.speed << " " << p.range << " " << p.position << endl;
	}
	void swap(Champion& a, Champion& b) {
		Champion temp = a;
		a = b;
		b = temp;
	}
};

void Array2LinkedList(LinkedList& list, Champion* p, int size) {
	for (int i = 0; i < size; ++i) {
		list.Insert_SL(p[i]);
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

	LinkedList list;
	Array2LinkedList(list, c_l, sizeof(c_l) / sizeof(c_l[0]));

	string order, s1;
	Champion c1;

	while (true) {
		cout << "-------------------" << endl;
		cout << "명령어를 입력하세요 : ";
		cin >> order;
		if (cin.fail()) {
			cout << "잘못된 입력값 입니다." << endl;
			cin.clear();
			break;
		}

		switch (order_v(order))
		{
		case SEARCH_E:
			cin >> c1.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				c1.name += " ";
				c1.name += s1;
			}
			if (cin.fail()) {
				cout << "잘못된 입력값 입니다." << endl;
				cin.clear();
				break;
			}
			list.Search_SL(c1.name);
			break;
		case INSERT_E:
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
			cout << "range : ";
			cin >> c1.range;
			cout << "speed : ";
			cin >> c1.speed;
			cout << "position : ";
			cin >> c1.position;
			if (cin.fail()) {
				cout << "잘못된 입력값 입니다." << endl;
				cin.clear();
				break;
			}
			list.Insert_SL(c1);
			break;
		case DELETE_E:
			cin >> c1.name;
			if (cin.peek() != '\n') {
				getchar();
				cin >> s1;
				c1.name += " ";
				c1.name += s1;
			}
			if (cin.fail()) {
				cout << "잘못된 입력값 입니다." << endl;
				cin.clear();
				break;
			}
			list.Delete_SL(c1.name);
			break;
		case DELETEALL_E:
			cin >> c1.position;
			if (cin.fail()) {
				cout << "잘못된 입력값 입니다." << endl;
				cin.clear();
				break;
			}
			list.DeleteAll_SL(c1.position);
			break;
		case PRINTALL_E:
			list.PrintAll_SL();
			break;
		case FINDMAXHP_E:
			list.FindMaxHp_SL();
			break;
		case SORTBYHP_E:
			list.SortByHp_SL();
			cout << "hp를 내림차순으로 정렬하였습니다." << endl;
			break;
		default:
			cout << "잘못된 명령어 입니다." << endl;
			break;
		}
	}
}