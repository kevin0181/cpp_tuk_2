#include<iostream>
#include <string>
#include<fstream>

using namespace std;

struct Champion {
	string position;
	string name;
	int hp;
	int power;
	int defence;
};

struct SingleNode {
	Champion champion;
	SingleNode* next;
	SingleNode(Champion p) :champion(p), next(nullptr) {};
};

struct DoubleNode {
	Champion champion;
	DoubleNode* next;
	DoubleNode* prev;
	DoubleNode(Champion p) :champion(p), next(nullptr), prev(nullptr) {};
};

class SingleLinkedList {
private :
	SingleNode* head;
	SingleNode* tail;
public:
	void Insert_SL(Champion p) {
		SingleNode* newNode = new SingleNode(p);

		if (newNode == nullptr) {
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


	}

private:
	void print_v(Champion p) {
		cout << p.position << " " << p.name << " " <<p.
	}

};

class DoubleLinkedList {

};

int main() {

}