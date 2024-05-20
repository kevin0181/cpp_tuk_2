#include <Windows.h>
#include <tchar.h>
#include <random>
#include <atlimage.h>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"INVERSUS";
LPCTSTR lpszWindowName = L"INVERSUS";

#define WIDTH 900
#define HEIGHT 1000
#define SCROLL_SPEED 5 // Speed of the scrolling

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
    HWND hWnd;
    MSG Message;
    WNDCLASSEX WndClass;
    g_hInst = hInstance;

    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
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

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
        20, 20, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return static_cast<int>(Message.wParam);
}

struct Circle {
    RECT rect;
    COLORREF color;
    int shadow = 10;
    void draw(HDC& mDC) {
        HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 200));
        HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

        Ellipse(mDC, rect.left, rect.top, rect.right, rect.bottom);

        SelectObject(mDC, oldBrush);
        DeleteObject(hBrush);
    }

    void draw_shadow(HDC& mDC) {
        // Creating a semi-transparent brush for the shadow
        HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
        HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

        // To remove the outline, we need to set the pen to NULL
        HPEN hPen = (HPEN)SelectObject(mDC, GetStockObject(NULL_PEN));

        Ellipse(mDC, rect.left + shadow, rect.top + shadow, rect.right + shadow, rect.bottom + shadow);

        SelectObject(mDC, hPen);
        SelectObject(mDC, oldBrush);
        DeleteObject(hBrush);
    }
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static int scrollPos = 0;
    static int scrollPos2 = 0;
    static RECT rect;
    static RECT mapRECT;
    static CImage backImage;

    static CImage backImage2;

    static Circle main;
    static vector<Circle> sub;

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HBITMAP hOldBitmap;

    switch (uMsg) {
    case WM_CREATE:

        GetClientRect(hWnd, &rect);

        if (backImage.Load(L"1.png") != S_OK) {
            MessageBox(hWnd, L"Failed to load image", L"Error", MB_OK | MB_ICONERROR);
            PostQuitMessage(1);
        }
        if (backImage2.Load(L"2.png") != S_OK) {
            MessageBox(hWnd, L"Failed to load image", L"Error", MB_OK | MB_ICONERROR);
            PostQuitMessage(1);
        }

        mapRECT = rect;
        mapRECT.left += 100;
        mapRECT.right -= 100;

        SetTimer(hWnd, 1, 30, NULL); // Set a timer to trigger every 30 milliseconds
        SetTimer(hWnd, 2, 20, NULL); // Set a timer to trigger every 30 milliseconds

        main.rect = { 0,0,50,50 };
        OffsetRect(&main.rect, mapRECT.right / 2 + 25, mapRECT.bottom - 100);

        break;
    case WM_PAINT: {
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        hOldBitmap = static_cast<HBITMAP>(SelectObject(mDC, hBitmap));

        FillRect(mDC, &rect, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));

        int imageHeight = backImage.GetHeight();
        int yOffset = scrollPos % imageHeight;

        int imageHeight2 = backImage2.GetHeight();
        int yOffset2 = scrollPos2 % imageHeight;

        // Draw the image twice to create a continuous scrolling effect
        backImage.Draw(mDC, 0, yOffset - imageHeight, rect.right, imageHeight);
        backImage.Draw(mDC, 0, yOffset, rect.right, imageHeight);

        // Draw the image twice to create a continuous scrolling effect
        backImage2.Draw(mDC, 50, yOffset - imageHeight, rect.right - 100, imageHeight);
        backImage2.Draw(mDC, 50, yOffset, rect.right - 100, imageHeight);

        main.draw_shadow(mDC);
        main.draw(mDC);

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);

        SelectObject(mDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        switch (wParam)
        {
        case 1:
            scrollPos += SCROLL_SPEED; // Update the scroll position
            break;
        case 2:
            scrollPos2 += SCROLL_SPEED; // Update the scroll position
            break;
        default:
            break;
        }

        InvalidateRect(hWnd, NULL, FALSE); // Force a repaint
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1); // Destroy the timer
        KillTimer(hWnd, 1); // Destroy the timer
        backImage.Destroy(); // Clean up the image object
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}
