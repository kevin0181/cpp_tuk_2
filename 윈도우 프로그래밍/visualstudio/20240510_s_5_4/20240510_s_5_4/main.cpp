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

LPCTSTR lpszClass = L"실습 5-4";
LPCTSTR lpszWindowName = L"실습 5-4";

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
    static Character character[6];
    static bool e_status = true;
    static bool s_status = true;
    static CImage backgroundImage;
    static int selectCharacter = 0;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, 1, 160, NULL);
        character[0].imgs[0].Load(L"among us move right1.png"); // 우로 걷기
        character[0].imgs[1].Load(L"among us move left1.png"); // 좌로 걷기
        character[0].imgs[2].Load(L"among us move back1.png"); // 위로 걷기
        character[0].imgs[3].Load(L"among us move front1.png"); // 아래로 걷기
        character[0].imgs[4].Load(L"among us jump right1.png"); // 우 점프
        character[0].imgs[5].Load(L"among us jump left1.png"); // 좌 점프
        character[0].imgs[6].Load(L"among us move right up1.png"); // 우 상단
        character[0].imgs[7].Load(L"among us move right down1.png"); // 우 하단
        character[0].imgs[8].Load(L"among us move left up1.png"); // 좌 상단
        character[0].imgs[9].Load(L"among us move left down1.png"); // 좌 하단

        for (int i = 0; i < 10; ++i) {
            if (character[0].imgs[i].IsNull()) {
                wchar_t errorMsg[256];
                swprintf_s(errorMsg, L"이미지 로드에 실패했습니다: %d번째 이미지", i);
                MessageBox(hWnd, errorMsg, L"이미지 로드 실패", MB_OK | MB_ICONERROR);
            }
        }

        backgroundImage.Load(L"b.png");

        break;
    }
    case WM_COMMAND:
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYUP:
        switch (wParam)
        {
        case VK_LEFT:
            character[selectCharacter].move.left = false;
            character[selectCharacter].select_img = 0;
            break;
        case VK_RIGHT:
            character[selectCharacter].move.right = false;
            character[selectCharacter].select_img = 0;
            break;
        case VK_UP:
            character[selectCharacter].move.up = false;
            character[selectCharacter].select_img = 0;
            break;
        case VK_DOWN:
            character[selectCharacter].move.down = false;
            character[selectCharacter].select_img = 0;
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
            character[selectCharacter].move.left = true;
            character[selectCharacter].move_status = 1;
            break;
        case VK_RIGHT:
            character[selectCharacter].move.right = true;
            character[selectCharacter].move_status = 0;
            break;
        case VK_UP:
            character[selectCharacter].move.up = true;
            character[selectCharacter].move_status = 2;
            break;
        case VK_DOWN:
            character[selectCharacter].move.down = true;
            character[selectCharacter].move_status = 3;
            break;
        default:
            break;
        }

        if (character[selectCharacter].move.right && character[selectCharacter].move.down) { //우 하단
            character[selectCharacter].move_status = 7;
        }

        if (character[selectCharacter].move.right && character[selectCharacter].move.up) { // 우 상단
            character[selectCharacter].move_status = 6;
        }

        if (character[selectCharacter].move.left && character[selectCharacter].move.down) { // 좌 하단
            character[selectCharacter].move_status = 9;
        }
        
        if (character[selectCharacter].move.left && character[selectCharacter].move.up) { // 좌 상단
            character[selectCharacter].move_status = 8;
        }

        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_CHAR:
        switch (wParam) 
        {
        case 'e':
            SetTimer(hWnd, 2, 160, NULL);
            break;
        case 's':
            SetTimer(hWnd, 3, 160, NULL);
            break;
        case 'j':
            character[selectCharacter].move.jump_status = true;
            character[selectCharacter].move.jump = 0;
            if (character[selectCharacter].move_status == 1) {
                character[selectCharacter].move_status = 5;
            }
            else if (character[selectCharacter].move_status == 0) {
                character[selectCharacter].move_status = 4;
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

        backgroundImage.Draw(mDC1, 0, 0, rect.right, rect.bottom);

        character[selectCharacter].print_v(mDC1);

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

        case 2:

            if (e_status) {
                character[selectCharacter].width += 10;
                character[selectCharacter].height +=10;
                if (character[selectCharacter].width >= 200 && character[selectCharacter].height >= 200) {
                    e_status = false;
                }
            }
            else {
                character[selectCharacter].width -= 10;
                character[selectCharacter].height -= 10;
                if (character[selectCharacter].width <= 100 && character[selectCharacter].height <= 100) {
                    e_status = true;
                    character[selectCharacter].width = 100;
                    character[selectCharacter].height = 100;
                    KillTimer(hWnd, 2);
                }
            }

            break;
        case 3:
            if (e_status) {
                character[selectCharacter].width -= 10;
                character[selectCharacter].height -= 10;
                if (character[selectCharacter].width <= 50 && character[selectCharacter].height <= 50) {
                    e_status = false;
                }
            }
            else {
                character[selectCharacter].width += 10;
                character[selectCharacter].height += 10;
                if (character[selectCharacter].width >= 100 && character[selectCharacter].height >= 100) {
                    e_status = true;
                    character[selectCharacter].width = 100;
                    character[selectCharacter].height = 100;
                    KillTimer(hWnd, 3);
                }
            }
            break;
        case 1:
        {
            Move& move = character[selectCharacter].move;
            if (move.down || move.left || move.up || move.right) {
                character[selectCharacter].select_img++;
                if (character[selectCharacter].select_img == 4) {
                    character[selectCharacter].select_img = 0;
                }

                if (move.down) {
                    character[selectCharacter].y += 7;
                }

                if (move.left) {
                    character[selectCharacter].x -= 7;
                }

                if (move.up) {
                    character[selectCharacter].y -= 7;
                }

                if (move.right) {
                    character[selectCharacter].x += 7;
                }

            }
            if (move.jump != -1 && move.jump_status) {
                move.jump += 10;
                character[selectCharacter].y -= move.jump;
                if (move.jump >= 20) {
                    move.jump_status = false;
                }
            }
            else if (move.jump != -1 && !move.jump_status) {
                move.jump += 10;
                character[selectCharacter].y += move.jump;
                if (move.jump >= 30) {
                    move.jump = -1;
                    move.jump_status = false;

                    if (character[selectCharacter].move_status == 5) {
                        character[selectCharacter].move_status = 1;
                        move.left = true;
                    }
                    else if (character[selectCharacter].move_status == 4) {
                        character[selectCharacter].move_status = 0;
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