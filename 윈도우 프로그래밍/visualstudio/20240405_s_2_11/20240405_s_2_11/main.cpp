#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 2-11번";
LPCTSTR lpszWindowName = L"실습 2-11번";

#define WIDTH 1000
#define HEIGHT 800
#define COL 16

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

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return Message.wParam;

}

class shape {
public:
};

class RedBarricade : public shape {
public:
    RECT sizeRect = {
        0,0,20,20
    };
    int positionX{};
    int positionY{};
    COLORREF color = RGB(255, 0, 0);
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
};

class ChangeColor : public shape {
public:
    RECT sizeRect = {
        0,0,20,20
    };
    int positionX{};
    int positionY{};
    COLORREF color;
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
};

class ChangeShape : public shape {
public:
    POINT sizePoint[6];
    int positionX{};
    int positionY{};
    COLORREF color = RGB(255, 165, 0); //주황
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
    char shape_c;
    bool mini_status = false; // false = 큰, true = 작은
};

class SizeDown : public shape {
public:
    RECT sizeRect = {
        0,0,10,10
    };
    int positionX{};
    int positionY{};
    COLORREF color = RGB(200, 200, 0); //노랑
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
};

class SizeUp : public shape {
public:
    RECT sizeRect = {
        0,0,20,20
    };
    int positionX{};
    int positionY{};
    COLORREF color = RGB(135, 206, 235); //하늘색
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
};

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC;
    HPEN hPen, oldPen;
    HBRUSH hBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static vector<shape*> shapes;
    static TCHAR str[30];
    static int cnt_x;

    switch (uMsg)
    {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        cnt_x = 0;
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_CHAR:
        str[cnt_x++] = wParam;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_SIZE:
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &rect);
        
        MoveToEx(hDC, 0, rect.bottom - (COL + 10), NULL);
        LineTo(hDC, rect.right, rect.bottom - (COL + 10));
        GetTextExtentPoint32(hDC, str, lstrlen(str), &size);
        TextOut(hDC, 0, rect.bottom - (COL + 5), str, lstrlen(str));// 입력 받은거 출력
        SetCaretPos((size.cx), rect.bottom - (COL + 5)); //caret 위치 표시
        ShowCaret(hWnd);

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        HideCaret(hWnd);
        DestroyCaret();
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}