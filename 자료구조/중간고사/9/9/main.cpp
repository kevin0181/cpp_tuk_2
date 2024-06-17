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
	Node* prev;
	Node(Champion p) :champion(p), next(nullptr),prev(nullptr) {};
};

class DoubleLinkedList {
private:
	Node* head = nullptr;
	Node* tail = nullptr;
public:

	void Insert_DL(Champion p) {
		Node* newNode = new Node(p);

		if (head == nullptr) {
			head = tail = newNode;
			tail->next = head;
			head->prev = tail;
		}
		else {
			tail->next = newNode;
			newNode->next = head;
			newNode->prev = tail;
			head->prev = newNode;
			tail = newNode;
		}
	}

	void print_DL() {
		if (head == nullptr) {
			cout << "no data" << endl;
			return;
		}

		Node* current = head;
		do {
			print(current->champion);
			current = current->next;
		} while (current != head);
	}

	void delete_DL(string name) {
		if (head == nullptr) {
			cout << "no data" << endl;
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

	void sortBy1() {
		if (head == nullptr) {
			cout << "no data" << endl;
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
			} while (current != tail);
		} while (swap_s);
	}

	void sortBy2() {
		if (head == nullptr) {
			cout << "no data" << endl;
			return;
		}
		bool swap_s;

		do {
			swap_s = false;
			Node* current = head;
			Node* next = head->next;
			do {
				if (current->champion.name > next->champion.name) {
					swap(current->champion, next->champion);
					swap_s = true;
				}
				current = next;
				next = current->next;
			} while (current != tail);
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

void loadFile(DoubleLinkedList& list) {
	fstream file("LOLDic_pr.txt");
	if (!file.is_open()) {
		cout << "file open fail" << endl;
		return;
	}

	Champion p;
	while (file >> p.position >> p.name >> p.hp >> p.attack >> p.defense) {
		list.Insert_DL(p);
	}

	file.close();
}

int main() {
	DoubleLinkedList list_dl;
	loadFile(list_dl);

	list_dl.print_DL();
	cout << "--------------------------" << endl;
	//list_dl.delete_DL("A2227316760");
	cout << "----------------s1---------------" << endl;
	//list_dl.print_DL();

	list_dl.sortBy1();
	list_dl.print_DL();
	cout << "--------------s2-----------------" << endl;

	list_dl.sortBy2();
	list_dl.print_DL();
	cout << "-------------------------------" << endl;
}