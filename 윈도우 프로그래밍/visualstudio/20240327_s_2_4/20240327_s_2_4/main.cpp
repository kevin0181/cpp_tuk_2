#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 2-4번";
LPCTSTR lpszWindowName = L"실습 2-4번";

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


#define COL_SIZE 15

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
    static TCHAR lpOut[100];
    static RECT rect;
    static SIZE size;
    static TCHAR str[100];
    static int count;
    static int enter_y{};
    static TCHAR get_x[100];
    static TCHAR get_y[100];
    static TCHAR get_n[3];
    static TCHAR get_m[3];
    static bool status = false;
    switch (uMsg)
    {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        count = 0;
        enter_y = 0;
        break;
    case WM_CHAR:
        if (wParam == VK_BACK) {
            if (count != 0)
                count--;
        }
        else if (wParam == VK_RETURN) {
            if (enter_y < 4 && str[0] != _T('\0')) {
                enter_y++;

                if (enter_y == 4) {
                    HideCaret(hWnd);
                    status = true;
                }
            }

            if (enter_y == 1) {
                for (size_t i = 0; i < count; ++i) {
                    get_x[i] = str[i];
                }
            }

            if (enter_y == 2) {
                for (size_t i = 0; i < count; ++i) {
                    get_y[i] = str[i];
                }
            }

            if (enter_y == 3) {
                if (_tstoi(str) > 19) {
                    enter_y--;
                }
                else {
                    for (size_t i = 0; i < count; ++i) {
                        if (i == 3)
                            break;
                        get_n[i] = str[i];
                    }
                }
            }

            if (enter_y == 4) {
                if (_tstoi(str) > 30 || _tstoi(str) < 1) {
                    enter_y--;
                }
                else {
                    for (size_t i = 0; i < count; ++i) {
                        if (i == 3)
                            break;
                        get_m[i] = str[i];
                    }
                }

            }

            for (int i = 0; i < count; ++i) {
                str[i] = '\0';
            }
            count = 0;

        }
        else if (wParam == 'q') {
            PostQuitMessage(0);
        }
        else {
            str[count++] = wParam;
        }
        str[count] = '\0';
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        GetTextExtentPoint32(hDC, str, lstrlen(str), &size);

        TextOut(hDC, 15, COL_SIZE * enter_y, str, lstrlen(str)); // 입력 받는거

        TextOut(hDC, 0, 0, L"x:", 2);
        TextOut(hDC, 0, COL_SIZE * 1, L"y:", 2);
        TextOut(hDC, 0, COL_SIZE * 2, L"n:", 2);
        TextOut(hDC, 0, COL_SIZE * 3, L"m:", 2);

        TextOut(hDC, 15, 0, get_x, lstrlen(get_x));
        TextOut(hDC, 15, COL_SIZE * 1, get_y, lstrlen(get_y));
        TextOut(hDC, 15, COL_SIZE * 2, get_n, lstrlen(get_n));
        TextOut(hDC, 16, COL_SIZE * 3, get_m, lstrlen(get_m));

        if (status) {

            for (size_t i = 0; i < _tstoi(get_m); ++i) {
                wsprintf(lpOut, L"%d * %d = %d", _tstoi(get_n), (i + 1), _tstoi(get_n) * (i + 1));
                TextOut(hDC, _tstoi(get_x), _tstoi(get_y) + (i * 15), lpOut, lstrlen(lpOut));
            }

            status = false;
            enter_y = 0;

            for (int i = 0; i < lstrlen(get_x); ++i) {
                get_x[i] = '\0';
            }
            for (int i = 0; i < lstrlen(get_y); ++i) {
                get_y[i] = '\0';
            }
            for (int i = 0; i < lstrlen(get_n); ++i) {
                get_n[i] = '\0';
            }
            for (int i = 0; i < lstrlen(get_m); ++i) {
                get_m[i] = '\0';
            }

        }

        SetCaretPos((size.cx + 15), COL_SIZE * enter_y);
        EndPaint(hWnd, &ps);
        ReleaseDC(hWnd, hDC);
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
