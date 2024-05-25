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

struct Node {
	Champion champion;
	Node* next;
	Node(Champion p) :champion(p), next(nullptr) {};
};

class SingleLinkedList {
private:
	Node* head = nullptr;
	Node* tail = nullptr;
public:

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
	}

	void PrintAll_SL() {
		if (head == nullptr) {
			cout << "no data" << endl;
			return;
		}

		Node* current = head;
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
			Node* current = head;
			Node* next = head->next;
			swap_s = false;
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

	Node* getHead() {
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

struct TreeNode {
	Champion champion;
	TreeNode* left;
	TreeNode* right;
	TreeNode(Champion c) : champion(c), left(nullptr), right(nullptr) {}
};

class BST {
public:
	TreeNode* root = nullptr;

	void SearchByName_BT(TreeNode* node, const string& name) const {
		if (node == nullptr) {
			cout << "Champion not found" << endl;
			return;
		}

		if (name < node->champion.name) {
			SearchByName_BT(node->left, name);
		}
		else if (name > node->champion.name) {
			SearchByName_BT(node->right, name);
		}
		else {
			// Node found
			print_v(node->champion);
		}
	}

	void Insert_BT(TreeNode*& node, Champion champion) {
		if (node == nullptr) {
			node = new TreeNode(champion);
		}
		else if (champion.name < node->champion.name) {
			Insert_BT(node->left, champion);
		}
		else {
			Insert_BT(node->right, champion);
		}
	}

	void insertFromList(SingleLinkedList& list) {
		Node* current = list.getHead();
		if (current == nullptr) return;

		do {
			Insert_BT(root, current->champion);
			current = current->next;
		} while (current != list.getHead());
	}

	void PrintAll_BT(TreeNode* node) {
		if (node != nullptr) {
			PrintAll_BT(node->left);
			print_v(node->champion);
			PrintAll_BT(node->right);
		}
	}


	void Delete_BT(TreeNode*& node, const string& name) {
		if (node == nullptr) return;

		if (name < node->champion.name) {
			Delete_BT(node->left, name);
		}
		else if (name > node->champion.name) {
			Delete_BT(node->right, name);
		}
		else {
			// Node found
			if (node->left == nullptr && node->right == nullptr) {
				// Case 1: No children (leaf node)
				delete node;
				node = nullptr;
			}
			else if (node->left == nullptr) {
				// Case 2: One child (right)
				TreeNode* temp = node;
				node = node->right;
				delete temp;
			}
			else if (node->right == nullptr) {
				// Case 2: One child (left)
				TreeNode* temp = node;
				node = node->left;
				delete temp;
			}
			else {
				// Case 3: Two children
				TreeNode* temp = findMin(node->right);
				node->champion = temp->champion;
				Delete_BT(node->right, temp->champion.name);
			}
		}
	}


	void SortByName_BT(TreeNode* node) const {
		if (node != nullptr) {
			SortByName_BT(node->right);
			print_v(node->champion);
			SortByName_BT(node->left);
		}
	}

private:

	TreeNode* findMin(TreeNode* node) const {
		while (node->left != nullptr) node = node->left;
		return node;
	}

	void print_v(const Champion& p) const {
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

int main() {
	SingleLinkedList list_sl;
	loadFile(list_sl);

	list_sl.SortByName_SL(); // 이름을 키값으로 정렬

	BST bst;
	bst.insertFromList(list_sl);

	// 새 챔피언 데이터를 BST에 삽입 예제
	Champion newChampion = { "llkkkk", "Aed", 2000, 300, 150 };
	bst.Insert_BT(bst.root, newChampion);

	// 챔피언 데이터를 BST에서 검색 예제
	cout << "\n검색 결과 : ";
	bst.SearchByName_BT(bst.root, "Aed");

	// 챔피언 데이터를 BST에서 삭제 예제
	bst.Delete_BT(bst.root, "Aed");

	bst.SortByName_BT(bst.root);

	cout << "-----------------------------------------------------" << endl;

	bst.PrintAll_BT(bst.root); // 트리 출력
}