#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <numeric>
#include<atlimage.h>

using namespace std;

HINSTANCE g_hInst;

LPCTSTR lpszClass = L"실습 5-3";
LPCTSTR lpszWindowName = L"실습 5-3";

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

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC1, mDC2;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    HBITMAP oldBit1, oldBit2;

    static HBITMAP hBit1, hBit2;
    static RECT rect;
    static SIZE size;
    static CImage img[2];
    static int img_select = 0;
    static RECT img_rect1 = { 0,0,200,200 };
    static RECT img_rect2 = { 0,0,200,200 };
    static int img1_status = -1;
    static float scale_factor = 1.0f;

    static bool img_big_status = false;

    static RECT img_save_rect = { 0,0,0,0 };

    switch (uMsg)
    {
    case WM_CREATE:
    {
        img[0].Load(L"dog1.jpeg");
        img[1].Load(L"dog2.jpeg");

        break;
    }
    case WM_COMMAND:
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때
        switch (wParam)
        {
        case VK_LEFT:
            OffsetRect(&img_rect2, -5, 0);
            img_save_rect = img_rect2;
            break;
        case VK_RIGHT:
            OffsetRect(&img_rect2, 5, 0);
            img_save_rect = img_rect2;
            break;
        case VK_UP:
            OffsetRect(&img_rect2, 0, -5);
            img_save_rect = img_rect2;
            break;
        case VK_DOWN:
            OffsetRect(&img_rect2, 0, 5);
            img_save_rect = img_rect2;
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_LBUTTONDOWN:
    {
        InvalidateRect(hWnd, NULL, false);
        break;
    }
    case WM_MOUSEMOVE:
        break;
    case WM_LBUTTONUP:
        break;
    case WM_CHAR:
        switch (wParam)
        {
        case 'r':
            img_rect2 = { 0,0,200,200 };
            img_rect1 = { 0,0,200,200 };
            img1_status = -1;
            img_select = 0;
            break;
        case '1':
            img_select = 0;
            break;
        case '2':
            img_select = 1;
            break;
        case '0':
            scale_factor = 1;
            break;
        case 'e':
            scale_factor += 0.1;
            break;
        case 's':
            scale_factor -= 0.1;
            break;
        case 'c':
            img_save_rect = img_rect2;
            break;
        case 'p':
            img1_status = 1;
            break;
        case 'f':
            img_big_status = !img_big_status;
            if (img_big_status) {
                img_rect1 = rect;
            }
            else {
                img_rect1 = { 0,0,200,200 };
            }
            break;
        case 'm':
            img_rect2 = { 0,0,300,300 };
            break;
        case 'n':
            img_rect2 = { 0,0,200,200 };
            break;
        case 'x':
            img_rect2.right += 5;
            break;
        case 'X':
            img_rect2.right -= 5;
            break;
        case 'y':
            img_rect2.bottom += 5;
            break;
        case 'Y':
            img_rect2.bottom -= 5;
            break;
        case 'a':
            break;
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
        mDC1 = CreateCompatibleDC(hDC);
        hBit1 = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        oldBit1 = (HBITMAP)SelectObject(mDC1, hBit1);

        img[img_select].StretchBlt(mDC1, 0, 0, rect.right, rect.bottom, 0, 0, img[img_select].GetWidth(), img[img_select].GetHeight(), SRCCOPY);
        
        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC1, 0, 0, SRCCOPY);

        if (img1_status != -1) {
            mPen = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
            oldPen = (HPEN)SelectObject(hDC, mPen);
            oldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));

            // 확대된 이미지 크기 계산
            const int src_width = static_cast<int>((img_rect2.right - img_rect2.left) / scale_factor);
            const int src_height = static_cast<int>((img_rect2.bottom - img_rect2.top) / scale_factor);

            // 중심 정렬을 위해 조정된 원본 영역 설정
            const int src_left = img_rect2.left + (img_rect2.right - img_rect2.left - src_width) / 2;
            const int src_top = img_rect2.top + (img_rect2.bottom - img_rect2.top - src_height) / 2;

            // 확대된 상태로 img_rect1에 원본 이미지 영역 표시 (StretchBlt 사용)
            StretchBlt(hDC,
                img_rect1.left,
                img_rect1.top,
                img_rect1.right - img_rect1.left,
                img_rect1.bottom - img_rect1.top,
                mDC1,
                src_left,
                src_top,
                src_width,
                src_height,
                SRCCOPY);

            // img_rect1의 테두리 그리기
            Rectangle(hDC, img_rect1.left, img_rect1.top, img_rect1.right, img_rect1.bottom);

            SelectObject(hDC, oldPen);
            SelectObject(hDC, oldBrush);
            DeleteObject(mPen);
        }

        mPen = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
        oldPen = (HPEN)SelectObject(hDC, mPen);
        oldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));

        Rectangle(hDC, img_rect2.left, img_rect2.top, img_rect2.right, img_rect2.bottom);

        SelectObject(hDC, oldPen);
        SelectObject(hDC, oldBrush);
        DeleteObject(mPen);

        // 로드된 비트맵을 선택
        SelectObject(mDC1, oldBit1);
        DeleteObject(hBit1);  // 해제
        DeleteDC(mDC1);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}