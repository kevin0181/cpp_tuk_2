#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 4-3";
LPCTSTR lpszWindowName = L"실습 4-3";

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
uniform_int_distribution<int> uid_shape(0, 5);
uniform_int_distribution<int> uid_color(0, 7);

struct Rec {
    RECT rect;
    COLORREF color;
    bool status;

    Rec() {
        switch (uid_color(gen))
        {
        case 0:
            color = RGB(0, 128, 128);
            break;
        case 1:
            color = RGB(240, 128, 128);
            break;
        case 2:
            color = RGB(204, 255, 0);
            break;
        case 3:
            color = RGB(139, 0, 139);
            break;
        case 4:
            color = RGB(255, 221, 0);
            break;
        case 5:
            color = (135, 206, 235);
            break;
        case 6:
            color = RGB(255, 182, 193);
            break;
        case 7:
            color = RGB(255, 253, 208);
            break;
        default:
            break;
        }
    }

};

struct Shape {
    int x = 0;
    int y = 0;
    RECT mapRect;
    bool status;
    vector<Rec> recs;
    int shape_status;
    Shape(int shape_s, RECT& mapRect) :shape_status(shape_s), mapRect(mapRect), status(true) {

        switch (uid_shape(gen))
        {
        case 0:
            for (int i = 0; i < 4; ++i) {
                Rec r;
                recs.push_back(r);
            }
            recs[0].rect = { mapRect.left, 0, mapRect.left + 50, 50 };
            recs[1].rect = { mapRect.left + 50, 0, mapRect.left + 100, 50 };
            recs[2].rect = { mapRect.left, 50, mapRect.left + 50,100 };
            recs[3].rect = { mapRect.left + 50, 50, mapRect.left + 100, 100 };
            break;
        case 1:
            for (int i = 0; i < 3; ++i) {
                Rec r;
                recs.push_back(r);
            }
            recs[0].rect = { mapRect.left, 0, mapRect.left + 50, 50 };
            recs[1].rect = { mapRect.left + 50, 0, mapRect.left + 100, 50 };
            recs[2].rect = { mapRect.left + 100, 0, mapRect.left + 150, 50 };
            break;
        case 2:
            for (int i = 0; i < 4; ++i) {
                Rec r;
                recs.push_back(r);
            }
            recs[0].rect = { mapRect.left, 0, mapRect.left + 50, 50 };
            recs[1].rect = { mapRect.left + 50, 0, mapRect.left + 100, 50 };
            recs[2].rect = { mapRect.left + 50, 50, mapRect.left + 100, 100 };
            recs[3].rect = { mapRect.left + 100, 50, mapRect.left + 150, 100 };
            break;
        case 3:
            for (int i = 0; i < 4; ++i) {
                Rec r;
                recs.push_back(r);
            }
            recs[0].rect = { mapRect.left, 0, mapRect.left + 50, 50 };
            recs[1].rect = { mapRect.left + 50, 0, mapRect.left + 100, 50 };
            recs[2].rect = { mapRect.left + 100, 0, mapRect.left + 150, 50 };
            recs[3].rect = { mapRect.left, 50, mapRect.left + 50, 100 };
            break;
        case 4:
            for (int i = 0; i < 5; ++i) {
                Rec r;
                recs.push_back(r);
            }
            recs[0].rect = { mapRect.left, 0, mapRect.left + 50, 50 };
            recs[1].rect = { mapRect.left + 50, 0, mapRect.left + 100, 50 };
            recs[2].rect = { mapRect.left + 100, 0, mapRect.left + 150, 50 };
            recs[3].rect = { mapRect.left, 50, mapRect.left + 50, 100 };
            recs[4].rect = { mapRect.left + 100, 50, mapRect.left + 150, 100 };
            break;
        case 5:
            for (int i = 0; i < 4; ++i) {
                Rec r;
                recs.push_back(r);
            }
            recs[0].rect = { mapRect.left, 0, mapRect.left + 50, 50 };
            recs[1].rect = { mapRect.left + 50, 0, mapRect.left + 100, 50 };
            recs[2].rect = { mapRect.left + 100, 0, mapRect.left + 150, 50 };
            recs[3].rect = { mapRect.left + 50, 50, mapRect.left + 100, 100 };
            break;
        default:
            break;
        }

    };

    void print_r(HDC& mDC) {
        for (int i = 0; i < recs.size(); ++i) {
            HBRUSH mBrush = CreateSolidBrush(recs[i].color);
            HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
            HPEN mPen = CreatePen(PS_SOLID, 0.5, RGB(0, 0, 0));
            HPEN oldPen = (HPEN)SelectObject(mDC, mPen);

            //OffsetRect(&recs[i].rect, (x * 50), (y * 50));
            Rectangle(mDC, recs[i].rect.left + (x * 50), recs[i].rect.top + (y * 50), recs[i].rect.right + (x * 50), recs[i].rect.bottom + (y * 50));
            
            SelectObject(mDC, oldBrush);
            SelectObject(mDC, oldPen);
            DeleteObject(mBrush);
            DeleteObject(mPen);
        }
    }

};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static int Timer1Count = 0;
    static RECT mapRect;
    static bool game_status = false;
    static vector<Shape> listShape;
    static vector<Shape> makeShape;
    static bool make_status = true;
    switch (uMsg)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, 1, 500, NULL);
        break;
    }
    case WM_COMMAND:
        switch (wParam)
        {
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때
    {
        InvalidateRect(hWnd, NULL, false);  // 윈도우를 다시 그립니다.
        break;
    }
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



        { //map
            mapRect = {
                rect.right / 2 - 425,
                -1,
                rect.right / 2 + 125,
                800
            };
            mBrush = (HBRUSH)GetStockObject(BLACK_BRUSH); // 투명한 브러시 사용
            oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

            FrameRect(mDC, &mapRect, mBrush); // 사각형의 테두리만 그림

            SelectObject(mDC, oldBrush);
            DeleteObject(mBrush);
        }

        if (make_status) {
            for (int i = 0; i < 2; ++i) {
                Shape ms(uid_shape(gen), mapRect);
                ms.x = 4;
                makeShape.push_back(ms);
            }
            make_status = false;
        }


        //놓인 출력
        for (int i = 0; i < listShape.size(); ++i) {
            listShape[i].print_r(mDC);
        }

        makeShape[1].x = 14;
        makeShape[1].y = 2;
        //움직이는 도형
        for (int i = 0; i < makeShape.size(); ++i) {
            makeShape[i].print_r(mDC);
        }
        
        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);
        DeleteObject(hBitmap); // 생성한 비트맵 삭제
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        switch (wParam)
        {
        case 1:
        {
            bool status = true;
            for (auto& r : makeShape[0].recs) {
                if ((r.rect.bottom + (makeShape[0].y * 50)) == mapRect.bottom) {
                    status = false;
                    listShape.push_back(makeShape[0]);
                    makeShape[0] = makeShape[1];
                    makeShape[0].x = 3;
                    makeShape[0].y = 0;
                    makeShape.pop_back();

                    Shape ms(uid_shape(gen), mapRect);
                    ms.x = 4;
                    makeShape.push_back(ms);
                }
            }
            if (status)
                makeShape[0].y++;
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