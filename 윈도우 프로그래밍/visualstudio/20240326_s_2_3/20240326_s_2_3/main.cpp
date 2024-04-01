#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 2-3번";
LPCTSTR lpszWindowName = L"실습 2-3번";

#define WIDTH 800
#define HEIGHT 600

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


random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uidX(0, 600);
uniform_int_distribution<int> uidY(0, 400);
uniform_int_distribution<int> uidN(0, 9);
uniform_int_distribution<int> uidC(5, 20);

uniform_int_distribution<int> rgb_r(0, 255);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC;
    RECT rect;
    SIZE size;

    static int x{};
    static int y{};
    static TCHAR n{};
    static int count{};
    TCHAR lpOut[100];

    switch (uMsg)
    {
    case WM_CREATE:
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        x = uidX(gen);
        y = uidY(gen);
        n = uidN(gen);
        count = uidC(gen);

        SetBkColor(hDC, RGB(rgb_r(gen), rgb_r(gen), rgb_r(gen)));
        SetTextColor(hDC, RGB(rgb_r(gen), rgb_r(gen), rgb_r(gen)));

        for (int i = 0; i < count; ++i) {
            for (int j = 0; j < count; ++j) {

                wsprintf(lpOut, L"%d", n);
                TextOut(hDC, x, y, lpOut, lstrlen(lpOut));
                x = x + 8;
            }
            y = y + 14;
            x = x - 8 * count;
        }

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CHAR:
        if (wParam == VK_RETURN)
            InvalidateRect(hWnd, NULL, TRUE);
        if (wParam == 'q')
            PostQuitMessage(0);
        break;
    default:
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
