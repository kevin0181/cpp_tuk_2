#include <iostream>
#include <cstdlib> // rand(), srand() 함수가 정의된 헤더
#include <ctime>   // time() 함수가 정의된 헤더
#include <random>
#include <string>
#include <sstream>

#define MAX_LENGHT 60

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(0, 3);

string changeSpace(string);
string changeWordStar(string, char, string);
int countWords(string);
string changeUpLo(string);
string moveLeft(string);
string moveRight(string);

int main()
{
    bool SYSTEM_STATUS = true;

    srand(static_cast<unsigned int>(time(nullptr)));

    string sentence; // 문장

    std::cout << "문장을 입력하세요 (a~Z) : "; // 왜 std 빼면 모호하다고 뜨지..?
    getline(cin, sentence);

    string save_sentence = sentence;

    if (sentence[sentence.length() - 1] != '.')
    { // 마지막에 .이 없으면 자동으로 입력
        sentence += '.';
    }

    while (SYSTEM_STATUS)
    {

        char result{}; // 명령어 입력

        // 문자열 60자 이상이면 프로그램 멈추기
        if (sentence.length() >= 60)
            SYSTEM_STATUS = false;

        std::cout << "명령어를 입력하세요 : ";
        cin >> result;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 문자 여러개 입력하는거 방지 (입력된 버퍼 지움)

        switch (result)
        {
        case '0':
            // exit
            SYSTEM_STATUS = false;
            break;
        case '1':
            // 공백에 (@,%,#또는.)
            sentence = changeSpace(sentence);
            std::cout << sentence << endl;
            break;
        case '2':
            // 단어 개수 출력
            std::cout << "Result : " << countWords(sentence) << " words" << endl;
            break;
        case '3':
            // lower or upper
            sentence = changeUpLo(sentence);
            std::cout << sentence << endl;
            break;
        case '4':
            // 좌측 한 문자 이동
            sentence = moveLeft(sentence);
            std::cout << sentence << endl;
            break;
        case '5':
            //  우측 한 문자 이동
            sentence = moveRight(sentence);
            std::cout << sentence << endl;
            break;
        default:
            // a~Z까지 입력 시, *로 변경
            sentence = changeWordStar(sentence, result, save_sentence);
            std::cout << sentence << endl;
            break;
        }
    }
}

// 왼쪽으로 1칸 이동
string moveLeft(string sentence)
{
    char temp{};
    for (int i = 0; i < sentence.length() - 2; ++i)
    {
        temp = sentence[i];
        sentence[i] = sentence[i + 1];
        sentence[i + 1] = temp;
    }
    return sentence;
}

// 우측으로 1칸 이동
string moveRight(string sentence)
{
    char temp{};
    for (int i = sentence.length() - 2; i > 0; --i)
    {
        temp = sentence[i];
        sentence[i] = sentence[i - 1];
        sentence[i - 1] = temp;
    }
    return sentence;
}

// 공백 변경
string changeSpace(string sentence)
{
    int randomNumber;
    for (int i = 0; i < sentence.length() - 1; ++i)
    {
        if (sentence[i] == ' ')
        {
            randomNumber = dis(gen);
            switch (randomNumber)
            {
            case 0:
                // @
                sentence[i] = '@';
                break;
            case 1:
                // %
                sentence[i] = '%';
                break;
            case 2:
                // #
                sentence[i] = '#';
                break;
            case 3:
                // .
                sentence[i] = '.';
                break;
            }
        }
        else if (sentence[i] == '@' || sentence[i] == '%' || sentence[i] == '#' || sentence[i] == '.')
        {
            sentence[i] = ' ';
        }
    }
    return sentence;
}

// 단어 개수 출력
int countWords(string sentence)
{
    int cnt{};
    for (int i = 0; i < sentence.length() - 1; ++i)
    {
        if (sentence[i] == ' ' || sentence[i] == '@' || sentence[i] == '%' || sentence[i] == '#' || sentence[i] == '.')
        {
            ++cnt;
        }
    }
    return cnt;
}

// 단어 대문자, 소문자
string changeUpLo(string sentence)
{
    for (auto i = sentence.begin(); i < sentence.end(); ++i)
    {
        if (isupper(*i)) // 만약 대문자면?
        {
            *i = tolower(*i);
        }
        else
        {
            *i = toupper(*i);
        }
    }
    return sentence;
}

// 단어 입력 -> *
string changeWordStar(string sentence, char c, string save_sentence)
{
    bool status = true;
    for (auto i = sentence.begin(); i < sentence.end(); ++i)
    {
        if (*i == c || *i == toupper(c) || *i == tolower(c))
        {
            *i = '*';
            status = false;
        }
    }

    if (status)
    {
        for (int i = 0; i < save_sentence.length(); ++i)
        {
            if (save_sentence[i] == c && sentence[i] == '*' || save_sentence[i] == toupper(c) && sentence[i] == '*' || save_sentence[i] == tolower(c) && sentence[i] == '*')
            {
                sentence[i] = save_sentence[i];
            }
        }
    }

    return sentence;
}
