#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <gdiplus.h>

#pragma comment (lib, "gdiplus.lib")

using namespace std;
using namespace Gdiplus;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 3-4";
LPCTSTR lpszWindowName = L"실습 3-4";

#define WIDTH 1200
#define HEIGHT 800

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;
HWND hwndButton;  // 버튼의 핸들

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

    hInst = hInstance;

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_VSCROLL, 0, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return Message.wParam;

}

#define MAP_WIDTH 600
#define MAP_HEIGHT 600

struct P {
    RECT rect;
    COLORREF color;
    int positionX = 0;
    int positionY = 0;
    int move_cnt = 0;
};

struct MAP {
    int x;
    int y;
};

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uid_u(0, 1);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput; // img make
    // GDI+ 관련된 어떤 함수라도 사용 전에 해당 함수를 호출해야 합니다.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static int Timer1Count = 0;

    static RECT mapRECT = {10, 10, 700, 700};
    static RECT uRECT = { 750,20,850,310 };

    const static wchar_t* buttonText = L"1번 유저 던지기!";  // 초기 버튼 텍스트
    static P red;
    static P blue;

    static int user_status = -1;
    static int move[4];
    static int move_stack = 0;
    int c_m = 108;

    static bool game_status = false;
        
    static MAP maps[30]= {
            {648,648},
            {650,500},
            {650,405},
            {650,310},
            {650,215},
            {650,60},
            {500,60},
            {400,60},
            {310,60},
            {215,60},
            {63,63},
            {65,214},
            {65,310},
            {65,400},
            {65,500},
            {65,650},
            {215,650},
            {310,650},
            {405,650},
            {500,650},
            {540,175},//20
            {455,258},
            {357,357},
            {255,460},
            {172,543},
            {170,175}, // 25
            {255,258},
            {357,357},
            {455,457},
            {540,542},
    };
    switch (uMsg)
    {
    case WM_CREATE:
    {
       // SetTimer(hWnd, 1, 50, NULL); // 50ms 마다 WM_TIMER 메시지 발생
       
       // 버튼 생성
        hwndButton = CreateWindow(L"button", buttonText,
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            880, 320, 150, 30,
            hWnd, (HMENU)1, hInst, NULL);

        red.rect = { 0, 0, 25, 25 };
        blue.rect = { 0, 0, 25, 25 };
        red.positionX = 800;
        red.positionY = 400;
        blue.positionX = 830;
        blue.positionY = 400;

        break;
    }
    case WM_COMMAND:
        if (!game_status) {
            if (LOWORD(wParam) == 1) {  // 버튼 클릭 이벤트 확인
                buttonText = (wcscmp(buttonText, L"1번 유저 던지기!") == 0) ? L"2번 유저 던지기!" : L"1번 유저 던지기!";
                SetWindowText(hwndButton, buttonText);  // 버튼 텍스트 변경

                user_status = user_status == 1 ? 2 : 1;

                for (int& m : move) {
                    m = uid_u(gen);
                }

                for (int i = 0; i < 4; ++i) {
                    if (move[i]) {
                        move_stack++;
                    }
                }

                if (move_stack == 0) { //mo
                    move_stack = 5;
                }

                if (user_status == 1)
                {

                    switch (red.move_cnt)
                    {
                    case 5:
                        move_stack--;
                        red.move_cnt = 20;
                        break;
                    case 10:
                        move_stack--;
                        red.move_cnt = 25;
                        break;
                    case 22:
                        move_stack--;
                        red.move_cnt = 28;
                        break;
                    case 27:
                        move_stack--;
                        red.move_cnt = 28;
                        break;
                    default:
                        break;
                    }

                    for (int i = 0; i < move_stack; ++i) {
                        red.move_cnt += 1;
                        if (red.move_cnt == 24) {
                            ++i;
                            red.move_cnt = 15;
                        }
                        if (red.move_cnt >= 30) {
                            game_status = true;
                            MessageBox(hWnd, L"red Win!", L"red win!", MB_OK | MB_ICONINFORMATION);
                            break;
                        }
                        if (red.move_cnt == 19 || red.move_cnt == 0) {
                            red.move_cnt = 0;
                            if (move_stack > 0) {
                                game_status = true;
                                MessageBox(hWnd, L"red Win!", L"red win!", MB_OK | MB_ICONINFORMATION);
                                break;
                            }
                        }
                    }



                    red.positionX = maps[red.move_cnt].x - 12.5;
                    red.positionY = maps[red.move_cnt].y - 12.5;
                }
                else if (user_status == 2) {
                    switch (blue.move_cnt)
                    {
                    case 5:
                        move_stack--;
                        blue.move_cnt = 20;
                        break;
                    case 10:
                        move_stack--;
                        blue.move_cnt = 25;
                        break;
                    case 22:
                        move_stack--;
                        blue.move_cnt = 28;
                        break;
                    case 27:
                        move_stack--;
                        blue.move_cnt = 28;
                        break;
                    default:
                        break;
                    }

                    for (int i = 0; i < move_stack; ++i) {
                        blue.move_cnt += 1;
                        if (blue.move_cnt == 24) {
                            ++i;
                            blue.move_cnt = 15;
                        }
                        if (blue.move_cnt > 30) {
                            game_status = true;
                            MessageBox(hWnd, L"blue Win!", L"blue win!", MB_OK | MB_ICONINFORMATION);
                            break;
                        }
                        if (blue.move_cnt == 19 || blue.move_cnt == 0) {
                            blue.move_cnt = 0;
                            if (move_stack > 0) {
                                game_status = true;
                                MessageBox(hWnd, L"blue Win!", L"blue win!", MB_OK | MB_ICONINFORMATION);
                                break;
                            }
                        }
                    }



                    blue.positionX = maps[blue.move_cnt].x - 12.5;
                    blue.positionY = maps[blue.move_cnt].y - 12.5;
                }
                move_stack = 0;
            }
        }

        if (user_status == 1 && blue.move_cnt == red.move_cnt) {
            blue.move_cnt = 0;
            blue.positionX = maps[blue.move_cnt].x - 12.5;
            blue.positionY = maps[blue.move_cnt].y - 12.5;
        }
        else if (user_status == 2 && red.move_cnt == blue.move_cnt) {
            red.move_cnt = 0;
            red.positionX = maps[red.move_cnt].x - 12.5;
            red.positionY = maps[red.move_cnt].y - 12.5;
        }

        SetFocus(hWnd);
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때
        break;
    case WM_LBUTTONDOWN: 
    //{
    //    int xPos = LOWORD(lParam); // 현재 마우스의 X 좌표
    //    int yPos = HIWORD(lParam); // 현재 마우스의 Y 좌표
    //    wchar_t szPos[100];        // 좌표를 저장할 문자 배열

    //    // 좌표 정보를 문자열로 포맷팅
    //    wsprintf(szPos, L"X: %d, Y: %d", xPos, yPos);

    //    // 메시지 박스로 좌표 표시
    //    MessageBox(hWnd, szPos, L"Current Position", MB_OK | MB_ICONINFORMATION);
    //    break;
    //}
    case WM_RBUTTONDOWN: {
        break;
    }
    case WM_CHAR:
        switch (wParam)
        {
        case 's':
        {
            red.rect = { 0, 0, 25, 25 };
            blue.rect = { 0, 0, 25, 25 };
            red.move_cnt = 0;
            blue.move_cnt = 0;
            red.positionX = 800;
            red.positionY = 400;
            blue.positionX = 830;
            blue.positionY = 400;
            game_status = false;
            move_stack = 0;
            user_status = -1;
            buttonText = L"1번 유저 던지기!";
            SetWindowText(hwndButton, buttonText);  // 버튼 텍스트 변경

            break;
        }
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
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        SelectObject(mDC, (HBITMAP)hBitmap);
        Rectangle(mDC, 0, 0, rect.right, rect.bottom);

        /*if (maps.size() != 0) {
            maps[0].paint_map(mDC);
        }*/


        Graphics graphics(mDC);
        Image mapImage(L"./../map.png");
        Rect mapRect(mapRECT.left, mapRECT.top, mapRECT.right - mapRECT.left, mapRECT.bottom - mapRECT.top);
        graphics.DrawImage(&mapImage, mapRect);

        Image redImage(L"./../red.png");
        Rect redRect(red.rect.left + red.positionX, red.rect.top + red.positionY, red.rect.right - red.rect.left, red.rect.bottom - red.rect.top);
        graphics.DrawImage(&redImage, redRect);

        Image blueImage(L"./../blue.png");
        Rect blueRect(blue.rect.left + blue.positionX, blue.rect.top + blue.positionY, blue.rect.right - blue.rect.left, blue.rect.bottom - blue.rect.top);
        graphics.DrawImage(&blueImage, blueRect);

        if (user_status != -1) {
            for (int i = 0; i < 4; ++i) {
                if (move[i]) {
                    Image u1Image(L"./../u1.png");
                    Rect uRect(uRECT.left + (i * 100), uRECT.top, uRECT.right - uRECT.left, uRECT.bottom - uRECT.top);
                    graphics.DrawImage(&u1Image, uRect);
                }
                else {
                    Image u2Image(L"./../u2.png");
                    Rect uRect(uRECT.left + (i * 100), uRECT.top, uRECT.right - uRECT.left, uRECT.bottom - uRECT.top);
                    graphics.DrawImage(&u2Image, uRect);
                }
            }
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
        KillTimer(hWnd, 1); // 타이머 종료
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}