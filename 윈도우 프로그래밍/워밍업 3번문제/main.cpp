#include <iostream>
#include <random>
#include <iomanip> // setw와 setfill를 사용하기 위해 필요한 헤더 파일
#include <unordered_set>

using namespace std;

void start1(int[][5]);
void sprint_v(int[][5]);
void dupCheck(int result[][5]);

int main()
{
    int result[4][5];
    start1(result);
    sprint_v(result);
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
            } while (seen.count(num) > 0);
            // result[i][j] = uid(gen);
        }
    }
}

void sprint_v(int result[][5])
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            cout << setw(2) << right << result[i][j] << " ";
        }
        cout << endl;
    }
}