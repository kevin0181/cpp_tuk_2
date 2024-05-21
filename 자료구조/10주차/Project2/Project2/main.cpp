#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>

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

	void FindMaxHp_SL() {
		if (head == nullptr) {
			cout << "no data" << endl;
			return;
		}

		SingleNode* current = head;
		int maxHp = current->champion.hp;

		do {
			if (current->champion.hp > maxHp) {
				maxHp = current->champion.hp;
			}
			current = current->next;
		} while (current != head);

		current = head;
		do {
			if (current->champion.hp == maxHp) {
				print_v(current->champion);
			}
			current = current->next;
		} while (current != head);
	}

	void SortByName_SL() {
		if (head == nullptr) {
			cout << "no data" << endl;
			return;
		}

		bool swap_status;

		do {
			swap_status = false;
			SingleNode* current = head;
			SingleNode* next = head->next;
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

private:
	void print_v(Champion p) {
		cout << p.position << " " << p.name << " " << p.hp << " " << p.power << " " << p.defence << endl;
	}
	
	void swap(Champion& a, Champion& b) {
		Champion temp = a;
		a = b;
		b = temp;
	}
};

class DoubleLinkedList {
private:
	DoubleNode* head = nullptr;
	DoubleNode* tail = nullptr;
public:

	void Insert_DL(Champion p) {
		DoubleNode* newNode = new DoubleNode(p);

		if (head == nullptr) {
			head = tail = newNode;
			//tail->next = head;
		}
		else {
			tail->next = newNode;
			newNode->prev = tail;
			tail = newNode;
		}
	}

	void PrintAll_DL() {
		if (head == nullptr) {
			cout << "no data" << endl;
			return;
		}

		DoubleNode* current = head;
		while (current != nullptr) {
			print_v(current->champion);
			current = current->next;
		}
	}

	void FindMaxHp_DL() {
		if (head == nullptr) {
			cout << "no data" << endl;
			return;
		}

		DoubleNode* current = head;
		int maxHp = current->champion.hp;
		while (current != nullptr) {
			if (current->champion.hp > maxHp) {
				maxHp = current->champion.hp;
			}
			current = current->next;
		}

		current = head;

		while (current != nullptr) {
			if (maxHp == current->champion.hp) {
				print_v(current->champion);
			}

			current = current->next;
		}
	}

	void SortByName_DL() {
		if (head == nullptr) {
			cout << "no data" << endl;
			return;
		}

		bool swap_status;
		do {
			swap_status = false;
			DoubleNode* current = head;
			while (current->next != nullptr) {
				if (current->champion.name < current->next->champion.name) {
					swap(current->champion, current->next->champion);
					swap_status = true;
				}
				current = current->next;
			}

		} while (swap_status);
	}

private:
	void print_v(Champion p) {
		cout << p.position << " " << p.name << " " << p.hp << " " << p.power << " " << p.defence << endl;
	}

	void swap(Champion& a, Champion& b) {
		Champion temp = a;
		a = b;
		b = temp;
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
		list.Insert_SL(p);
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
		list.Insert_DL(p);
	}

	file.close();
}

int main(){

	SingleLinkedList singleLinkedList;
	DoubleLinkedList doubleLinkedList;

	loadSingle(singleLinkedList);
	loadDouble(doubleLinkedList);

	LARGE_INTEGER frequency, start, end;
	QueryPerformanceFrequency(&frequency);

	QueryPerformanceCounter(&start);
	singleLinkedList.FindMaxHp_SL();
	QueryPerformanceCounter(&end);
	cout << " single linked list 수행 시간 : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
	QueryPerformanceCounter(&start);
	doubleLinkedList.FindMaxHp_DL();
	QueryPerformanceCounter(&end);
	cout << " double linked list 수행 시간 : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;

	Sleep(1000);

	QueryPerformanceCounter(&start);
	singleLinkedList.SortByName_SL();
	QueryPerformanceCounter(&end);
	cout << " single linked list 수행 시간 : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
	QueryPerformanceCounter(&start);
	doubleLinkedList.SortByName_DL();
	QueryPerformanceCounter(&end);
	cout << " double linked list 수행 시간 : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;

	Sleep(1000);

	QueryPerformanceCounter(&start);
	singleLinkedList.PrintAll_SL();
	QueryPerformanceCounter(&end);
	cout << " single linked list 수행 시간 : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
	QueryPerformanceCounter(&start);
	doubleLinkedList.PrintAll_DL();
	QueryPerformanceCounter(&end);
	cout << " double linked list 수행 시간 : " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << "마이크로초" << endl;
}