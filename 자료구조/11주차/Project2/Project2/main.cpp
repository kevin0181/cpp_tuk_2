#include <iostream>
#include<string>
#include<fstream>
#include<Windows.h>

using namespace std;

struct Champion {
	string position;
	string name;
	int hp;
	int attack;
	int defense;
};

struct SingleNode {
	Champion champion;
	SingleNode* next;

	SingleNode(Champion p) : champion(p), next(nullptr) {};
};

struct TreeNode {
	Champion champion;
	TreeNode* right;
	TreeNode* left;

	TreeNode(Champion p) : champion(p), right(nullptr), left(nullptr) {};
};

class SingleLinkedList {
private:
	SingleNode* head = nullptr;
	SingleNode* tail = nullptr;
public:

	void Insert_SL(Champion p) {
		SingleNode* newNode = new SingleNode(p);

		if (head == nullptr) {
			head = tail = newNode;
			tail->next = head;
		}
		else {
			tail->next = newNode;
			newNode->next = head;
			tail = newNode;
		}
	}

	void PrintAll_SL() {
		if (head == nullptr) {
			cout << "no data" << endl;
			return;
		}

		SingleNode* current = head;
		do {
			print_v(current->champion);
			current = current->next;
		} while (current != head);
	}

	void SortByName_SL() {
		if (head == nullptr) {
			cout << "no data" << endl;
			return;
		}

		bool swap_s;
		do {
			swap_s = false;
			SingleNode* current = head;
			SingleNode* next = head->next;
			do {
				if (current->champion.name < next->champion.name) {
					swap(current->champion, next->champion);
					swap_s = true;
				}
				current = next;
				next = current->next;
			} while (current != tail);
		} while (swap_s);
	}

	SingleNode* getHead() {
		return head;
	}

private:
	void print_v(Champion p) {
		cout << p.position << " " << p.name << " " << p.hp << " " << p.attack << " " << p.defense << endl;
	}

	void swap(Champion& a, Champion& b) {
		Champion temp = a;
		a = b;
		b = temp;
	}
};

class BST {
private:
public:
	TreeNode* root = nullptr;

	void InsertSingleLinkedList(SingleLinkedList& list) {
		SingleNode* current = list.getHead();
		if (current == nullptr) {
			cout << "no data" << endl;
			return;
		}

		do {
			Insert_BST(root, current->champion);
			current = current->next;
		} while (current != list.getHead());
	}

	void Insert_BST(TreeNode*& node, Champion p) {
		if (node == nullptr) {
			node = new TreeNode(p);
		}
		else if (p.name < node->champion.name) {
			Insert_BST(node->left, p);
		}
		else {
			Insert_BST(node->right, p);
		}
	}

	void InsertSingleLinkedList2(SingleLinkedList& list) {
		SingleNode* current = list.getHead();
		if (current == nullptr) {
			cout << "no data" << endl;
			return;
		}

		do {
			Insert_BT(root, current->champion);
			current = current->next;
		} while (current != list.getHead());
	}

	void Insert_BT(TreeNode*& node, Champion p) {
		if (node == nullptr) {
			node = new TreeNode(p);
		}
		else if (p.name > node->champion.name) {
			Insert_BT(node->left, p); // 내림차순 삽입을 위해 왼쪽에 삽입
		}
		else {
			Insert_BT(node->right, p); // 내림차순 삽입을 위해 오른쪽에 삽입
		}
	}

	void SearchByName_BST(TreeNode* node, const string& name) const{
		if (node == nullptr) {
			cout << "no data" << endl;
			return;
		}

		if (name < node->champion.name) {
			SearchByName_BST(node->left, name);
		}
		else if (name > node->champion.name) {
			SearchByName_BST(node->right, name);
		}
		else {
			print_v(node->champion);
		}
	}

	void PrintAll_BST(TreeNode* node) {
		if (node == nullptr) {
			return;
		}

		PrintAll_BST(node->left);
		print_v(node->champion);
		PrintAll_BST(node->right);
	}

	void Delete_BST(TreeNode*& node, const string& name) {
		if (node == nullptr) {
			cout << "no data" << endl;
			return;
		}

		if (name < node->champion.name) {
			Delete_BST(node->left, name);
		}
		else if (name > node->champion.name) {
			Delete_BST(node->right, name);
		}
		else {
			if (node->left == nullptr && node->right == nullptr) {
				delete node;
				node = nullptr;
			}
			else if (node->left == nullptr) {
				TreeNode* temp = node;
				node = node->right;
				delete temp;
			}
			else if (node->right == nullptr) {
				TreeNode* temp = node;
				node = node->left;
				delete temp;
			}
			else {
				TreeNode* temp = findMin(node->right);
				node->champion = temp->champion;
				Delete_BST(node->right, temp->champion.name);
			}
		}
	}

	void SortByName_BST2(TreeNode* node) {
		if (node != nullptr) {
			SortByName_BST2(node->right);
			print_v(node->champion);
			SortByName_BST2(node->left);
		}
	}

private:

	TreeNode* findMin(TreeNode* node) const {
		while (node->left != nullptr) node = node->left;
		return node;
	}

	void print_v(Champion p) const {
		cout << p.position << " " << p.name << " " << p.hp << " " << p.attack << " " << p.defense << endl;
	}

};

void loadFile(SingleLinkedList& list) {
	ifstream file("LOLDic_pr.txt");

	if (!file.is_open()) {
		cerr << "file open fail" << endl;
		return;
	}

	Champion p;
	while (file >> p.position >> p.name >> p.hp >> p.attack >> p.defense) {
		list.Insert_SL(p);
	}

	file.close();
}

enum ORDER {
	SEARCHBYNAME_BST,
	INSERT_BST,
	DELETE_BST,
	PRINTALL_BST,
	SORTBYNAME_BST_1,
	SORTBYNAME_BST_2,
	NULL_BST
};

ORDER order_v(const string& order) {
	if (order == "SearchByName")
		return SEARCHBYNAME_BST;
	if (order == "Insert")
		return INSERT_BST;
	if (order == "Delete")
		return DELETE_BST;
	if (order == "PrintAll")
		return PRINTALL_BST;
	if (order == "SortByName")
		return SORTBYNAME_BST_1;
	if (order == "SortByName2")
		return SORTBYNAME_BST_2;
	return NULL_BST;
}

int main() {

	LARGE_INTEGER frequency, start, end;

	SingleLinkedList list_sl;

	loadFile(list_sl);

	QueryPerformanceFrequency(&frequency);

	QueryPerformanceCounter(&start);
	list_sl.SortByName_SL();
	QueryPerformanceCounter(&end);

	cout << "SortByName_SL - 수행 시간 : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;

	list_sl.PrintAll_SL();

	BST bst;
	bst.InsertSingleLinkedList(list_sl);

	while (true) {
		cout << " ------------------------------------------- " << endl;
		cout << "order : ";
		string order;
		
		cin >> order;
		
		if (cin.fail()) {
			cout << "잘못된 명령어" << endl;
			cin.clear();
		}

		switch (order_v(order))
		{
		case SEARCHBYNAME_BST:
		{
			string name;

			cout << "name : ";
			cin >> name;

			if (cin.fail()) {
				cout << "잘못된 명령어" << endl;
				cin.clear();
				break;
			}
			QueryPerformanceCounter(&start);
			bst.SearchByName_BST(bst.root, name);
			QueryPerformanceCounter(&end);
			cout << "SortByName_SL - 수행 시간 : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;

			break;
		}
		case INSERT_BST:
		{
			Champion p;
			cout << "position : ";
			cin >> p.position;
			cout << "name : ";
			cin >> p.name;
			cout << "hp : ";
			cin >> p.hp;
			cout << "attack : ";
			cin >> p.attack;
			cout << "defense : ";
			cin >> p.defense;

			if (cin.fail()) {
				cout << "잘못된 명령어" << endl;
				cin.clear();
				break;
			}
			QueryPerformanceCounter(&start);
			bst.Insert_BST(bst.root, p);
			QueryPerformanceCounter(&end);
			cout << "SortByName_SL - 수행 시간 : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;

			break;
		}
		case DELETE_BST:
		{
			string name;

			cout << "name : ";
			cin >> name;

			if (cin.fail()) {
				cout << "잘못된 명령어" << endl;
				cin.clear();
				break;
			}
			QueryPerformanceCounter(&start);
			bst.Delete_BST(bst.root, name);
			QueryPerformanceCounter(&end);
			cout << "SortByName_SL - 수행 시간 : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;

			break;
		}
		case PRINTALL_BST:
			QueryPerformanceCounter(&start);
			bst.PrintAll_BST(bst.root);
			QueryPerformanceCounter(&end);
			cout << "SortByName_SL - 수행 시간 : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
			break;
		case SORTBYNAME_BST_1:
		{
			QueryPerformanceCounter(&start);
			// 새로운 BST 생성
			BST bst2;

			// 원본 리스트로부터 내림차순 삽입
			bst2.InsertSingleLinkedList2(list_sl);

			// 원본 bst를 지우고 새로운 bst2를 복사
			bst.root = nullptr; // 기존 bst의 트리를 비움
			bst = bst2; // 새로운 bst2를 bst로 대입
			QueryPerformanceCounter(&end);
			cout << "SortByName_SL - 수행 시간 : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;

			cout << "내림차순으로 정렬" << endl;
			break;
		}
		case SORTBYNAME_BST_2:
			QueryPerformanceCounter(&start);
			bst.SortByName_BST2(bst.root);
			QueryPerformanceCounter(&end);
			cout << "SortByName_SL - 수행 시간 : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
			break;
		default:
			cout << "틀린 명령어" << endl;
			break;
		}
		
	}
}