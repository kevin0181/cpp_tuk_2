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
static bool moveAll = false;     // ��ü �̵� ��� Ȱ��ȭ ����

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    static HBITMAP hBitmap;
    HPEN mPen, oldPen, redPen;
    HBRUSH mBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static int Timer1Count = 1000;
    static RECT imgRect[4] = { {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} };
    static DWORD dwordS = SRCCOPY;
    static int selectedIndex = -1;  // ���õ� �̹����� �ε���
    static bool m_status = false;
    static float scale[4] = { 1.0f, 1.0f, 1.0f, 1.0f };  // �� �̹����� Ȯ��/��� ����
    const int moveStep = 20;  // �̵��ϴ� ����

    switch (uMsg)
    {
    case WM_CREATE:
    {
        hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));

        if (!hBitmap) {
            MessageBox(hWnd, _T("��Ʈ�� �ε忡 �����߽��ϴ�."), _T("����"), MB_OK);
        }
        else {
            BITMAP bm;
            GetObject(hBitmap, sizeof(BITMAP), &bm);
            size.cx = bm.bmWidth;
            size.cy = bm.bmHeight;
            imgRect[0].right = bm.bmWidth;
            imgRect[0].bottom = bm.bmHeight;
        }

        break;
    }
    case WM_COMMAND:
      
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // Ű���� Ű�� ������ ��
        if (moveAll) {
            switch (wParam)
            {
            case VK_LEFT:
                    for (int i = 0; i < 4; ++i) {
                        imgRect[i].left -= moveStep;
                        imgRect[i].right -= moveStep;
                    }
                break;
            case VK_RIGHT:
                    for (int i = 0; i < 4; ++i) {
                        imgRect[i].left += moveStep;
                        imgRect[i].right += moveStep;
                    }
                break;
            }
        }
        else if (selectedIndex >= 0 && selectedIndex < 4) {
            switch (wParam)
            {
            case VK_LEFT:
                if (imgRect[selectedIndex].left - moveStep >= 0) {
                    imgRect[selectedIndex].left -= moveStep;
                    imgRect[selectedIndex].right -= moveStep;
                }
                break;
            case VK_RIGHT:
                if (imgRect[selectedIndex].right + moveStep <= rect.right) {
                    imgRect[selectedIndex].left += moveStep;
                    imgRect[selectedIndex].right += moveStep;
                }
                break;
            }
        }
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_LBUTTONDOWN:
    {
        if (m_status == false) {
            break;
        }
        int xPos = LOWORD(lParam);
        int yPos = HIWORD(lParam);

        selectedIndex = -1;
        for (int i = 0; i < 4; ++i) {
            if (xPos >= imgRect[i].left && xPos < imgRect[i].right &&
                yPos >= imgRect[i].top && yPos < imgRect[i].bottom) {
                selectedIndex = i;
                break;
            }
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    }

    case WM_CHAR:
        switch (wParam)
        {
        case 'p':
            moveAll = !moveAll;  // ��ü �̵� ��� Ȱ��ȭ/��Ȱ��ȭ ��ȯ
            break;
        case's':
            hBitmap = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));

            if (!hBitmap) {
                MessageBox(hWnd, _T("��Ʈ�� �ε忡 �����߽��ϴ�."), _T("����"), MB_OK);
            }
            else {
                BITMAP bm;
                GetObject(hBitmap, sizeof(BITMAP), &bm);
                size.cx = bm.bmWidth;
                size.cy = bm.bmHeight;
                for (int i = 0; i < 4; ++i) {
                    imgRect[i] = { 0,0,0,0 };
                }
                imgRect[0].right = bm.bmWidth;
                imgRect[0].bottom = bm.bmHeight;
            }
        case '+':
            if (selectedIndex >= 0) {
                scale[selectedIndex] *= 1.1f;  // Ȯ��
                InvalidateRect(hWnd, NULL, true);
            }
            break;
        case '-':
            if (selectedIndex >= 0) {
                scale[selectedIndex] *= 0.9f;  // ���
                InvalidateRect(hWnd, NULL, true);
            }
            break;
        case 'a':
            for (int i = 0; i < 4; ++i) {
                imgRect[i] = { 0,0,0,0 };
            }

            imgRect[0].right = rect.right;
            imgRect[0].bottom = rect.bottom;
            m_status = false;
            break;
        case 'r':
            dwordS = dwordS == SRCCOPY ? NOTSRCCOPY : SRCCOPY;
            break;
        case 'd':
            m_status = true;
            // ù ��° �簢�� (���� ��)
            imgRect[0] = { 0, 0, rect.right / 2, rect.bottom / 2 };
            // �� ��° �簢�� (������ ��)
            imgRect[1] = { rect.right / 2, 0, rect.right, rect.bottom / 2 };
            imgRect[2] = { 0,rect.bottom / 2,rect.right / 2,rect.bottom };
            imgRect[3] = { rect.right / 2, rect.bottom / 2, rect.right, rect.bottom };
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_PAINT:
    {
        GetClientRect(hWnd, &rect);
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);

        // �ε�� ��Ʈ���� ����
        SelectObject(mDC, hBitmap);

        // ��Ʈ���� ������ ũ�⿡ �°� �����ϸ��Ͽ� �׸���
        for (int i = 0; i < 4; ++i) {
            int width = imgRect[i].right - imgRect[i].left;
            int height = imgRect[i].bottom - imgRect[i].top;

            int bmpWidth = static_cast<int>(size.cx * scale[i]);
            int bmpHeight = static_cast<int>(size.cy * scale[i]);

            StretchBlt(hDC, imgRect[i].left, imgRect[i].top, width, height, mDC, 0, 0, bmpWidth, bmpHeight, dwordS);

            if (i == selectedIndex) {
                HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
                FrameRect(hDC, &imgRect[i], redBrush);
                DeleteObject(redBrush);
            }
        }
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
