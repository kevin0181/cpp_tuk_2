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
	SingleNode(Champion p) :champion(p), next(nullptr) {};
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

	void Insert_BST(TreeNode*& node, Champion champion) {
		if (node == nullptr) {
			node = new TreeNode(champion);
		}
		else if (champion.name < node->champion.name) {
			Insert_BST(node->left, champion);
		}
		else {
			Insert_BST(node->right, champion);
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
			cout << "노데이터" << endl;
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

	void Delete_BST(TreeNode* node, const string& name) {
		if (node == nullptr) {
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

private:
	TreeNode* findMin(TreeNode* node) {
		while (node->left != nullptr)
			node = node->left;
		return node;
	}
	
	void print_v(Champion p) {
		cout << p.position << " " << p.name << " " << p.hp << " " << p.attack << " " << p.defense << endl;
	}
};

void loadFile(SingleLinkedList& list_sl) {
	ifstream file("LOLDic_pr.txt");

	if (!file.is_open()) {
		cerr << "file open fail" << endl;
		return;
	}

	Champion p;
	while (file >> p.position >> p.name >> p.hp >> p.attack >> p.defense) {
		list_sl.Insert_SL(p);
	}

	file.close();
}

int main() {
	SingleLinkedList list_sl;
	loadFile(list_sl);

	list_sl.SortByName_SL();

	BST bst;
	bst.InsertSingleLinkedList(list_sl);

	Champion p;
	//bst.Insert_BST(bst.root, p);
	bst.PrintAll_BST(bst.root);
	bst.SearchByName_BST(bst.root, "kk");
	bst.SortByName_BST(bst.root);
}