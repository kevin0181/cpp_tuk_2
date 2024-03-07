#include <iostream>
#include <iomanip> // setw�� setfill�� ����ϱ� ���� �ʿ��� ��� ����
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <random>

using namespace std;

void start1(int[][5]);
void sprint_v(int[][5]);
void copyArray(int result[][5], int save_result[][5]);
void ascending(int result[][5]);
void descending(int result[][5]);
void even(int result[][5]);
void oddNumber(int result[][5]);
void maxPlus(int result[][5]);
void minMinus(int result[][5]);
void pV(int result[][5]);
void uV(int result[][5]);
void rV(int result[][5]);

int main()
{
    int result[4][5];
    int save_result[4][5];
    start1(result);
    sprint_v(result);
    bool SYSTEM_STATUS = true;

    bool RESULT_STATUS = false; // �ʱ� ���� ���� = false

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
            if (RESULT_STATUS) {
                copyArray(save_result, result);
                RESULT_STATUS = false;
            }
            else {
                ascending(result);
                RESULT_STATUS = true;
            }
            sprint_v(result);
            break;
        case 'd': // ������ ��������
            if (RESULT_STATUS) {
                copyArray(save_result, result);
                RESULT_STATUS = false;
            }
            else {
                descending(result);
                RESULT_STATUS = true;
            }
            sprint_v(result);
            break;
        case 'e': // ¦���� ���
            even(result);
            break;
        case 'o': // Ȧ���� ���
            oddNumber(result);
            break;
        case 'm': // �ִ밪�� ã�� ��� ���� ���� ���� ���
            maxPlus(result);
            break;
        case 'n': // �ּҰ��� ã�� ��� ������ �� ���� ���
            minMinus(result);
            break;
        case 'p': // �� ���� ���� ��� ���� �� ���� 1���� ����
            pV(result);
            sprint_v(result);
            break;
        case 'u': // �� ���� ���� ��� ���� �� ���� 4�࿡ ����
            uV(result);
            break;
        case 'r': // ������ ���ڸ� �����ϰ� ������
            rV(result);
            sprint_v(result);
            break;
        case 's': // ���ο� ������ ���ڸ� �ٽ� �ڵ� �Է�
            start1(result);
            sprint_v(result);
            break;
        default:
            //�߸��� ����
            break;
        }

    }
}

void rV(int result[][5]) {
    int* flatArr = &result[0][0]; //2���� �迭�� 1���� �迭�� ��ȯ �������� �����Ѽ� 1�����迭�� �������� ����
    int size = 4 * 5;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> uid(0, 19);

    // suffle �Լ��� ������� �ʰ� �迭�� �������� ����
    for (int i = 0; i < size; ++i) {
        int j = uid(gen); // 0���� size-1������ ������ �ε��� ����
        // ������ �� ��Ҹ� ��ȯ
        int temp = flatArr[i];
        flatArr[i] = flatArr[j];
        flatArr[j] = temp;
    }

}

void uV(int result[][5]) {
    int lineNum = 1;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            lineNum *= result[j][i];
        }
        result[3][i] = lineNum;
        lineNum = 1;
    }

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            cout << setw(10) << left << result[i][j] << " ";
        }
        cout << endl;
    }
}


void pV(int result[][5]) {
    int lineNum{};
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            lineNum += result[i][j];
        }
        result[i][0] = lineNum;
        lineNum = 0;
    }
}

void minMinus(int result[][5]) {
    int minNum = result[0][0];

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (minNum > result[i][j])
                minNum = result[i][j];
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            cout << setw(2) << left << (result[i][j] - minNum) << " ";
        }
        cout << endl;
    }
}

void maxPlus(int result[][5]) {

    int maxNum{};

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (maxNum < result[i][j])
                maxNum = result[i][j];
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            cout << setw(2) << left << (result[i][j] + maxNum) << " ";
        }
        cout << endl;
    }

}

void oddNumber(int result[][5]) {
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (result[i][j] % 2 == 0) {
                cout << setw(2) << left << "0" << " ";
            }
            else {
                cout << setw(2) << left << result[i][j] << " ";
            }
        }
        cout << endl;
    }
}

void even(int result[][5]) {
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (result[i][j] % 2 == 0) {
                cout << setw(2) << left << result[i][j] << " ";
            }
            else {
                cout << setw(2) << left << "0" << " ";
            }
        }
        cout << endl;
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

void copyArray(int result[][5], int save_result[][5]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            save_result[i][j] = result[i][j];
        }
    }
}