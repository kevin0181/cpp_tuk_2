#include <iostream>
#include <cstdlib> // rand(), srand() �Լ��� ���ǵ� ���
#include <ctime>   // time() �Լ��� ���ǵ� ���
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

int main()
{
    bool SYSTEM_STATUS = true;

    srand(static_cast <unsigned int>(time(nullptr)));

    string sentence; // ����

    std::cout << "������ �Է��ϼ��� (a~Z) : "; //�� std ���� ��ȣ�ϴٰ� ����..?
    getline(cin, sentence);

    string save_sentence = sentence;
    
    if (sentence[sentence.length() - 1] != '.') { // �������� .�� ������ �ڵ����� �Է�
        sentence += '.';
    }

    while (SYSTEM_STATUS)
    {


        char result{};      // ��ɾ� �Է�

        // ���ڿ� 60�� �̻��̸� ���α׷� ���߱�
        if (sentence.length() >= 60)
            SYSTEM_STATUS = false;

        std::cout << "��ɾ �Է��ϼ��� : ";
        cin >> result;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ���� ������ �Է��ϴ°� ���� (�Էµ� ���� ����)

        switch (result)
        {
        case '0':
            // exit
            SYSTEM_STATUS = false;
            break;
        case '1':
            // ���鿡 (@,%,#�Ǵ�.)
            sentence = changeSpace(sentence);
            std::cout << sentence << endl;
            break;
        case '2':
            // �ܾ� ���� ���
            std::cout << "Result : " << countWords(sentence) << " words" << endl;
            break;
        case '3':
            // lower or upper
            std::cout << sentence << endl;
            break;
        case '4':
            // ���� �� ���� �̵�
            std::cout << sentence << endl;
            break;
        case '5':
            //  ���� �� ���� �̵�
            std::cout << sentence << endl;
            break;
        default:
            // a~Z���� �Է� ��, *�� ����
            sentence = changeWordStar(sentence, result, save_sentence);
            std::cout << sentence << endl;
            break;
        }
      
    }
}

// ���� ����
string changeSpace(string sentence)
{
    int randomNumber;
    for (int i = 0; i < sentence.length(); ++i) {
        if (sentence[i] == ' ') {
            randomNumber = dis(gen);
            switch (randomNumber) {
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
    }
    return sentence;
}

// �ܾ� ���� ���
int countWords(string sentence)
{
    int cnt{};
    for (int i = 0; i < sentence.length() - 1; ++i) {
        if (sentence[i] == ' ' || sentence[i] == '@' || sentence[i] == '%'|| sentence[i] == '#'|| sentence[i] == '.') {
            ++cnt;
        }
    }
    return cnt;
}

//�ܾ� �빮��, �ҹ���
string changeUpLo(string s)
{
    return "";
}

// �ܾ� �Է� -> *
string changeWordStar(string sentence, char c, string save_sentence) {
    bool status = true;
    for (auto i = sentence.begin(); i < sentence.end(); ++i) {
        if (*i == c || *i == toupper(c) || *i == tolower(c)) {
            *i = '*';
            status = false;
        }
    }

    if (status) {
        for (int i = 0; i < save_sentence.length(); ++i) {
            if (save_sentence[i] == c && sentence[i] == '*' || save_sentence[i] == toupper(c) && sentence[i] == '*' || save_sentence[i] == tolower(c) && sentence[i] == '*') {
                sentence[i] = save_sentence[i];
            }
        }
    }

    return sentence;
}