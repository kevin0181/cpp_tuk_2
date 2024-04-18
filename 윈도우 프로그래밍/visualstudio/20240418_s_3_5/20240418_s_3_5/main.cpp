#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <gdiplus.h>
#include <math.h>

#pragma comment (lib, "gdiplus.lib")

using namespace std;
using namespace Gdiplus;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 3-5";
LPCTSTR lpszWindowName = L"실습 3-5";

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

struct Circle {
    RECT rect;
    COLORREF color;
    int positionX = 0;
    int positionY = 0;
    int radius = 0;
    int speed;
    double seta = 0;
    double seta2 = 0;
};

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uid_radius(20, 100);
uniform_int_distribution<int> uid_positionX(0, 1200);
uniform_int_distribution<int> uid_positionY(0, 800);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    
    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static int Timer1Count = 0;

    static vector<Circle> circles; // 원 도형들을 담아두기 위한 배열
    static bool status = false;
    static bool m_s1 = false;
    static bool m_s2 = false;

    static bool Drag = false;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, 1, 10, NULL);
        SetTimer(hWnd, 2, 1, NULL);

        Circle c;
        for (int i = 0; i < 10; ++i) { // 10개의 원 추가
            c.color = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen)); // random color
            c.radius = uid_radius(gen); // random radius
            c.positionX = uid_positionX(gen);
            c.positionY = uid_positionY(gen);
            c.rect = { c.positionX - c.radius,c.positionY - c.radius, c.positionX + c.radius, c.positionY + c.radius };

            circles.push_back(c); // 하나의 원 도형을 만들어 줬으니깐 배열에 추가
        }

        Drag = false;

        break;
    }
    case WM_COMMAND:
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때
        break;
    case WM_RBUTTONDOWN:
    {
        Circle c;
        c.color = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen)); // random color
        c.radius = uid_radius(gen); // random radius
        c.positionX = uid_positionX(gen);
        c.positionY = uid_positionY(gen);
        c.rect = { c.positionX - c.radius,c.positionY - c.radius, c.positionX + c.radius, c.positionY + c.radius };

        for (int i = 0; i < circles.size() - 1; ++i) {
            circles[i] = circles[i + 1];
        }

        circles.push_back(c);
    }
        break;
    case WM_LBUTTONDOWN:
    {
        Drag = true;

        POINT p;
        p.x = LOWORD(lParam);
        p.y = HIWORD(lParam);

        for (int i = 0; i < circles.size(); ++i) {
            if (PtInRect(&circles[i].rect, p)) {
                circles[i].positionX = p.x;
                circles[i].positionY = p.y;
                circles[i].rect = { circles[i].positionX - circles[i].radius, circles[i].positionY - circles[i].radius,
                    circles[i].positionX + circles[i].radius, circles[i].positionY + circles[i].radius };
                break;
            }
        }
        SetCapture(hWnd);
    }
        break;
    case WM_MOUSEMOVE:
    {
        if (Drag && (wParam & MK_LBUTTON)) {
            POINT p;
            p.x = LOWORD(lParam);
            p.y = HIWORD(lParam);

            for (int i = 0; i < circles.size(); ++i) {
                if (PtInRect(&circles[i].rect, p)) {
                    circles[i].positionX = p.x;
                    circles[i].positionY = p.y;
                    circles[i].rect = { circles[i].positionX - circles[i].radius, circles[i].positionY - circles[i].radius,
                        circles[i].positionX + circles[i].radius, circles[i].positionY + circles[i].radius };
                    break;
                }
            }
        }
    }

        break;
    case WM_LBUTTONUP:
        if (Drag) {
            Drag = false;
            ReleaseCapture();
        }
        break;
    case WM_CHAR:
        switch (wParam)
        {
        case '1':
            m_s1 = !m_s1;
            break;
        case '2':
            m_s2 = !m_s2;
            break;
        case 's':
            status = !status;
            break;
        case 'c':

            for (int i = 0; i < 5; ++i) {
                
                BYTE red = 255 - GetRValue(circles[i].color);   // Red 채널 추출 및 반전
                BYTE green = 255 - GetGValue(circles[i].color); // Green 채널 추출 및 반전
                BYTE blue = 255 - GetBValue(circles[i].color);  // Blue 채널 추출 및 반전
                circles[i].color = RGB(red, green, blue);

            }
            
            break;
        case 'r':
        {
            circles.clear();
            Circle c;
            for (int i = 0; i < 10; ++i) { // 10개의 원 추가
                c.color = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen)); // random color
                c.radius = uid_radius(gen); // random radius
                c.positionX = uid_positionX(gen);
                c.positionY = uid_positionY(gen);
                c.rect = { c.positionX - c.radius,c.positionY - c.radius, c.positionX + c.radius, c.positionY + c.radius };

                circles.push_back(c); // 하나의 원 도형을 만들어 줬으니깐 배열에 추가
            }
        }
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
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        SelectObject(mDC, (HBITMAP)hBitmap);
        Rectangle(mDC, 0, 0, rect.right, rect.bottom);

        int setaDegrees[] = { 0, 45, 90, 135, 180 }; // 각도 배열

        for (int i = 0; i < circles.size(); ++i) {
            mBrush = CreateSolidBrush(circles[i].color); // 도형 한개에 저장되어 있던 색을 가져와서 바꾸고
            oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

            Ellipse(mDC, circles[i].rect.left, circles[i].rect.top, circles[i].rect.right, circles[i].rect.bottom); // 도형출력하고
            SelectObject(mDC, oldBrush);
            DeleteObject(mBrush); //삭제하고
            int x, y;
            for (int j = 0; j < 270; j = j + 45) {
                int seta = circles[i].seta;
                seta += j;
                x = circles[i].positionX + (int)(circles[i].radius * cos(seta));
                y = circles[i].positionY + (int)(circles[i].radius * sin(seta));

                MoveToEx(mDC, circles[i].positionX, circles[i].positionY, NULL);
                LineTo(mDC, x, y);
            }

            mBrush = CreateSolidBrush(RGB(255,255,0)); // 도형 한개에 저장되어 있던 색을 가져와서 바꾸고
            oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
            int seta2 = circles[i].seta2;
            x = circles[i].positionX + (int)(circles[i].radius * cos(seta2));
            y = circles[i].positionY + (int)(circles[i].radius * sin(seta2));

            Ellipse(mDC, x - 10, y - 10, x + 10, y + 10);

            SelectObject(mDC, oldBrush);
            DeleteObject(mBrush); //삭제하고
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
            if (!status) {
                if (!m_s1) {
                    for (auto& circle : circles) {
                        circle.seta = (circle.seta + 1.4);  // 각도가 360을 넘지 않도록
                    }
                }
                else {
                    for (auto& circle : circles) {
                        circle.seta = (circle.seta - 1.4);  // 각도가 360을 넘지 않도록
                    }
                }
            }
            break;
        case 2:
            if (!status) {
                if (!m_s2) {
                    for (auto& circle : circles) {
                        circle.seta2 = (circle.seta2 - 0.3);  // 각도가 360을 넘지 않도록
                    }
                }
                else {
                    for (auto& circle : circles) {
                        circle.seta2 = (circle.seta2 + 0.3);  // 각도가 360을 넘지 않도록
                    }
                }
            }
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1); // 타이머 종료
        KillTimer(hWnd, 2); // 타이머 종료
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}