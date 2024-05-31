#include<iostream>
#include <string>
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
			return;
		}

		do {
			Insert_BST(root, current->champion);
			current = current->next;
		} while (current != list_sl.getHead());
	}

	void Insert_BST(TreeNode* node, Champion p) {
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

	void PrintAll_BST(TreeNode* node) {
		if (node != nullptr) {
			PrintAll_BST(node->left);
			print_v(node->champion);
			PrintAll_BST(node->right);
		}
	}

	void SearchByName_BST(TreeNode* node, const string& name) {
		if (node == nullptr) {
			cout << "¾ø¾î!" << endl;
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

	void 

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
	ifstream file("");

	if (!file.is_open()) {

	}


}

int main() {
	
}