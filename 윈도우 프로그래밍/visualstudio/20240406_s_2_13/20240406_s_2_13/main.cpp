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

void randChar(TCHAR* selectWord, vector<Word_s>& words);
void start(TCHAR* selectWord, HDC hDC, int rand_i);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC;
    HPEN hPen, oldPen;
    HBRUSH hBrush, oldBrush;
    static RECT rect;
    static SIZE size;

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
    switch (uMsg)
    {
    case WM_CREATE:
    {
        selectWord = word[uid_getWord(gen)];

        uniform_int_distribution<int> uid_ri(0, lstrlen(selectWord) - 1);
        rand_i = uid_ri(gen);

        int rand2 = uid_ranSize(gen);

        for (int i = 0; i < rand2; ++i) {
            randChar(selectWord, words);
        }
    }
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:

        hDC = BeginPaint(hWnd, &ps);

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

        start(selectWord, hDC, rand_i);

        for (auto& word : words) {
            SIZE textSize;
            GetTextExtentPoint32(hDC, &word.w, 1, &textSize); // 글자의 크기를 가져옴
            int letterX = (cellSize - textSize.cx) / 2 + (cellSize * word.x); // 가운데 정렬을 위해 X 좌표 계산
            int letterY = (cellSize - textSize.cy) / 2 + (cellSize * word.y); // 가운데 정렬을 위해 Y 좌표 계산
            TextOut(hDC, letterX, letterY, &word.w, 1); // 글자 출력
        }

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