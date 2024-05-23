#include<iostream>
#include <string>
#include<fstream>
#include<Windows.h>

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
	SingleNode(Champion p) : champion(p), next(nullptr) {};
};

struct DoubleNode {
	Champion champion;
	DoubleNode* next;
	DoubleNode* prev;
	DoubleNode(Champion p) : champion(p), next(nullptr), prev(nullptr) {};
};

template <typename Node>
class LinkedList {
protected:
	Node* head;
	Node* tail;
public:

	virtual void Insert_SL_DL(Champion p) = 0;

	void PrintAll_SL_DL() {
		if (head == nullptr) {
			cout << " no data" << endl;
			return;
		}

		Node* current = head;
		do {
			print_v(current->champion);
			current = current->next;
		} while (current != head);
	}

	void FindMaxHp_SL_DL() {
		if (head == nullptr) {
			cout << " no data" << endl;
			return;
		}

		Node* current = head;
		int maxHp = current->champion.hp;
		do {
			if (current->champion.hp > maxHp) {
				maxHp = current->champion.hp;
			}
			current = current->next;
		} while (current != head);

		do {
			if (current->champion.hp == maxHp) {
				print_v(current->champion);
			}
			current = current->next;
		} while (current != head);
	}

	void SortByName_SL_DL() {
		if (head == nullptr) {
			cout << " no data" << endl;
			return;
		}

		bool swap_status;
		do {
			swap_status = false;
			Node* current = head;
			Node* next = head->next;
			do {
				if (current->champion.name < next->champion.name) {
					swap(current->champion, next->champion);
					swap_status = true;
				}
				current = next;
				next = current->next;
			} while (current != tail);
		} while (swap_status);
	}

protected:
	void print_v(Champion p) {
		cout << p.position << " " << p.name << " " << p.hp << " " << p.power << " " << p.defence << endl;
	}

	void swap(Champion& a, Champion& b) {
		Champion temp = a;
		a = b;
		b = temp;
	}
};

class SingleLinkedList :public LinkedList<SingleNode> {
public:
	void Insert_SL_DL(Champion p) override {
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
};

class DoubleLinkedList : public LinkedList<DoubleNode> {
public:
	void Insert_SL_DL(Champion p) override {
		DoubleNode* newNode = new DoubleNode(p);

		if (head == nullptr) {
			head = tail = newNode;
			tail->next = head;
			head->prev = tail;
		}
		else {
			tail->next = newNode;
			newNode->prev = tail;
			newNode->next = head;
			head->prev = newNode;
			tail = newNode;
		}
	}

};

void loadSingle(SingleLinkedList& list) {
	ifstream file("LOLDic_pr.txt");

	if (!file.is_open()) {
		cerr << "file open fail" << endl;
		return;
	}

	Champion p;
	while (file >> p.position >> p.name >> p.hp >> p.power >> p.defence) {
		list.Insert_SL_DL(p);
	}

	file.close();
}


void loadDouble(DoubleLinkedList& list) {
	ifstream file("LOLDic_pr.txt");

	if (!file.is_open()) {
		cerr << "file open fail" << endl;
		return;
	}

	Champion p;
	while (file >> p.position >> p.name >> p.hp >> p.power >> p.defence) {
		list.Insert_SL_DL(p);
	}

	file.close();
}

int main() {
	DoubleLinkedList list_dl;
	SingleLinkedList list_sl;

	loadSingle(list_sl);
	loadDouble(list_dl);

	LARGE_INTEGER frequency, start, end;

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	list_sl.FindMaxHp_SL_DL();
	QueryPerformanceCounter(&end);
	cout << "싱글 FindMaxHp_SL_DL :" << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	list_dl.FindMaxHp_SL_DL();
	QueryPerformanceCounter(&end);
	cout << "더블 FindMaxHp_SL_DL :" << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;

	Sleep(1000);

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	list_sl.SortByName_SL_DL();
	QueryPerformanceCounter(&end);
	cout << "싱글 SortByName_SL_DL :" << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	list_dl.SortByName_SL_DL();
	QueryPerformanceCounter(&end);
	cout << "더블 SortByName_SL_DL :" << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;

	Sleep(1000);

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	list_sl.PrintAll_SL_DL();
	QueryPerformanceCounter(&end);
	cout << "싱글 PrintAll_SL_DL :" << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	list_dl.PrintAll_SL_DL();
	QueryPerformanceCounter(&end);
	cout << "더블 PrintAll_SL_DL :" << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
	
}