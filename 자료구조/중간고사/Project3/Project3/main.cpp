#include <iostream>
#include<string>
#include<fstream>

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

	void print_SL() {
		if (head == nullptr) {
			return;
		}

		Node* current = head;
		do {
			print(current->champion);
			current = current->next;
		} while (current != head);
	}

	void search_SL(string name) {
		if (head == nullptr) {
			return;
		}

		Node* current = head;
		do {
			if (current->champion.name == name) {
				print(current->champion);
			}
			current = current->next;
		} while (current != head);
	}

	void delete_SL(string name) {
		if (head == nullptr) {
			return;
		}

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

	void sortByName_SL() {

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
	list_sl.search_SL("A2227316760");

}