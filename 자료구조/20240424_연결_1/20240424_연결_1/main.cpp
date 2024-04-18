#include <iostream>

// 단일 연결 리스트의 노드 구조체
struct Node {
    int data;       // 노드가 저장할 데이터
    Node* next;     // 다음 노드를 가리키는 포인터

    // Node 생성자
    Node(int val) : data(val), next(nullptr) {}
};

// 단일 연결 리스트 클래스
class SinglyLinkedList {
public:
    Node* head;  // 리스트의 첫 노드를 가리키는 포인터

    // 생성자
    SinglyLinkedList() : head(nullptr) {}

    // 리스트의 맨 앞에 노드 추가
    void insertAtFront(int data) {
        Node* newNode = new Node(data);
        newNode->next = head;
        head = newNode;
    }

    // 리스트 출력
    void printList() {
        Node* temp = head;
        while (temp != nullptr) {
            std::cout << temp->data << " -> ";
            temp = temp->next;
        }
        std::cout << "NULL\n";
    }

    // 연결 리스트 메모리 해제
    ~SinglyLinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        std::cout << "List destroyed.\n";
    }
};

int main() {
    SinglyLinkedList list;

    // 몇 개의 노드를 리스트에 추가
    list.insertAtFront(10);
    list.insertAtFront(20);
    list.insertAtFront(30);

    // 리스트 출력
    list.printList();

    // 리스트 소멸자가 호출되면서 메모리가 자동으로 해제됩니다.
    return 0;
}
