#include <iostream>
#include <random>
#include <iomanip> // setw�� setfill�� ����ϱ� ���� �ʿ��� ��� ����
#include <unordered_set>
#include <algorithm>

using namespace std;

void start1(int[][5]);
void sprint_v(int[][5]);
void copyArray(int result[][5], int save_result[][5]);
void ascending(int result[][5]);
void descending(int result[][5]);

int main()
{
    int result[4][5];
    int save_result[4][5];
    start1(result);
    sprint_v(result);
    bool SYSTEM_STATUS = true;

    copyArray(result, save_result);

    while (SYSTEM_STATUS) {

        char order{};
        cout << "��ɾ �Է��ϼ��� : ";
        cin >> order;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ���� ������ �Է��ϴ°� ���� (�Էµ� ���� ����)

        switch (order)
        {
        case 'q':
            SYSTEM_STATUS = false;
            break;
        case 'a': // ����� ��������
            ascending(result);
            sprint_v(result);
            break;
        case 'd': // ������ ��������
            descending(result);
            sprint_v(result);
            break;
        case 'e': // ¦���� ���
            break;
        case 'o': // Ȧ���� ���
            break;
        case 'm': // �ִ밪�� ã�� ��� ���� ���� ���� ���
            break;
        case 'n': // �ּҰ��� ã�� ��� ������ �� ���� ���
            break;
        case 'p': // �� ���� ���� ��� ���� �� ���� 1���� ����
            break;
        case 'u': // �� ���� ���� ��� ���� �� ���� 4�࿡ ����
            break;
        case 'r': // ������ ���ڸ� �����ϰ� ������
            break;
        case 's': // ���ο� ������ ���ڸ� �ٽ� �ڵ� �Է�
            break;
        default:
            //�߸��� ����
            break;
        }

    }
}

// ����� ��������
void ascending(int result[][5]) {
    int* flatArr = &result[0][0]; //2���� �迭�� 1���� �迭�� ��ȯ �������� �����Ѽ� 1�����迭�� �������� ����
    int size = 4 * 5;

    sort(flatArr, flatArr + size);

    int k = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            result[i][j] = flatArr[k++];
        }
    }

}
// ������ ��������
void descending(int result[][5]) {

    int* flatArr = &result[0][0]; //2���� �迭�� 1���� �迭�� ��ȯ �������� �����Ѽ� 1�����迭�� �������� ����
    int size = 4 * 5;

    sort(flatArr, flatArr + size);
    int saveArr[20]{};

    int k = 0;
    for (int i = 0; i < size; ++i) {
        saveArr[i] = flatArr[k++];
    }

    k = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 3; j >= 0; --j) {
            result[j][i] = saveArr[k++];
        }
    }
}

// ������ ����
void start1(int result[][5])
{

    int num{};
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> uid(1, 50);

    unordered_set<int> seen; // �ߺ� Ȯ���� ����

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            do
            {
                num = uid(gen);
            } while (seen.count(num) > 0); // �ߺ��� ���ڰ� �ȳ��ö� ���� �ݺ�
            result[i][j] = num;
            seen.insert(num);
        }
    }
}

void sprint_v(int result[][5])
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            cout << setw(2) << left << result[i][j] << " ";
        }
        cout << endl;
    }
}

void copyArray(int result[][5],int save_result[][5]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            save_result[i][j] = result[i][j];
        }
    }
}