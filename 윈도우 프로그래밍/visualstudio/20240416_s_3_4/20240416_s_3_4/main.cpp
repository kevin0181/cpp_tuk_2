#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 3-4";
LPCTSTR lpszWindowName = L"실습 3-4";

#define WIDTH 1200
#define HEIGHT 800

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

#define MAP_WIDTH 600
#define MAP_HEIGHT 600

struct P {
    RECT rect;
    COLORREF color;
    int positionX;
    int positionY;
};
struct Map {
    RECT rect;
    COLORREF color;
    bool status; // 현재 칸에 말이 존재하는지.
    int positionX;
    int positionY;
    int roundX;
    int roundY;
    void paint_map(HDC &mDC) {
        HBRUSH hBrush = CreateSolidBrush(color);
        HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
        RoundRect(mDC, rect.left, rect.top, rect.right, rect.bottom, roundX, roundY);
        SelectObject(mDC, oldBrush);
        DeleteObject(hBrush);
    }
};

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static int Timer1Count = 0;
    static P p1;
    static P p2;
    static vector<Map> maps;

    switch (uMsg)
    {
    case WM_CREATE:
    {
       // SetTimer(hWnd, 1, 50, NULL); // 50ms 마다 WM_TIMER 메시지 발생
       
        Map map;
        RECT rect;
        for (int i = 0; i < 30; ++i) {
            switch (i)
            {
            case 0:
                rect = { 0, 0,70,70 };
                map.rect = rect;
                map.color = RGB(255, 255, 0);
                map.roundX = 4;
                map.roundY = 4;
                break;
            default:
                rect = { 0,0,50,50 };
                map.rect = rect;
                map.roundX = 4;
                map.roundY = 4;
                break;
            }

            maps.push_back(map);
        }

        break;
    }
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때
        break;
    case WM_LBUTTONDOWN: 
        break;
    case WM_RBUTTONDOWN: {
        break;
    }
    case WM_CHAR:
        switch (wParam)
        {
        case 'q':
            PostQuitMessage(0);
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_PAINT:
    {
        GetClientRect(hWnd, &rect);
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        SelectObject(mDC, (HBITMAP)hBitmap);
        Rectangle(mDC, 0, 0, rect.right, rect.bottom);

        if (maps.size() != 0) {
            maps[0].paint_map(mDC);
        }

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);
        DeleteObject(hBitmap); // 생성한 비트맵 삭제
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1); // 타이머 종료
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}