#include <iostream>
#define MAX_LENGHT 60

using namespace std;

int main()
{
    bool SYSTEM_STATUS = true;

    while (SYSTEM_STATUS)
    {

        char num{};      // 명령어 입력
        string sentence; // 문장 입력

        cout << "문장을 입력하세요 (a~Z) : ";
        cin >> sentence;

        // 문자열 60자 이상이면 프로그램 멈추기
        if (sentence.length() >= 60)
            SYSTEM_STATUS = false;

        cout << "0 ~ 5까지의 명령어를 입력하세요 : ";
        cin >> num;

        switch (num)
        {
        case '0':
            // exit
            SYSTEM_STATUS = false;
            break;
        case '1':
            // 공백에 (@,%,#또는.)
            break;
        case '2':
            // 단어 개수 출력
            break;
        case '3':
            // lower or upper
            break;
        case '4':
            // 좌측 한 문자 이동
            break;
        case '5':
            //  우측 한 문자 이동
            break;
        default:
            // a~Z까지 입력 시, *로 변경
            break;
        }
    }
}

// 공백 변경
string changeSpace(string s)
{
}

// 단어 개수 출력
int countWords(string s)
{
}

//단어 대문자, 소문자
string changeUpLo(string s)
{
}