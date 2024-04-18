#include <iostream>

// ���� ���� ����Ʈ�� ��� ����ü
struct Node {
    int data;       // ��尡 ������ ������
    Node* next;     // ���� ��带 ����Ű�� ������

    // Node ������
    Node(int val) : data(val), next(nullptr) {}
};

// ���� ���� ����Ʈ Ŭ����
class SinglyLinkedList {
public:
    Node* head;  // ����Ʈ�� ù ��带 ����Ű�� ������

    // ������
    SinglyLinkedList() : head(nullptr) {}

    // ����Ʈ�� �� �տ� ��� �߰�
    void insertAtFront(int data) {
        Node* newNode = new Node(data);
        newNode->next = head;
        head = newNode;
    }

    // ����Ʈ ���
    void printList() {
        Node* temp = head;
        while (temp != nullptr) {
            std::cout << temp->data << " -> ";
            temp = temp->next;
        }
        std::cout << "NULL\n";
    }

    // ���� ����Ʈ �޸� ����
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

    // �� ���� ��带 ����Ʈ�� �߰�
    list.insertAtFront(10);
    list.insertAtFront(20);
    list.insertAtFront(30);

    // ����Ʈ ���
    list.printList();

    // ����Ʈ �Ҹ��ڰ� ȣ��Ǹ鼭 �޸𸮰� �ڵ����� �����˴ϴ�.
    return 0;
}
