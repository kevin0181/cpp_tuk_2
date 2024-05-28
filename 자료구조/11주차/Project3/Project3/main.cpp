#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>

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
	TreeNode* left;
	TreeNode* right;
	TreeNode(Champion p) : champion(p), left(nullptr), right(nullptr) {};
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
public:
	TreeNode* root = nullptr;

	void InsertSingleLinkedList(SingleLinkedList& list_sl) {
		SingleNode* current = list_sl.getHead();
		if (current == nullptr) {
			cout << "데이터가 존재하지 않습니다." << endl;
			return;
		}

		do {
			Insert_BST(root, current->champion);
			current = current->next;
		} while (current != list_sl.getHead());
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

	void InsertSingleLinkedList2(SingleLinkedList& list_sl) {
		SingleNode* current = list_sl.getHead();
		if (current == nullptr) {
			cout << "데이터가 존재하지 않습니다." << endl;
			return;
		}

		do {
			Insert_BT(root, current->champion);
			current = current->next;
		} while (current != list_sl.getHead());
	}

	void Insert_BT(TreeNode*& node, Champion p) {
		if (node == nullptr) {
			node = new TreeNode(p);
		}
		else if (p.name > node->champion.name) {
			Insert_BT(node->left, p);
		}
		else {
			Insert_BT(node->right, p);
		}
	}

	void PrintAll_BST(TreeNode* node) {
		if (node != nullptr) {
			PrintAll_BST(node->left);
			print_v(node->champion);
			PrintAll_BST(node->right);
		}
	}

	void SearchByName_BST(TreeNode* node, const string& name) {
		if (node == nullptr) {
			cout << "존재하지 않는 챔피언" << endl;
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

	void SortByName_BST(TreeNode* node) {
		if (node != nullptr) {
			SortByName_BST(node->right);
			print_v(node->champion);
			SortByName_BST(node->left);
		}
	}

	void SortByName_BT(SingleLinkedList list_sl, BST& bst2) {
		bst2.InsertSingleLinkedList2(list_sl);
	}

private:

	TreeNode* findMin(TreeNode* node) const {
		while (node->left != nullptr)
			node = node->left;
		return node;
	}

	void print_v(Champion p) {
		cout << p.position << " " << p.name << " " << p.hp << " " << p.attack << " " << p.defense << endl;
	}
};

void loadFile(SingleLinkedList& list) {
	ifstream file("LOLDic_pr.txt");

	if (!file.is_open()) {
		cerr << "파일 오픈 실패" << endl;
		return;
	}

	Champion p;
	while (file >> p.position >> p.name >> p.hp >> p.attack >> p.defense) {
		list.Insert_SL(p);
	}

	file.close();
}

enum ORDER {
	SEARCHBYNAME,
	INSERT,
	_DELETE,
	PRINTALL,
	SORTBYNAME,
	SORTBYNAME_V2,
	_NULL
};

ORDER order_v(const string& order) {
	if (order == "SearchByName")
		return SEARCHBYNAME;
	if (order == "Insert")
		return INSERT;
	if (order == "Delete")
		return _DELETE;
	if (order == "PrintAll")
		return PRINTALL;
	if (order == "SortByName")
		return SORTBYNAME;
	if (order == "SortByName2")
		return SORTBYNAME_V2;
	return _NULL;
}

int main() {
	SingleLinkedList list_sl;

	LARGE_INTEGER frequency, start, end;
	QueryPerformanceFrequency(&frequency);

	loadFile(list_sl);

	QueryPerformanceCounter(&start);
	list_sl.SortByName_SL();
	QueryPerformanceCounter(&end);
	cout << "단일 연결 리스트 - SortByName : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;

	BST bst;
	bst.InsertSingleLinkedList(list_sl);


	while (true) {
		cout << "--------------------------------" << endl;
		cout << "order : ";

		string order;

		cin >> order;

		if (cin.fail()) {
			cin.clear();
			cout << "잘못된 입력값 입니다." << endl;
		}
		
		Champion p;

		switch (order_v(order))
		{
		case SEARCHBYNAME:
			cout << "name : ";
			cin >> p.name;

			if (cin.fail()) {
				cin.clear();
				cout << "잘못된 입력값 입니다." << endl;
				break;
			}
			QueryPerformanceCounter(&start);
			bst.SearchByName_BST(bst.root, p.name);
			QueryPerformanceCounter(&end);
			cout << "이진 탐색 트리 - SearchByName_BST : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;

			break;
		case INSERT:
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
				cin.clear();
				cout << "잘못된 입력값 입니다." << endl;
				break;
			}
			QueryPerformanceCounter(&start);
			bst.Insert_BST(bst.root, p);
			QueryPerformanceCounter(&end);
			cout << "이진 탐색 트리 - Insert_BST : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
			break;
		case _DELETE:
			cout << "name : ";
			cin >> p.name;

			if (cin.fail()) {
				cin.clear();
				cout << "잘못된 입력값 입니다." << endl;
				break;
			}
			QueryPerformanceCounter(&start);
			bst.Delete_BST(bst.root, p.name);
			QueryPerformanceCounter(&end);
			cout << "이진 탐색 트리 - Delete_BST : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
			break;
		case PRINTALL:
			QueryPerformanceCounter(&start);
			bst.PrintAll_BST(bst.root);
			QueryPerformanceCounter(&end);
			cout << "이진 탐색 트리 - PrintAll_BST : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
			break;
		case SORTBYNAME:
		{
			QueryPerformanceCounter(&start);
			BST bst2;
			bst2.SortByName_BT(list_sl, bst2);
			bst = bst2;
			QueryPerformanceCounter(&end);
			cout << "이진 트리 - SortByName_BT : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
			cout << "내림차순으로 정렬되었습니다." << endl;
			break;
		}
		case SORTBYNAME_V2:
			QueryPerformanceCounter(&start);
			bst.SortByName_BST(bst.root);
			QueryPerformanceCounter(&end);
			cout << "이진 탐색 트리 - SortByName_BST : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
			break;
		default:
			cout << "잘못된 입력값" << endl;
			break;
		}
	}
}