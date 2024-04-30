#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 4-2";
LPCTSTR lpszWindowName = L"실습 4-2";

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

#define WIDTH_LINE 40
#define HEGHIT_LINE 40

struct Rec {
    int x;
    int y;
    RECT rect;
    COLORREF color;
    bool status;
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
    static int cellSizeX;
    static int cellSizeY;
    static bool LDrag = false;
    static bool LStatus = false;
    static bool RDrag = false;
    static RECT moseRect;
    static vector<Rec> rec;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        moseRect = { 0,0,0,0 };
        cellSizeX = 20;
        cellSizeY = 20;
        break;
    }
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때
        break;
    case WM_LBUTTONDOWN: { 
        LDrag = true;
        moseRect.left = LOWORD(lParam);
        moseRect.top = HIWORD(lParam);
        
        InvalidateRect(hWnd, NULL, false);  // 윈도우를 다시 그립니다.
        break;
    }
    case WM_LBUTTONUP:
    {
        LDrag = false;
        LStatus = false;

        if (rec.size() >= 5) {
            rec.erase(rec.begin());
        }

        Rec newRect;
        // Snap the coordinates to the nearest grid lines
        newRect.x = (min(moseRect.left, moseRect.right) / cellSizeX) * cellSizeX;
        newRect.y = (min(moseRect.top, moseRect.bottom) / cellSizeY) * cellSizeY;
        int right = (max(moseRect.right, moseRect.left) / cellSizeX) * cellSizeX;
        int bottom = (max(moseRect.bottom, moseRect.top) / cellSizeY) * cellSizeY;

        // Ensure the rectangle is at least one grid cell in size
        if (right == newRect.x) right += cellSizeX;
        if (bottom == newRect.y) bottom += cellSizeY;

        right = right + cellSizeX;
        bottom = bottom + cellSizeY;

        newRect.rect = { newRect.x, newRect.y, right, bottom };
        newRect.color = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
        newRect.status = false;
        rec.push_back(newRect);

        moseRect = { 0,0,0,0 };
        InvalidateRect(hWnd, NULL, false);
        break;
    }
    case WM_RBUTTONDOWN: {
        RDrag = true;
        InvalidateRect(hWnd, NULL, false);  // 윈도우를 다시 그립니다.
        break;
    }
    case WM_RBUTTONUP:
    {
        RDrag = false;
        InvalidateRect(hWnd, NULL, false);
    }
        break;
    case WM_MOUSEMOVE:
    {
        POINT moseP;
        moseP.x = LOWORD(lParam);
        moseP.y = HIWORD(lParam);

        if (LDrag) {
            LStatus = true;
            moseRect.right = moseP.x;
            moseRect.bottom = moseP.y;
        }

        if (RDrag) {

        }
        InvalidateRect(hWnd, NULL, false);
    }
        break;
    case WM_CHAR:
        switch (wParam)
        {
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
        FillRect(mDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

        cellSizeX = rect.right / WIDTH_LINE;
        cellSizeY = rect.bottom / HEGHIT_LINE;

        for (int y = 0; y <= WIDTH_LINE; ++y) {
            MoveToEx(mDC, y * cellSizeX, 0, NULL);
            LineTo(mDC, y * cellSizeX, WIDTH_LINE * cellSizeY);
        }

        for (int x = 0; x <= HEGHIT_LINE; ++x) {
            MoveToEx(mDC, 0, x * cellSizeY, NULL);
            LineTo(mDC, HEGHIT_LINE * cellSizeX, x * cellSizeY);
        }

        // 각 사각형을 그리기 전에 브러시를 설정합니다.
        for (auto& r : rec) {
            mBrush = CreateSolidBrush(r.color);
            SelectObject(mDC, mBrush);
            Rectangle(mDC, r.rect.left, r.rect.top, r.rect.right, r.rect.bottom);
            DeleteObject(mBrush);
        }

        // 겹치는 사각형의 색상을 혼합
        for (size_t i = 0; i < rec.size(); i++) {
            for (size_t j = i + 1; j < rec.size(); j++) {
                RECT intersection;
                if (IntersectRect(&intersection, &rec[i].rect, &rec[j].rect)) {
                    int newRed = (GetRValue(rec[i].color) + GetRValue(rec[j].color)) / 2;
                    int newGreen = (GetGValue(rec[i].color) + GetGValue(rec[j].color)) / 2;
                    int newBlue = (GetBValue(rec[i].color) + GetBValue(rec[j].color)) / 2;
                    mBrush = CreateSolidBrush(RGB(newRed, newGreen, newBlue));
                    SelectObject(mDC, mBrush);
                    Rectangle(mDC, intersection.left, intersection.top, intersection.right, intersection.bottom);
                    DeleteObject(mBrush);
                }
            }
        }

        if (LStatus) {
            mBrush = (HBRUSH)GetStockObject(BLACK_BRUSH); // 투명한 브러시 사용
            oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

            FrameRect(mDC, &moseRect, mBrush); // 사각형의 테두리만 그림

            SelectObject(mDC, oldBrush);
            DeleteObject(mBrush);
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
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}