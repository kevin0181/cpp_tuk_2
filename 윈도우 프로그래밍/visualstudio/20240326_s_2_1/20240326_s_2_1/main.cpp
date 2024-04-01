#include <Windows.h>
#include <tchar.h>
#include <random>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 2-1번";
LPCTSTR lpszWindowName = L"실습 2-1번";

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

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, 0, 0, 800, 600, NULL, (HMENU)NULL, hInstance, NULL);
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
uniform_int_distribution<int> uid(0, 80);
uniform_int_distribution<int> uid2(0, 60);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC;


    int randomNumber1{};
    int randomNumber2{};
    TCHAR lpOut[100];
    switch (uMsg)
    {
    case WM_CREATE:
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        wsprintf(lpOut, L"%d:(%d,%d)", 0, 400, 300);
        TextOut(hDC, 400, 300, lpOut, lstrlen(lpOut));
        for (int i = 1; i < 10; ++i) {
            randomNumber1 = uid(gen) * 10;
            randomNumber2 = uid2(gen) * 10;
            wsprintf(lpOut, L"%d:(%d,%d)", i, randomNumber1, randomNumber2);
            TextOut(hDC, randomNumber1, randomNumber2, lpOut, lstrlen(lpOut));
        }

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
