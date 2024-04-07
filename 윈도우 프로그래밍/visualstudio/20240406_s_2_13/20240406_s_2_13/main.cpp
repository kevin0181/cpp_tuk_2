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
uniform_int_distribution<int> uid_getWord(0, 5);
uniform_int_distribution<int> uid_position(0, 19);
uniform_int_distribution<int> uid_ranSize(10, 30);

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

void randChar(TCHAR* selectWord, vector<Word_s>& words);
void start(TCHAR* selectWord, HDC hDC, int rand_i);
void insertWord(TCHAR ch_1, vector<Word_s>& words);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC;
    HPEN hPen, oldPen;
    HBRUSH hBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static Shape myShape;
    static TCHAR word[6][100] = {
         _T("APPLE"),
         _T("BANANA"),
         _T("CHERRY"),
         _T("Hello, World!"),
         _T("Goodbye!"),
         _T("Win32 API.")
    };

    static TCHAR *selectWord;
    static vector<Word_s> words;
    static  int rand_i;

    int position_arrow;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        selectWord = word[uid_getWord(gen)];

        uniform_int_distribution<int> uid_ri(0, lstrlen(selectWord) - 1);
        rand_i = uid_ri(gen);

        int rand2 = uid_ranSize(gen);

        insertWord(selectWord[rand_i], words);

        for (int i = 0; i < rand2; ++i) {
            randChar(selectWord, words);
        }

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

        for (auto& word : words) {
            if (word.x == myShape.positionX && word.y == myShape.positionY) {

                switch (position_arrow)
                {
                case 0:
                    word.x -= 1;
                    if (word.x == -1) {
                        word.x = 19;
                    }
                    break;
                case 1:
                    word.x += 1;
                    if (word.x == 20) {
                        word.x = 0;
                    }
                    break;
                case 2:
                    word.y -= 1;
                    if (word.y == -1) {
                        word.y = 19;
                    }
                    break;
                case 3:
                    word.y += 1;
                    if (word.y == 20) {
                        word.y = 0;
                    }
                    break;
                default:
                    break;
                }

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

        start(selectWord, hDC, rand_i);

        selectWord[rand_i];

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

void randChar(TCHAR* selectWord, vector<Word_s> &words) {

    int p_x;
    int p_y;

    p_x = uid_position(gen);
    p_y = uid_position(gen);

    if (p_x == 0 && p_y == 0) {
        return;
    }

    uniform_int_distribution<> uid_Alpha('A', 'z');
    Word_s word;
    word.w = uid_Alpha(gen);
    word.x = p_x;
    word.y = p_y;

    words.push_back(word);
}

void insertWord(TCHAR char_1, vector<Word_s>& words) {
    int p_x;
    int p_y;

    do {
        p_x = uid_position(gen);
        p_y = uid_position(gen);
    } while (p_x != 0 && p_y != 0);

    Word_s word;
    word.w = char_1;
    word.x = p_x;
    word.y = p_y;
    words.push_back(word);
}

void start(TCHAR* selectWord, HDC hDC, int rand_i) {
    // 선택된 단어 출력하기
    int wordLength = lstrlen(selectWord);
    SIZE textSize;
    int startX = (gridSize - wordLength) / 2 * cellSize; // 그리드 가운데에 단어를 배치
    for (int i = 0; i < wordLength; ++i) {
        if (rand_i == i) {
            TCHAR letter[2] = { ' ', _T('\0') }; // 단어에서 글자를 가져옴
            GetTextExtentPoint32(hDC, letter, 1, &textSize); // 글자의 크기를 가져옴
            int letterX = startX + i * cellSize + (cellSize - textSize.cx) / 2; // 가운데 정렬을 위해 X 좌표 계산
            int letterY = ((gridSize / 2) * cellSize + (cellSize - textSize.cy) / 2) - (cellSize * 9); // 가운데 정렬을 위해 Y 좌표 계산
            TextOut(hDC, letterX, letterY, letter, 1); // 글자 출력
        }
        else {
            TCHAR letter[2] = { selectWord[i], _T('\0') }; // 단어에서 글자를 가져옴
            GetTextExtentPoint32(hDC, letter, 1, &textSize); // 글자의 크기를 가져옴
            int letterX = startX + i * cellSize + (cellSize - textSize.cx) / 2; // 가운데 정렬을 위해 X 좌표 계산
            int letterY = ((gridSize / 2) * cellSize + (cellSize - textSize.cy) / 2) - (cellSize * 9); // 가운데 정렬을 위해 Y 좌표 계산
            TextOut(hDC, letterX, letterY, letter, 1); // 글자 출력
        }
    }
}