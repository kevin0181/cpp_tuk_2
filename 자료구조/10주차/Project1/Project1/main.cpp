#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>

using namespace std;

struct Champion {
    string position;
    string name;
    int hp;
    int power;
    int defense;
};

struct Node {
    Champion champion;
    Node* next;
    Node* prev;
    Node(Champion p) : champion(p), next(nullptr), prev(nullptr) {};
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
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        cout << "Inserted: " << p.name << " (" << p.position << ")" << endl;
        cout << "head: " << head->champion.name << ", tail: " << tail->champion.name << endl;
    }

    void PrintAll_DL() {
        if (head == nullptr) {
            cout << "데이터가 존재하지 않습니다." << endl;
            return;
        }

        Node* current = head;
        while (current != nullptr) {
            print_v(current->champion);
            current = current->next;
        }
    }

    void FindMaxHp_DL() {
        if (head == nullptr) {
            cout << "데이터가 존재하지 않습니다." << endl;
            return;
        }

        Node* current = head;
        int maxHp = current->champion.hp;
        while (current != nullptr) {
            if (current->champion.hp > maxHp) {
                maxHp = current->champion.hp;
            }
            current = current->next;
        }

        current = head;

        while (current != nullptr) {
            if (current->champion.hp == maxHp) {
                print_v(current->champion);
            }
            current = current->next;
        }
    }

    void SortByName_DL() {
        if (head == nullptr) {
            cout << "데이터가 존재하지 않습니다." << endl;
            return;
        }

        bool swap_status;
        do {
            swap_status = false;
            Node* current = head;
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
        cout << p.position << " " << p.name << " " << p.hp << " " << p.power << " " << p.defense << endl;
    }

    void swap(Champion& a, Champion& b) {
        Champion temp = a;
        a = b;
        b = temp;
    }
};

class SingleCircularLinkedList {
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
        cout << "Inserted: " << p.name << " (" << p.position << ")" << endl;
        cout << "head: " << head->champion.name << ", tail: " << tail->champion.name << endl;
    }

    void PrintAll_SL() {
        if (head == nullptr) {
            cout << "데이터가 존재하지 않습니다." << endl;
            return;
        }

        Node* current = head;
        do {
            print_v(current->champion);
            current = current->next;
        } while (current != head);
    }

    void FindMaxHp_SL() {
        if (head == nullptr) {
            cout << "데이터가 존재하지 않습니다." << endl;
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
            cout << "데이터가 존재하지 않습니다." << endl;
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

private:

    void print_v(Champion p) {
        cout << p.position << " " << p.name << " " << p.hp << " " << p.power << " " << p.defense << endl;
    }

    void swap(Champion& a, Champion& b) {
        Champion temp = a;
        a = b;
        b = temp;
    }
};

enum ORDER_E {
    SEARCH_E,
    INSERT_E,
    DELETE_E,
    DELETEALL_E,
    PRINTALL_E,
    FINDMAXHP_E,
    SORTBYHP_E,
    NULL_E
};

ORDER_E order_v(string order) {
    if (order == "Search")
        return SEARCH_E;
    if (order == "Insert")
        return INSERT_E;
    if (order == "Delete")
        return DELETE_E;
    if (order == "DeleteAll")
        return DELETEALL_E;
    if (order == "PrintAll")
        return PRINTALL_E;
    if (order == "FindMaxHp")
        return FINDMAXHP_E;
    if (order == "SortByHp")
        return SORTBYHP_E;
    return NULL_E;
}

void loadFile(SingleCircularLinkedList& list) {
    ifstream inputFile("LOLDic_pr.txt");

    if (!inputFile.is_open()) {
        cerr << "파일을 열 수 없습니다." << endl;
        return;
    }

    Champion champion;

    while (inputFile >> champion.position >> champion.name >> champion.hp >> champion.power >> champion.defense) {
        list.Insert_SL(champion);
    }

    inputFile.close();
}

void loadFile2(DoubleLinkedList& list) {
    ifstream inputFile("LOLDic_pr.txt");

    if (!inputFile.is_open()) {
        cerr << "파일을 열 수 없습니다." << endl;
        return;
    }

    Champion champion;

    while (inputFile >> champion.position >> champion.name >> champion.hp >> champion.power >> champion.defense) {
        list.Insert_DL(champion);
    }

    inputFile.close();
}

int main() {
    SingleCircularLinkedList list;
    DoubleLinkedList list_d;

    string order, s1;
    Champion c1;

    loadFile(list);
    loadFile2(list_d);

    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);

    while (true) {
        cout << "-----------------------------" << endl;
        cout << "명령어를 입력해주세요 : ";
        cin >> order;

        if (cin.fail()) {
            cout << "잘못된 입력값 입니다." << endl;
            cin.clear();
            break;
        }

        switch (order_v(order))
        {
        case PRINTALL_E:
            // 시간 측정을 위해 start와 end 시점을 기록합니다.
            QueryPerformanceCounter(&start);
            list.PrintAll_SL(); // printAll f
            QueryPerformanceCounter(&end);
            cout << "SingleCircularLinkedList PrintAll 수행 시간: " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << " 마이크로초" << endl;

            QueryPerformanceCounter(&start);
            list_d.PrintAll_DL(); // printAll f
            QueryPerformanceCounter(&end);
            cout << "DoubleLinkedList PrintAll 수행 시간: " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << " 마이크로초" << endl;

            break;
        case FINDMAXHP_E:
            QueryPerformanceCounter(&start);
            list.FindMaxHp_SL(); // findMaxHp f
            QueryPerformanceCounter(&end);
            cout << "SingleCircularLinkedList FindMaxHp 수행 시간: " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << " 마이크로초" << endl;

            QueryPerformanceCounter(&start);
            list_d.FindMaxHp_DL(); // findMaxHp f
            QueryPerformanceCounter(&end);
            cout << "DoubleLinkedList FindMaxHp 수행 시간: " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << " 마이크로초" << endl;

            break;
        case SORTBYHP_E:
            QueryPerformanceCounter(&start);
            list.SortByName_SL(); // sortByHp f
            QueryPerformanceCounter(&end);
            cout << "SingleCircularLinkedList SortByName 수행 시간: " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << " 마이크로초" << endl;

            QueryPerformanceCounter(&start);
            list_d.SortByName_DL(); // sortByHp f
            QueryPerformanceCounter(&end);
            cout << "DoubleLinkedList SortByName 수행 시간: " << (end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart << " 마이크로초" << endl;

            cout << "이름 내림차순으로 정렬되었습니다." << endl;

            break;
        default:
            cout << "잘못된 명령어 입니다." << endl;
            break;
        }
    }
}
