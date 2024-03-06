#include <iostream>
#include <random>
#include <list>
#include <algorithm>
#include <unordered_set>

using namespace std;

void sprint_v(list<int> &random);
void ascending(list<int> &random);
void descending(list<int> &random);
void th3(list<int> &random);
void th5(list<int> &random);
void th35_1(list<int> &random);
void th3A5_1(list<int> &random);
void start1(int num, list<int> &random, list<int> &saveRandom);

int main()
{
    int num{};

    list<int> random(50);
    list<int> saveRandom(50);

    start1(num, random, saveRandom);

    std::cout << "Generatednumbers: ";
    sprint_v(random);

    bool PROGRAM_STATUS = true;

    while (PROGRAM_STATUS)
    {
        char result;
        std::cout << "명령어를 입력하세요 : ";
        cin >> result;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 문자 여러개 입력하는거 방지 (입력된 버퍼 지움)

        switch (result)
        {
        case 'q':
            PROGRAM_STATUS = false;
            break;
        case 'a': // 오름차순
            ascending(random);
            std::cout << "Command: a ";
            sprint_v(random);
            break;
        case 'd': // 내림차순
            descending(random);
            std::cout << "Command: d ";
            sprint_v(random);
            break;
        case 'e': // 3의 배수 더하기
            std::cout << "Command: e ";
            th3(random);
            break;
        case 'f': // 5의 배수 더하기
            std::cout << "Command: f ";
            th5(random);
            break;
        case 'm': // 3, 5 배수 출력, 더하기 1
            std::cout << "Command: m ";
            th35_1(random);
            break;
        case 'n': // 3 & 5 배수 출력 , 빼기 1
            std::cout << "Command: n ";
            th3A5_1(random);
            break;
        case 'r': // 다시 재정렬
            random = saveRandom;
            std::cout << "Command: r ";
            sprint_v(random);
            break;
        case 'p':
            start1(num, random, saveRandom);
            std::cout << "Generatednumbers: ";
            sprint_v(random);
            break;
        default:
            std::cout << "잘못된 입력입니다. " << endl;
            break;
        }
    }
}

void start1(int num, list<int> &random, list<int> &saveRandom)
{
    while (num < 101)
    {
        std::cout << "숫자를 100보다 큰 수로 입력하세요 : ";
        cin >> num;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> uid(1, num - 1);

    for (int &i : random)
    {
        i = uid(gen);
    }

    unordered_set<int> seen; // 이미 나타난 값들을 저장하기 위한 set
    // 중복값 제거
    while (random.size() < 50)
    {
        int new_value = uid(gen); // 새로운 랜덤 값 생성

        // 이미 나타난 값인 경우 다시 생성
        if (seen.find(new_value) != seen.end())
        {
            continue;
        }

        // 새로운 값이 중복이 아니라면 리스트에 추가하고 seen에 추가
        random.push_back(new_value);
        seen.insert(new_value);
    }

    saveRandom = random; // 원본 값 저장.
}

// 3&5의 배수 출력, 빼기 1
void th3A5_1(list<int> &random)
{
    for (int i : random)
    {
        if (i % 3 == 0 && i % 5 == 0)
        {
            std::cout << i << " ";
        }
    }
    std::cout << " -> ";
    for (int i : random)
    {
        if (i % 3 == 0 && i % 5 == 0)
        {
            std::cout << i - 1 << " ";
        }
    }
    std::cout << endl;
}
// 3,5의 배수 출력, 더하기 1
void th35_1(list<int> &random)
{
    for (int i : random)
    {
        if (i % 3 == 0 || i % 5 == 0)
        {
            std::cout << i << " ";
        }
    }
    std::cout << " -> ";
    for (int i : random)
    {
        if (i % 3 == 0 || i % 5 == 0)
        {
            std::cout << i + 1 << " ";
        }
    }
    std::cout << endl;
}

// 5의 배수 출력, 더하기
void th5(list<int> &random)
{
    int result{};
    for (int i : random)
    {
        if (i % 5 == 0)
        {
            std::cout << i << " ";
            result += i;
        }
    }
    std::cout << " -> " << result << endl;
}

// 3의 배수 출력, 더하기
void th3(list<int> &random)
{
    int result{};
    for (int i : random)
    {
        if (i % 3 == 0)
        {
            std::cout << i << " ";
            result += i;
        }
    }
    std::cout << " -> " << result << endl;
}

// 오름차순
void ascending(list<int> &random)
{
    random.sort();
}
// 내림차순
void descending(list<int> &random)
{
    random.sort([](int a, int b)
                { return a > b; });
}

void sprint_v(list<int> &random)
{
    for (int i : random)
    {
        std::cout << i << " ";
    }
    std::cout << endl;
}