#include <iostream>
#include <string>
#include<fstream>

using namespace std;

struct Champion
{
	string position;
	string name;
	int hp;
	int attack;
	int defense;
};

struct Node {
	Champion champion;
	Node* left;
	Node* right;
	Node(Champion p) :champion(p), left(nullptr), right(nullptr) {};
};

class BST {
private:
	Node* head = nullptr;
public:

	Node*& getHead() {
		return head;
	}

	void insert_BST(Node*& node, Champion p) {
		if (node == nullptr) {
			node = new Node(p);
		}
		else if (p.name < node->champion.name) {
			insert_BST(node->left, p);
		}
		else {
			insert_BST(node->right, p);
		}
	}

	void print_BST(Node* node) {
		if (node != nullptr) {
			print_BST(node->left);
			print_v(node->champion);
			print_BST(node->right);
		}
	}

	void search_BST(Node* node, string name) {
		if (node == nullptr) {
			return;
		}

		if (name < node->champion.name) {
			search_BST(node->left, name);
		}
		else if (name > node->champion.name) {
			search_BST(node->right, name);
		}
		else {
			print_v(node->champion);
		}
	}

	void delete_BST(Node* node, string name) {
		if (node == nullptr) {
			return;
		}

		if (name < node->champion.name) {
			delete_BST(node->left, name);
		}
		else if (name > node->champion.name) {
			delete_BST(node->right, name);
		}
		else {
			if (node->left == nullptr && node->right == nullptr) {
				delete node;
				node = nullptr;
			}
			else if (node->left == nullptr) {
				Node* temp = node;
				node = node->right;
				delete temp;
			}
			else if (node->right == nullptr) {
				Node* temp = node;
				node = node->left;
				delete temp;
			}
			else {
				Node* temp = finMin(node->right);

			}
		}
	}

private:
	void print_v(Champion p) {
		cout << p.position << " " << p.name << " " << p.hp << " " << p.attack << " " << p.defense << endl;
	}
};

void loadFile(BST& bst) {
	fstream file("LOLDic_pr.txt");
	if (!file.is_open()) {
		return;
	}

	Champion p;
	while (file >> p.position >> p.name >> p.hp >> p.attack >> p.defense) {
		bst.insert_BST(bst.getHead(), p);
	}

	file.close();
}

int main() {
	BST bst;
	loadFile(bst);

	bst.print_BST(bst.getHead());
}