#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"시험 연습1";
LPCTSTR lpszWindowName = L"시험 연습1";

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitMap;
    static RECT rect;
    const int WIDTH_GRID = 20;
    const int HEIGHT_GRID = 20;
    static int cellSizeX;
    static int cellSizeY;
    switch (uMsg)
    {
    case WM_CREATE:
        break;
    case WM_PAINT:
        GetClientRect(hWnd, &rect);
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitMap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        SelectObject(mDC, (HBITMAP)hBitMap);
        FillRect(mDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
        
        cellSizeX = rect.right / WIDTH_GRID;
        cellSizeY = rect.bottom / HEIGHT_GRID;


        for (int x = 0; x <= cellSizeX; ++x) {
            MoveToEx(mDC, 0, x * cellSizeY, NULL);
            LineTo(mDC, WIDTH_GRID * cellSizeX, x * cellSizeY);
        }

        for (int y = 0; y <= cellSizeY; ++y) {
            MoveToEx(mDC, 0, y * cellSizeX, NULL);
            LineTo(mDC, y * cellSizeX, HEIGHT_GRID * cellSizeY);
        }


        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);
        DeleteObject(hBitMap);
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}