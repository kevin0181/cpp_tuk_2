﻿#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 3-1";
LPCTSTR lpszWindowName = L"실습 3-1";

#define WIDTH 900
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

struct shape {
    int positionX;
    int positionY;
    HBRUSH mBrush, oldBrush;
    RECT circleRect;
    int cellSizeX;
    int cellSizeY;
    int move_i;

    void print_shape(HDC& mDC) {

        circleRect.left = 0;
        circleRect.top = 0;
        circleRect.right = cellSizeX;
        circleRect.bottom = cellSizeY;

        circleRect.left += (positionX * cellSizeX);
        circleRect.right += (positionX * cellSizeX);
        circleRect.top += (positionY * cellSizeY);
        circleRect.bottom += (positionY * cellSizeY);

        mBrush = CreateSolidBrush(RGB(255, 0, 0));
        oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
        Ellipse(mDC, circleRect.left, circleRect.top, circleRect.right, circleRect.bottom);
        DeleteObject(mBrush);
    }
};

#define gridSize 20

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uid_ran_4(0, 3);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    int cellSizeX;
    int cellSizeY;
    static int hero_positionX{};
    static int hero_positionY{};
    static bool start_status = false;
    static int Timer1Count = 0;
    static bool hero_lineX_status = false;
    static bool hero_lineY_status = false;

    static vector<shape> shapes;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, 1, 1, NULL);

        for (int i = 0; i < 20; ++i) {
            shape shape1;
            shape1.move_i = uid_ran_4(gen);
            shapes.push_back(shape1);
        }

    }
    break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:

        hDC = BeginPaint(hWnd, &ps);

        InvalidateRect(hWnd, NULL, TRUE);

        EndPaint(hWnd, &ps);
        break;
    case WM_CHAR:

        switch (wParam)
        {
        case  's': //start
            start_status = true;
            break;
        default:
            break;
        }
        
        InvalidateRect(hWnd, NULL, TRUE);

        break;
    case WM_SIZE:
        break;
    case WM_PAINT:
    {
        GetClientRect(hWnd, &rect);
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        SelectObject(mDC, (HBITMAP)hBitmap);

        Rectangle(mDC, 0, 0, rect.right, rect.bottom);
        cellSizeX = rect.right / gridSize;
        cellSizeY = rect.bottom / gridSize;

        for (int y = 0; y <= gridSize; ++y) {
            MoveToEx(mDC, y * cellSizeX, 0, NULL);
            LineTo(mDC, y * cellSizeX, gridSize * cellSizeY);
        }

        for (int x = 0; x <= gridSize; ++x) {
            MoveToEx(mDC, 0, x * cellSizeY, NULL);
            LineTo(mDC, gridSize * cellSizeX, x * cellSizeY);
        }
        
        shapes[0].cellSizeX = cellSizeX;
        shapes[0].cellSizeY = cellSizeY;

        if (start_status) {
            if (Timer1Count % 2 == 0) {
                // 수평 이동
                hero_positionX += hero_lineX_status ? -1 : 1;

                // 수평 이동에 대한 경계 검사
                if (hero_positionX > 19) {
                    hero_positionX = 19; // 최대값으로 제한
                    hero_lineX_status = true; // 방향을 왼쪽으로 변경
                    hero_positionY += hero_lineY_status ? -1 : 1; // 수직 이동
                }
                else if (hero_positionX < 0) {
                    hero_positionX = 0; // 최소값으로 제한
                    hero_lineX_status = false; // 방향을 오른쪽으로 변경
                    hero_positionY += hero_lineY_status ? -1 : 1; // 수직 이동
                }

                // 수직 경계 검사 및 방향 변경
                if (hero_positionY > 19) {
                    hero_positionY = 19; // 아래쪽으로 제한
                    hero_lineY_status = true; // 방향을 위로 변경
                }
                else if (hero_positionY < 0) {
                    hero_positionY = 0; // 위쪽으로 제한
                    hero_lineY_status = false; // 방향을 아래로 변경
                }
            }
            shapes[0].positionX = hero_positionX;
            shapes[0].positionY = hero_positionY;
            shapes[0].print_shape(mDC);
        }
        else {
            shapes[0].positionX = 0;
            shapes[0].positionY = 0;
            shapes[0].print_shape(mDC);
        }
        
        //--- 마지막에 메모리 DC의 내용을 화면 DC로 복사한다.
        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        switch (wParam) {
        case 1:
            Timer1Count++;
            break;
        }
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