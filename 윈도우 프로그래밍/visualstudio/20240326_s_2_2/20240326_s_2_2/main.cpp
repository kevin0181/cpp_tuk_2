#include <Windows.h>
#include <tchar.h>
#include <random>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 2-2번";
LPCTSTR lpszWindowName = L"실습 2-2번";

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
uniform_int_distribution<int> uid(2, 10);
uniform_int_distribution<int> rgb_r(0, 255);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC;

    int randomNumber1{};
    int randomNumber2{};

    int randW = uid(gen);
    int randH = uid(gen);

    randomNumber1 = WIDTH / randW;
    randomNumber2 = HEIGHT / randH;

    RECT rect;
    switch (uMsg)
    {
    case WM_CREATE:
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);

        for (size_t i = 0; i < randH; ++i) {

            for (size_t j = 0; j < randW; ++j) {
                rect.left = randomNumber1 * j;
                rect.top = randomNumber2 * i;
                rect.right = randomNumber1 * (j + 1);
                rect.bottom = randomNumber2 * (i + 1);

                SetBkColor(hDC, RGB(rgb_r(gen), rgb_r(gen), rgb_r(gen)));
                SetTextColor(hDC, RGB(rgb_r(gen), rgb_r(gen), rgb_r(gen)));

                DrawText(hDC, L"abcdefghijklmnopqrstuvwxyzabcdefghij", strlen("abcdefghijklmnopqrstuvwxyzabcdefghij"), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

            }

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
