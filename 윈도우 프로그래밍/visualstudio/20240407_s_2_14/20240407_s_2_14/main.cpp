#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 2-13번";
LPCTSTR lpszWindowName = L"실습 2-13번";

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

// 설정: 바둑판 칸의 크기를 20x20 픽셀로 정의
#define cellSize 40
#define gridSize 20

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uid_position(0, 19);
uniform_int_distribution<int> uid_ranSize(10, 30);
uniform_int_distribution<> uid_ranAlpha('A', 'z');
uniform_int_distribution<int> uid_num(0, 9);

struct Word_s {
    TCHAR w;
    int x;
    int y;
};
class Shape {
public:
    POINT sizePoint[6];
    int positionX{};
    int positionY{};
    COLORREF color = RGB(255, 0, 0); //주황
    COLORREF bolderColor = RGB(255, 255, 255);
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
    char shape_c;
    char original_c;
    bool mini_status = false; // false = 큰, true = 작은

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

        switch (wParam) // myShape 이동
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

        Word_s wo;
        int change_i;
        for (int i = 0; i < words.size();++i) {
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

        /*for (int i = 0; i < words.size(); ++i) {
            if (wo.x == words[i].x && wo.y == words[i].y && change_i != i) {
                wo.w = (wo.w - '0') + (words[i].w - '0') + '0';
                words[i] = wo;
                words.erase(words.begin() + change_i);
            }
        }*/

        for (int i = 0; i < words.size(); ++i) {
            if (wo.x == words[i].x && wo.y == words[i].y && change_i != i) {
                wo.w = (wo.w - '0') + (words[i].w - '0') + '0';
                words[i] = wo;
                words.erase(words.begin() + change_i);
            }
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

            int rand2 = uid_ranSize(gen);

            for (int i = 0; i < rand2; ++i) {
                randChar(words, 0);
            }
            break;
        }
        case '2':
        {
            words.clear();

            int rand2 = uid_ranSize(gen);

            for (int i = 0; i < rand2; ++i) {
                randChar(words, 1);
            }
            break;
        }
        case '3':
        {
            words.clear();

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

        // 바둑판 선 그리기
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
            GetTextExtentPoint32(hDC, &word.w, 1, &textSize); // 글자의 크기를 가져옴
            int letterX = (cellSize - textSize.cx) / 2 + (cellSize * word.x); // 가운데 정렬을 위해 X 좌표 계산
            int letterY = (cellSize - textSize.cy) / 2 + (cellSize * word.y); // 가운데 정렬을 위해 Y 좌표 계산
            TextOut(hDC, letterX, letterY, &word.w, 1); // 글자 출력
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
    case 0: // 숫자만
        word.w = uid_num(gen) + '0';
        break;
    case 1: // 영어만
        word.w = uid_ranAlpha(gen);
        break;
    case 2: // 둘다
    {
        uniform_int_distribution<int> uid_ii(0, 1);
        if (uid_ii(gen) == 0) {
            word.w = uid_num(gen) + '0';
        }
        else {
            word.w = uid_ranAlpha(gen);
        }
        break;
    }
    default:
        break;
    }
    
    word.x = p_x;
    word.y = p_y;

    words.push_back(word);
}