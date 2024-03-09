#include <iostream>
#include <iomanip> // setw와 setfill를 사용하기 위해 필요한 헤더 파일
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
   
    bool RESULT_STATUS = false; // 초기 정렬 상태 = false

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
        case 'd': // 열따라 내림차순
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
        case 'e': // 짝수만 출력
            even(result);
            break;
        case 'o': // 홀수만 출력
            oddNumber(result);
            break;
        case 'm': // 최대값을 찾아 모든 값에 더한 값을 출력
            maxPlus(result);
            break;
        case 'n': // 최소값을 찾아 모든 값에서 뺀 값을 출력
            minMinus(result);
            break;
        case 'p': // 한 행의 값을 모두 더해 각 행의 1열에 저장
            pV(result);
            sprint_v(result);
            break;
        case 'u': // 한 열의 값을 모두 곱해 각 열의 4행에 저장
            uV(result);
            break;
        case 'r': // 기존의 숫자를 랜덤하게 재정렬
            rV(result);
            sprint_v(result);
            break;
        case 's': // 새로운 랜덤한 숫자를 다시 자동 입력
            start1(result);
            sprint_v(result);
            break;
        default:
            //잘못된 접근
            break;
        }

    }
}

void rV(int result[][5]) {
    int* flatArr = &result[0][0]; //2차원 배열을 1차원 배열로 전환 시작점을 가르켜서 1차원배열로 가져오는 느낌
    int size = 4 * 5;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> uid(0, 19);

    // suffle 함수를 사용하지 않고 배열을 무작위로 섞기
    for (int i = 0; i < size; ++i) {
        int j = uid(gen); // 0부터 size-1까지의 랜덤한 인덱스 선택
        // 선택한 두 요소를 교환
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
            cout << setw(2) << left << (result[i][j]+maxNum) << " ";
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