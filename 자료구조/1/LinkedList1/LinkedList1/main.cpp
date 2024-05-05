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

// 단일 환형 연결 리스트의 노드를 정의하는 구조체
struct Node {
	Champion data; // 챔피언 데이터
	Node* next; // 다음 노드를 가리키는 포인터

	Node(Champion c) : data(c), next(nullptr) {} // 생성자
};

// 단일 환형 연결 리스트 클래스
class SingleCircularLinkedList {
private:
	Node* head; // 리스트의 시작을 가리키는 헤드 포인터
	Node* tail; // 리스트의 마지막을 가리키는 테일 포인터
	bool sort_status = false;
public:
	SingleCircularLinkedList() : head(nullptr) {} // 생성자

	// 소멸자
	~SingleCircularLinkedList() {
		if (head == nullptr) return;
		tail->next = nullptr;
		Node* current = head;
		while (current != nullptr) {
			Node* next = current->next;
			delete current;
			current = next;
		}
	}

	void add(Champion p) {
		Node* newNode = new Node(p);
		if (head == nullptr) {
			head = tail = newNode; // 첫 노드를 헤드와 테일로 설정
			tail->next = head; // 환형 구조를 만듦
		}
		else {
			tail->next = newNode; // 마지막 노드의 next를 새 노드로 설정
			newNode->next = head; // 새 노드의 next를 헤드로 설정
			tail = newNode; // 새 노드를 새로운 테일로 설정
		}

		if (sort_status)
			sortByHp();

	}

	void printAll() {
		if (head == nullptr) return;
		
		Node* current = head;
		do {
			printChampion(current->data);
			current = current->next;
		} while (current != head); // 다시 헤드로 돌아올 때까지 반복
	}

	void delete_L(string name) {
		if (head == nullptr) return;
		Node* current = head;
		Node* prev = tail;  
		do {
			if (current->data.name == name) {
				if (current == head && current == tail) {
					// 유일한 노드인 경우
					head = tail = nullptr;
				}
				else if (current == head) {
					// 삭제 노드가 헤드인 경우
					head = head->next;
					tail->next = head;
				}
				else if (current == tail) {
					// 삭제 노드가 테일인 경우
					tail = prev;
					tail->next = head;
				}
				else {
					// 중간 노드 삭제
					prev->next = current->next;
				}
				delete current; // 메모리 해제
				break;
			}
			prev = current;
			current = current->next;
		} while (current != head);
	}

	void deleteAll_L(string position) {

		if (head == nullptr) return;

		Node* current = head;
		Node* prev = tail;
		Node* toDelete = nullptr;
		bool continueLoop = true;

		while (continueLoop) {
			if (current->data.position == position) {
				if (current == head && current == tail) {
					toDelete = current;
					head = tail = nullptr;
					continueLoop = false;
				}
				else if (current == head) {
					head = head->next;
					tail->next = head;
					toDelete = current;
					current = head;
				}
				else if (current == tail) {
					tail = prev;
					tail->next = head;
					toDelete = current;
					continueLoop = false;
				}
				else {
					prev->next = current->next;
					toDelete = current;
					current = current->next;
				}
				delete toDelete; // 메모리 해제
				if (head == nullptr) break;
			}
			else {
				prev = current;
				current = current->next;
				if (current == head) break;
			}
		}
	}

	void findMaxHp_L() {
		if (head == nullptr) return;

		Node* current = head;
		int maxHp = current->data.hp;
		do {
			if (current->data.hp > maxHp) {
				maxHp = current->data.hp;
			}
			current = current->next;
		} while (current != head);

		current = head;
		do {
			if (current->data.hp == maxHp) {
				printChampion(current->data);
			}
			current = current->next;
		} while (current != head);
	}

	void sortByHp() {
		if (head == nullptr)return;

		sort_status = true;
		bool swapped;
		do {
			swapped = false;
			Node* current = head;
			Node* next = head->next;

			do {
				if (current->data.hp < next->data.hp) { // 현재 노드의 hp가 다음 노드의 hp보다 작다면
					swap(current->data, next->data); // 데이터를 스왑
					swapped = true;
				}
				current = next;
				next = current->next;
			} while (current != tail); // 환형 리스트이므로 테일에서 멈춤

		} while (swapped); // 스왑이 일어나지 않을 때까지 반복

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

// 배열의 데이터를 단일 환형 연결 리스트로 변환하는 함수
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

	cout << "Search [챔피언이름] : 주어진 챔피언의 정보를 출력한다." << endl;
	cout << "Insert : 새로운 챔피언의 정보를 입력받아삽입한다." << endl;
	cout << "Delete [챔피언이름] : 주어진 챔피언에 대한 정보를 삭제한다." << endl;
	cout << "DeleteAll [position] : 주어진 위치의 모든 챔피언 정보를 삭제한다." << endl;
	cout << "PrintAll : 모든 챔피언의 정보를 배열에 저장된 순서대로 출력한다." << endl;
	cout << "FindMaxHp: 가장 체력이 큰 챔피언의 정보를 출력한다." << endl;
	cout << "SortByHp: 체력이 큰 챔피언부터 순서대로 저장한다." << endl;

	string order_s;
	string s1;
	Champion c1;
	bool status = false;
	int maxHp{};

	while (true) {
		cout << "----------------------------" << endl;
		cout << "명령어를 입력하세요 : ";
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
				cout << "잘못된 입력값 입니다." << endl;
				cin.clear();
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
			if (cin.fail()) {
				cout << "잘못된 입력값 입니다." << endl;
				cin.clear();
				break;
			}
			list.add(c1);
			cout << c1.name << " 챔피언을 생성 하였습니다." << endl;
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
			list.deleteAll_L(c1.position);
			break;
		case PRINT_ALL_O:
			list.printAll();
			break;
		case FINDMAXHP_O:
			list.findMaxHp_L();
			break;
		case SORTBYHP_O:
			list.sortByHp();

			cout << "체력(hp)을 기준으로 내림차순 정렬되었습니다." << endl;
			break;
		default:
			cout << "잘못된 명령어 입니다." << endl;
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