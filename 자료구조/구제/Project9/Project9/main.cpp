#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Champion {
	string name;
	int hp;
	int attack;
	int defense;
	int k;
	string position;
};

struct Node {
	Champion champion;
	Node* next;
	Node(Champion p) : champion(p), next(nullptr) {};
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
		}
		else {
			tail->next = newNode;
			tail = newNode;
		}
	}

	void Print_SL() {
		if (head == nullptr) {
			return;
		}

		Node* current = head;
		while (current != nullptr) {
			print_v(current->champion);
			current = current->next;
		}
	}

	void Search_SL(string name) {
		if (head == nullptr) {
			return;
		}

		Node* current = head;
		while (current != nullptr) {
			if (current->champion.name == name) {
				print_v(current->champion);
			}
			current = current->next;
		}
	}

	void Delete_SL(string name) {
		if (head == nullptr) {
			return;
		}

		Node* current = head;
		Node* prev = nullptr;

		if (head->champion.name == name) {
			Node* temp = head;
			head = head->next;
			if (head == nullptr) {
				tail = nullptr;
			}
			delete temp;
			return;
		}
	
		while (current != nullptr && current->champion.name != name) {
			prev = current;
			current = current->next;
		}

		if (current == nullptr) {
			return;
		}

		prev->next = current->next;
		if (current == tail) {
			tail = prev;
		}
		delete current;
	}

	void DeleteAll_SL(string position) {
		if (head == nullptr) {
			return;
		}

		Node* current = head;
		Node* prev = nullptr;

		while (current != nullptr) {
			if (current->champion.position == position) {
				Node* temp = current;
				if (current == head) {
					head = head->next;
					if (head == nullptr) {
						tail = nullptr;
					}
					current = head;
				}
				else {
					prev->next = current->next;
					if (current == tail) {
						tail = prev;
					}
					current = current->next;
				}
				delete temp;
			}
			else {
				prev = current;
				current = current->next;
			}
		}
	}

private:
	void print_v(Champion p) {
		cout << p.position << " " << p.name << " " << p.hp << " " << p.attack << " " << p.defense << endl;
	}
};

int main() {

	Champion c_l[] = {
		{"Jayce",3466,8307,452,73,"mid"},
		{"Vayne",3318,2790,390,60,"support"},
		{"Lee Sin",9864,6257,747,19,"support"},
		{"Teemo",1915,1313,700,68,"mid"},
		{"Nami",2968,2153,803,74,"support"},
		{"Jinx",2583,3042,123,53,"jungle"},
		{"Cassiopeia",6067,7330,683,24,"top"},
		{"Ahri",4573,7079,177,79,"top"},
		{"Fiora",2591,9329,337,81,"jungle"},
		{"Zac",9864,7272,405,49,"bottom"},
		{"Janna",4922,2275,690,66,"jungle"},
		{"Ashe",7421,5549,428,31,"top"},
		{"Twitch",2980,7703,204,91,"mid"},
		{"Ezreal",3660,9835,274,44,"mid"},
		{"Sona",5655,8442,252,26,"bottom"},
		{"Lulu",3231,2014,503,32,"support"},
		{"Amumu",6763,3994,427,19,"jungle"},
		{"Master Yi",5737,7168,434,61,"top"},
		{"Jarvan",1895,3070,323,80,"mid"},
		{"Shen",4794,9220,783,42,"mid"},
		{"Fizz",9864,1913,433,38,"bottom"},
		{"Taric",6158,7451,160,36,"bottom"},
	};

	SingleLinkedList list_sl;

	for (int i = 0; i < sizeof(c_l) / sizeof(c_l[0]); ++i) {
		list_sl.Insert_SL(c_l[i]);
	}

	list_sl.Print_SL();
	cout << "------------------------------" << endl;
	list_sl.Search_SL("Lulu");
	list_sl.Delete_SL("Taric");
	cout << "------------------------------" << endl;
	list_sl.Print_SL();

	list_sl.DeleteAll_SL("mid");
	cout << "------------------------------" << endl;
	list_sl.Print_SL();

}