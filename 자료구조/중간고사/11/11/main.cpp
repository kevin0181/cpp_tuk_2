#include <iostream>
#include <string>
#include <fstream>

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

	void sortByName_SL() {
		if (head == nullptr) {
			return;
		}

		bool swap_s;
		do {
			swap_s = false;
			Node* current = head;
			Node* next = head->next;
			do {
				if (current->champion.name < next->champion.name) {
					swap(current->champion, next->champion);
					swap_s = true;
				}
				current = next;
				next = current->next;
			} while (next != head);
		} while (swap_s);
	}
private:
	void print(Champion p) {
		cout << p.position << " " << p.name << " " << p.hp << " " << p.attack << " " << p.defense << endl;
	}
	void swap(Champion& a, Champion& b) {
		Champion temp = a;
		a = b;
		b = temp;
	}
};

void loadFile(SingleLinkedList& list) {
	fstream file("LOLDic_pr.txt");

	if (!file.is_open()) {
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
}