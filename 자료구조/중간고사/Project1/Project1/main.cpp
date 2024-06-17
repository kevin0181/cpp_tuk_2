#include<iostream>
#include<string>
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
	Node* next;
	Node(Champion p) : champion(p), next(nullptr) {};
};
class SingleLinkedList {
private:
	Node* head = nullptr;
public:

	Node* getHead() {
		return head;
	}

	void Insert_SL(Champion champion) {
		Node* newNode = new Node(champion);

		if (head == nullptr) {
			head = newNode;
		}
		else {
			newNode->next = head;
			head = newNode;
		}
	}

	void search_SL(Node* node, string name) {
		if (node == nullptr) {
			cout << "ã�� �̸� ���� ��" << endl;
			return;
		}
		
		if (node->champion.name == name) {
			print(node->champion);
		}
		else {
			search_SL(node->next, name);
		}
	}

	void print_SL() {
		if (head == nullptr) {
			cout << "������ X" << endl;
			return;
		}

		Node* current = head;
		while (current != nullptr) {
			print(current->champion);
			current = current->next;
		}
	}

private:
	void print(Champion p) {
		cout << p.position << " " << p.name << " " << p.hp << " " << p.attack << " " << p.defense << endl;
	}
};

void loadFile(SingleLinkedList& list) {
	fstream file("LOLDic_pr.txt");
	
	if (!file.is_open()) {
		cerr << "���� ����" << endl;
		return;
	}

	Champion c;
	while (file >> c.position >> c.name >> c.hp >> c.attack >> c.defense) {
		list.Insert_SL(c);
	}

	file.close();
}

int main() {
	SingleLinkedList list_sl;
	loadFile(list_sl);

	
	list_sl.print_SL();

	cout << "----------------------" << endl;
	list_sl.search_SL(list_sl.getHead(), "A2227316760");

}