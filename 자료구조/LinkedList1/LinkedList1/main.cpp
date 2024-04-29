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

public:
	SingleCircularLinkedList() : head(nullptr) {} // 생성자

	// 소멸자
	~SingleCircularLinkedList() {
		if (head == nullptr) return;

		Node* current = head->next;
		head->next = nullptr; // 순환 참조 끊기
		while (current != nullptr) {
			Node* next = current->next;
			delete current;
			current = next;
		}
	}

	void add(Champion p) {
		Node* newNode = new Node(p);
		if (head == nullptr) {
			newNode->next = newNode; // 새 노드가 자신을 가리키도록 설정
			head = newNode; // 새 노드를 헤드로 설정
		}
		else {
			Node* current = head;
			while (current->next != head) { // 마지막 노드 찾기
				current = current->next;
			}
			current->next = newNode; // 마지막 노드가 새 노드를 가리키도록 설정
			newNode->next = head; // 새 노드가 첫 번째 노드를 가리키도록 설정
		}
	}

	void printAll() {
		if (head == nullptr) return;
		
		Node* current = head->next;
		do {
			printChampion(current->data);
			current = current->next;
		} while (current != head->next); // 시작 노드로 돌아올 때까지 반복
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
			break;
		case PRINT_ALL_O:
			list.printAll();
			break;
		case FINDMAXHP_O:
			break;
		case SORTBYHP_O:
			cout << "champion의 hp를 내림차순 정렬 했습니다." << endl;
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