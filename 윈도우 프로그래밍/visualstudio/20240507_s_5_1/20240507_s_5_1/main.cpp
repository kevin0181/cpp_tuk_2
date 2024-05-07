#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include<set>
#include<stack>
#include "resource.h"

using namespace std;

HINSTANCE g_hInst;

LPCTSTR lpszClass = L"�ǽ� 5-1";
LPCTSTR lpszWindowName = L"�ǽ� 5-1";

#define WIDTH 1000
#define HEIGHT 900

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

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_VSCROLL, 200, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return Message.wParam;

}

#define WIDTH_LINE 40
#define HEGHIT_LINE 40

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    static HBITMAP hBitmap;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static int Timer1Count = 1000;
    static RECT imgRect;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));

        if (!hBitmap) {
            MessageBox(hWnd, _T("��Ʈ�� �ε忡 �����߽��ϴ�."), _T("����"), MB_OK);
        }
        else {
            BITMAP bm;
            GetObject(hBitmap, sizeof(BITMAP), &bm);
            size.cx = bm.bmWidth;
            size.cy = bm.bmHeight;
            imgRect.right = bm.bmWidth;
            imgRect.bottom = bm.bmHeight;
        }

        break;
    }
    case WM_COMMAND:
      
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // Ű���� Ű�� ������ ��
        InvalidateRect(hWnd, NULL, false);  // �����츦 �ٽ� �׸��ϴ�.
        break;
    case WM_CHAR:
        switch (wParam)
        {
        case 'a':
            imgRect.right = rect.right;
            imgRect.bottom = rect.bottom;
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

        // �ε�� ��Ʈ���� ����
        SelectObject(mDC, hBitmap);

        // ��Ʈ���� ������ ũ�⿡ �°� �����ϸ��Ͽ� �׸���
        StretchBlt(hDC, 0, 0, imgRect.right, imgRect.bottom, mDC, 0, 0, size.cx, size.cy, SRCCOPY);

        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
       
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_DESTROY:
        DeleteObject(hBitmap); // ��Ʈ�� ���ҽ��� ����
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
