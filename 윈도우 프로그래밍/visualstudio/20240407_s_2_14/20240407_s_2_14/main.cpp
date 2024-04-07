#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"�ǽ� 2-13��";
LPCTSTR lpszWindowName = L"�ǽ� 2-13��";

#define WIDTH 1200
#define HEIGHT 860

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {

    HWND hWnd;
    MSG Message;
    WNDCLASSEX WndClass;
    g_hInst = hInstance;

    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = (WNDPROC)WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_QUESTION);
    WndClass.hCursor = LoadCursor(NULL, IDC_HAND);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_QUESTION);
    RegisterClassEx(&WndClass);

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_VSCROLL, 0, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return Message.wParam;

}

// ����: �ٵ��� ĭ�� ũ�⸦ 20x20 �ȼ��� ����
#define cellSize 40
#define gridSize 20

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uid_position(0, 19);
uniform_int_distribution<int> uid_ranSize(10, 30);
uniform_int_distribution<> uid_ranAlpha('a', 'z');
uniform_int_distribution<int> uid_num(0, 9);

struct Word_s {
    vector<TCHAR> w;
    int x;
    int y;
};
class Shape {
public:
    POINT sizePoint[6];
    int positionX{};
    int positionY{};
    COLORREF color = RGB(255, 0, 0); //��Ȳ
    COLORREF bolderColor = RGB(255, 255, 255);
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
    char shape_c;
    char original_c;
    bool mini_status = false; // false = ū, true = ����

    void print_(HDC hDC) {
        hBrush = CreateSolidBrush(color);
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        hPen = CreatePen(PS_SOLID, 2, bolderColor);
        oldPen = (HPEN)SelectObject(hDC, hPen);

        Ellipse(hDC, 0 + (positionX)*cellSize + 5, 0 + (positionY)*cellSize + 5, 35 + (positionX)*cellSize, 35 + (positionY)*cellSize);

        SelectObject(hDC, oldBrush);
        DeleteObject(hBrush);
        DeleteObject(hPen);
    }

};

void randChar(vector<Word_s>& words, int status);
char addCharacters(char ch1, char ch2);
char addCharAndNumber(char ch, int num);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC;
    HPEN hPen, oldPen;
    HBRUSH hBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static Shape myShape;

    static vector<Word_s> words;

    int position_arrow;

    Word_s wo;
    static int mode_status;
    switch (uMsg)
    {
    case WM_CREATE:
    {
        myShape.positionX = 0;
        myShape.positionY = 0;
    }
    break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:

        hDC = BeginPaint(hWnd, &ps);

        switch (wParam) // myShape �̵�
        {
        case VK_LEFT:
            myShape.positionX -= 1;
            if (myShape.positionX == -1) {
                myShape.positionX = 19;
            }
            position_arrow = 0;
            break;
        case VK_RIGHT:
            myShape.positionX += 1;
            if (myShape.positionX == 20) {
                myShape.positionX = 0;
            }
            position_arrow = 1;
            break;
        case VK_UP:
            myShape.positionY -= 1;
            if (myShape.positionY == -1) {
                myShape.positionY = 19;
            }
            position_arrow = 2;
            break;
        case VK_DOWN:
            myShape.positionY += 1;
            if (myShape.positionY == 20) {
                myShape.positionY = 0;
            }
            position_arrow = 3;
            break;
        default:
            break;
        }

        int change_i;
        for (int i = 0; i < words.size(); ++i) {
            if (words[i].x == myShape.positionX && words[i].y == myShape.positionY) {
                switch (position_arrow)
                {
                case 0:
                    words[i].x -= 1;
                    if (words[i].x == -1) {
                        words[i].x = 19;
                    }
                    break;
                case 1:
                    words[i].x += 1;
                    if (words[i].x == 20) {
                        words[i].x = 0;
                    }
                    break;
                case 2:
                    words[i].y -= 1;
                    if (words[i].y == -1) {
                        words[i].y = 19;
                    }
                    break;
                case 3:
                    words[i].y += 1;
                    if (words[i].y == 20) {
                        words[i].y = 0;
                    }
                    break;
                default:
                    break;
                }
                wo = words[i];
                change_i = i;
            }
        }



        switch (mode_status)
        {
        case 1:
            for (int i = 0; i < words.size(); ++i) {
                if (wo.x == words[i].x && wo.y == words[i].y && change_i != i) {

                    int sum = stoi(wstring(wo.w.begin(), wo.w.end())) + stoi(wstring(words[i].w.begin(), words[i].w.end()));

                    wo.w.clear();

                    for (auto ch : to_wstring(sum)) {
                        wo.w.push_back(ch);
                    }

                    words[i] = wo;
                    words.erase(words.begin() + change_i);
                }
            }
            break;
        default:
            for (int i = 0; i < words.size(); ++i) {
                if (wo.x == words[i].x && wo.y == words[i].y && change_i != i) {
                    int num1, num2, sum;
                    if (std::isalpha(wo.w[0]) && std::isalpha(words[i].w[0])) {
                        // �� �� ��� ������ ��
                        num1 = wo.w[0] - 'a' + 1;
                        num2 = words[i].w[0] - 'a' + 1;
                        sum = (num1 + num2 - 1) % 26 + 1;
                        words[i].w[0] = 'a' + sum - 1;
                    }
                    else if (std::isalpha(wo.w[0]) && std::isdigit(words[i].w[0])) {
                        // wo.w�� �����̰� words[i].w�� ������ ��
                        num1 = wo.w[0] - 'a' + 1;
                        num2 = words[i].w[0] - '0'; // ASCII '0'���� '9'���� 1���� 10���� ����
                        sum = (num1 + num2 - 1) % 26 + 1;
                        words[i].w[0] = 'a' + sum - 1;
                    }
                    else if (std::isdigit(wo.w[0]) && std::isalpha(words[i].w[0])) {
                        // wo.w�� �����̰� words[i].w�� ������ ��
                        num1 = wo.w[0] - '0';
                        num2 = words[i].w[0] - 'a' + 1;
                        sum = (num1 + num2 - 1) % 26 + 1;
                        words[i].w[0] = 'a' + sum - 1;
                    }
                    else {
                        // �� �� ��� ������ ��
                        num1 = wo.w[0] - '0';
                        num2 = words[i].w[0] - '0';
                        sum = (num1 + num2) % 10;
                        words[i].w[0] = '0' + sum;
                    }

                    words.erase(words.begin() + change_i);
                    break; // ��ġ�ϴ� ù ��° ��Ҹ� ó���� �� �ݺ� ����
                }
            }
            break;
        }

        InvalidateRect(hWnd, NULL, TRUE);

        EndPaint(hWnd, &ps);
        break;
    case WM_CHAR:

        switch (wParam)
        {
        case 'q':
            PostQuitMessage(0);
            break;
        case's':
            break;
        case '1':
        {
            words.clear();
            myShape.positionX = 0;
            myShape.positionY = 0;

            mode_status = 1;

            int rand2 = uid_ranSize(gen);

            for (int i = 0; i < rand2; ++i) {
                randChar(words, 0);
            }
            break;
        }
        case '2':
        {
            words.clear();
            myShape.positionX = 0;
            myShape.positionY = 0;
            mode_status = 2;
            int rand2 = uid_ranSize(gen);

            for (int i = 0; i < rand2; ++i) {
                randChar(words, 1);
            }
            break;
        }
        case '3':
        {
            words.clear();
            myShape.positionX = 0;
            myShape.positionY = 0;

            mode_status = 3;

            int rand2 = uid_ranSize(gen);

            for (int i = 0; i < rand2; ++i) {
                randChar(words, 2);
            }
            break;
        }
        default:
            break;
        }

        InvalidateRect(hWnd, NULL, TRUE);

        break;
    case WM_SIZE:
        break;
    case WM_PAINT:
    {
        hDC = BeginPaint(hWnd, &ps);

        // �ٵ��� �� �׸���
        for (int y = 0; y <= gridSize; ++y) {
            MoveToEx(hDC, y * cellSize, 0, NULL);
            LineTo(hDC, y * cellSize, gridSize * cellSize);
        }

        for (int x = 0; x <= gridSize; ++x) {
            MoveToEx(hDC, 0, x * cellSize, NULL);
            LineTo(hDC, gridSize * cellSize, x * cellSize);
        }

        for (auto& word : words) {
            SIZE textSize;
            GetTextExtentPoint32(hDC, word.w.data(), 1, &textSize); // ������ ũ�⸦ ������
            int letterX = (cellSize - textSize.cx) / 2 + (cellSize * word.x); // ��� ������ ���� X ��ǥ ���
            int letterY = (cellSize - textSize.cy) / 2 + (cellSize * word.y); // ��� ������ ���� Y ��ǥ ���
            TextOut(hDC, letterX, letterY, word.w.data(), word.w.size()); // ���� ���
        }

        myShape.print_(hDC);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void randChar(vector<Word_s>& words, int status) {

    int p_x;
    int p_y;

    p_x = uid_position(gen);
    p_y = uid_position(gen);

    if (p_x == 0 && p_y == 0) {
        return;
    }
    Word_s word;
    switch (status)
    {
    case 0: // ���ڸ�
        word.w.push_back(uid_num(gen) + '0');
        break;
    case 1: // ���
        word.w.push_back(uid_ranAlpha(gen));
        break;
    case 2: // �Ѵ�
    {
        uniform_int_distribution<int> uid_ii(0, 1);
        if (uid_ii(gen) == 0) {
            word.w.push_back(uid_num(gen) + '0');
        }
        else {
            word.w.push_back(uid_ranAlpha(gen));
        }
        break;
    }
    default:
        break;
    }

    word.x = p_x;
    word.y = p_y;

    words.push_back(word);
}// ASCII ���� ����Ͽ� ���ڸ� ���ϴ� �Լ�
char addCharacters(char ch1, char ch2) {
    // �� ���ڰ� ��� ���ĺ� �ҹ������� Ȯ��
    if ((ch1 >= 'a' && ch1 <= 'z') && (ch2 >= 'a' && ch2 <= 'z')) {
        int offset = (ch1 - 'a' + ch2 - 'a' + 1) % 26;
        return 'a' + offset;
    }
    return ch1; // �⺻ ��ȯ��, ���� ��Ȳ ó���� �ʿ��� �� ����
}

char addCharAndNumber(char ch, int num) {
    if (ch >= 'a' && ch <= 'z') {
        int offset = (ch - 'a' + num) % 26;
        if (offset < 0) {
            offset += 26; // ���� ����
        }
        // ��������� char�� ĳ�����Ͽ� ��� ��ȯ ���� ����
        return static_cast<char>('a' + offset);
    }
    return ch;
}