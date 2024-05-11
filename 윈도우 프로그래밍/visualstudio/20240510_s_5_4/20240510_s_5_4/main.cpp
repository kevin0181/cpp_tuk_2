#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <numeric>
#include<atlimage.h>
#include <gdiplus.h>

using namespace Gdiplus;

using namespace std;

ULONG_PTR gdiplusToken;

HINSTANCE g_hInst;

LPCTSTR lpszClass = L"실습 5-3";
LPCTSTR lpszWindowName = L"실습 5-3";

#define WIDTH 1000
#define HEIGHT 900

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {

    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

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
    GdiplusShutdown(gdiplusToken);

    return Message.wParam;

}

#define IMG_CELL_SIZE 165

struct Move {
    bool left;
    bool right;
    bool up;
    bool down;
    int jump = -1;
    bool jump_status;
};

struct Character {
    int x = 0;
    int y = 0;
    int width = 100;
    int height = 100;
    int move_status = 0; // 초기 이미지 인덱스
    Move move;
    CImage imgs[10];
    int select_img = 0;

    void print_v(HDC& mDC1) {
        if (move_status != -1)
            //imgs[move_status].Draw(mDC1, x, y, width, height, IMG_CELL_SIZE * (select_img - 1), IMG_CELL_SIZE * (select_img - 1), IMG_CELL_SIZE * select_img, IMG_CELL_SIZE * select_img);
            imgs[move_status].Draw(mDC1, x, y, width, height, IMG_CELL_SIZE * select_img, 0, IMG_CELL_SIZE, IMG_CELL_SIZE);

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
        SetTimer(hWnd, 1, 160, NULL);
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
            character.select_img = 0;
            break;
        case VK_RIGHT:
            character.move.right = false;
            character.select_img = 0;
            break;
        case VK_UP:
            character.move.up = false;
            character.select_img = 0;
            break;
        case VK_DOWN:
            character.move.down = false;
            character.select_img = 0;
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
            character.move_status = 1;
            break;
        case VK_RIGHT:
            character.move.right = true;
            character.move_status = 0;
            break;
        case VK_UP:
            character.move.up = true;
            character.move_status = 2;
            break;
        case VK_DOWN:
            character.move.down = true;
            character.move_status = 3;
            break;
        default:
            break;
        }

        if (character.move.right && character.move.down) { //우 하단
            character.move_status = 7;
        }

        if (character.move.right && character.move.up) { // 우 상단
            character.move_status = 6;
        }

        if (character.move.left && character.move.down) { // 좌 하단
            character.move_status = 9;
        }
        
        if (character.move.left && character.move.up) { // 좌 상단
            character.move_status = 8;
        }

        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_CHAR:
        switch (wParam) 
        {
        case 'j':
            character.move.jump_status = true;
            character.move.jump = 0;
            if (character.move_status == 1) {
                character.move_status = 5;
            }
            else if (character.move_status == 0) {
                character.move_status = 4;
            }
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
    case WM_PAINT:
    {
        GetClientRect(hWnd, &rect);
        hDC = BeginPaint(hWnd, &ps);
        mDC1 = CreateCompatibleDC(hDC);
        hBit1 = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        oldBit1 = (HBITMAP)SelectObject(mDC1, hBit1);

        FillRect(mDC1, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

        character.print_v(mDC1);

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC1, 0, 0, SRCCOPY);

        // 로드된 비트맵을 선택
        SelectObject(mDC1, oldBit1);
        DeleteObject(hBit1);  // 해제
        DeleteDC(mDC1);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        switch (wParam)
        {
        case 1:
        {
            Move& move = character.move;
            if (move.down || move.left || move.up || move.right) {
                character.select_img++;
                if (character.select_img == 4) {
                    character.select_img = 0;
                }

                if (move.down) {
                    character.y += 7;
                }

                if (move.left) {
                    character.x -= 7;
                }

                if (move.up) {
                    character.y -= 7;
                }

                if (move.right) {
                    character.x += 7;
                }

            }
            if (move.jump != -1 && move.jump_status) {
                move.jump += 10;
                character.y -= move.jump;
                if (move.jump >= 20) {
                    move.jump_status = false;
                }
            }
            else if (move.jump != -1 && !move.jump_status) {
                move.jump += 10;
                character.y += move.jump;
                if (move.jump >= 30) {
                    move.jump = -1;
                    move.jump_status = false;

                    if (character.move_status == 5) {
                        character.move_status = 1;
                        move.left = true;
                    }
                    else if (character.move_status == 4) {
                        character.move_status = 0;
                        move.right = true;
                    }

                }
            }
            break;
        }
        default:
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