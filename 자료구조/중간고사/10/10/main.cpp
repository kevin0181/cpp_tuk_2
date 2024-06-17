#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Champion {
	string positon;
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

	Node* getHead() {
		return head;
	}

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

	void print_sl() {
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

	void print_sl2(Node* node) { //Àç±ÍÇÔ¼ö
		if (node == tail) {
			print(node->champion);
			return;
		}
		print(node->champion);
		print_sl2(node->next);
	}
private:
	void print(Champion p) {
		cout << p.positon << " " << p.name << " " << p.hp << " " << p.attack << " " << p.defense << endl;
	}
};

void loadFile(Champion champion_list[500]) {
	fstream file("LOLDic_pr.txt");
	if (!file.is_open()) {
		cout << "!" << endl;
		return;
	}
	int i = 0;
	Champion p;
	while (file >> p.positon >> p.name >> p.hp >> p.attack >> p.defense) {
		champion_list[i++] = p;
	}

	file.close();
}

void swap(Champion& a, Champion& b) {
	Champion temp = a;
	a = b;
	b = temp;
}

int main() {
	Champion champion_list[500];

	SingleLinkedList list_sl;
	
	loadFile(champion_list);
	
	int list_size = sizeof(champion_list) / sizeof(champion_list[0]);

	for (int i = 0; i < list_size; ++i) {
		for (int j = 0; j < list_size; ++j) {
			if (champion_list[i].name > champion_list[j].name) {
				swap(champion_list[i], champion_list[j]);
			}
		}
	}

	for (int i = 0; i < list_size; ++i) {
		if (champion_list[i].name == "") {
			continue;
		}
		list_sl.Insert_SL(champion_list[i]);
		cout << champion_list[i].name << endl;
	}

	list_sl.print_sl2(list_sl.getHead());

}