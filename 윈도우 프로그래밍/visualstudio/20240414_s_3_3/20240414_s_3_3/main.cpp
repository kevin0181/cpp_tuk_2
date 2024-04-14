#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 3-3";
LPCTSTR lpszWindowName = L"실습 3-3";

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
    switch (uMsg)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, 1, 50, NULL); // 50ms 마다 WM_TIMER 메시지 발생
        break;
    }
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때
        break;
    case WM_LBUTTONDOWN: {  // 마우스 왼쪽 버튼 클릭
        break;
    }
    case WM_RBUTTONDOWN: {  // 마우스 오른쪽 버튼 클릭
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
        Rectangle(mDC, 0, 0, rect.right, rect.bottom); //바탕 흰색

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);
        DeleteObject(hBitmap); // 생성한 비트맵 삭제
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        switch (wParam) {
        default:
            break;
        }
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
