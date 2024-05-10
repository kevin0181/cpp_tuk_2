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

#define IMG_CELL_SIZE 33

struct Move {
    bool left;
    bool right;
    bool up;
    bool down;
    bool jump;
};

struct Character {
    int x = 0;
    int y = 0;
    int width = IMG_CELL_SIZE;
    int height = IMG_CELL_SIZE;
    int move_status = -1; // 초기 이미지 인덱스
    Move move;
    CImage imgs[10];

    void print_v(HDC& mDC1) {
        if (move_status != -1)
            imgs[move_status].Draw(mDC1, x, y, width, height);
    }
};

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
    static Character character;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        character.imgs[0].Load(L"among us move right1.png"); // 우로 걷기
        character.imgs[1].Load(L"among us move left1.png"); // 좌로 걷기
        character.imgs[2].Load(L"among us move back1.png"); // 위로 걷기
        character.imgs[3].Load(L"among us move front1.png"); // 아래로 걷기
        character.imgs[4].Load(L"among us jump right1.png"); // 우 점프
        character.imgs[5].Load(L"among us jump left1.png"); // 좌 점프
        character.imgs[6].Load(L"among us move right up1.png"); // 우 상단
        character.imgs[7].Load(L"among us move right down1.png"); // 우 하단
        character.imgs[8].Load(L"among us move left up1.png"); // 좌 상단
        character.imgs[9].Load(L"among us move left down1.png"); // 좌 하단

        for (int i = 0; i < 10; ++i) {
            if (character.imgs[i].IsNull()) {
                wchar_t errorMsg[256];
                swprintf_s(errorMsg, L"이미지 로드에 실패했습니다: %d번째 이미지", i);
                MessageBox(hWnd, errorMsg, L"이미지 로드 실패", MB_OK | MB_ICONERROR);
            }
        }

        break;
    }
    case WM_COMMAND:
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYUP:
        switch (wParam)
        {
        case VK_LEFT:
            character.move.left = false;
            character.move_status = 1;
            break;
        case VK_RIGHT:
            character.move.right = false;
            character.move_status = 0;
            break;
        case VK_UP:
            character.move.up = false;
            break;
        case VK_DOWN:
            character.move.down = false;
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때
        switch (wParam)
        {
        case VK_LEFT:
            character.move.left = true;
            break;
        case VK_RIGHT:
            character.move.right = true;
            break;
        case VK_UP:
            character.move.up = true;
            break;
        case VK_DOWN:
            character.move.down = true;
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
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_PAINT:
    {
        GetClientRect(hWnd, &rect);
        hDC = BeginPaint(hWnd, &ps);
        mDC1 = CreateCompatibleDC(hDC);
        hBit1 = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        oldBit1 = (HBITMAP)SelectObject(mDC1, hBit1);

        FillRect(mDC1, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

        character.print_v(mDC1);

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