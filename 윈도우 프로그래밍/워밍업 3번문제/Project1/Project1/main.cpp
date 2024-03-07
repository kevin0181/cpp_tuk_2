#include <iostream>
#include <random>
#include <iomanip> // setw와 setfill를 사용하기 위해 필요한 헤더 파일
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
        cout << "명령어를 입력하세요 : ";
        cin >> order;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 문자 여러개 입력하는거 방지 (입력된 버퍼 지움)

        switch (order)
        {
        case 'q':
            SYSTEM_STATUS = false;
            break;
        case 'a': // 행따라 오름차순
            ascending(result);
            sprint_v(result);
            break;
        case 'd': // 열따라 내림차순
            descending(result);
            sprint_v(result);
            break;
        case 'e': // 짝수만 출력
            break;
        case 'o': // 홀수만 출력
            break;
        case 'm': // 최대값을 찾아 모든 값에 더한 값을 출력
            break;
        case 'n': // 최소값을 찾아 모든 값에서 뺀 값을 출력
            break;
        case 'p': // 한 행의 값을 모두 더해 각 행의 1열에 저장
            break;
        case 'u': // 한 열의 값을 모두 곱해 각 열의 4행에 저장
            break;
        case 'r': // 기존의 숫자를 랜덤하게 재정렬
            break;
        case 's': // 새로운 랜덤한 숫자를 다시 자동 입력
            break;
        default:
            //잘못된 접근
            break;
        }

    }
}

// 행따라 오름차순
void ascending(int result[][5]) {
    int* flatArr = &result[0][0]; //2차원 배열을 1차원 배열로 전환 시작점을 가르켜서 1차원배열로 가져오는 느낌
    int size = 4 * 5;

    sort(flatArr, flatArr + size);

    int k = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            result[i][j] = flatArr[k++];
        }
    }

}
// 열따라 내림차순
void descending(int result[][5]) {

    int* flatArr = &result[0][0]; //2차원 배열을 1차원 배열로 전환 시작점을 가르켜서 1차원배열로 가져오는 느낌
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

// 랜덤한 숫자
void start1(int result[][5])
{

    int num{};
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> uid(1, 50);

    unordered_set<int> seen; // 중복 확인을 위한

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            do
            {
                num = uid(gen);
            } while (seen.count(num) > 0); // 중복된 숫자가 안나올때 까지 반복
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