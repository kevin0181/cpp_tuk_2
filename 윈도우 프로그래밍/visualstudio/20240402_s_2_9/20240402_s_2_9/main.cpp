#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"�ǽ� 2-9��";
LPCTSTR lpszWindowName = L"�ǽ� 2-9��";

#define WIDTH 1000
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

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return Message.wParam;

}

class Shape {
public:
    virtual void draw(HDC hdc, RECT rect) const = 0; // ���� ���� �Լ�
    virtual ~Shape() {} // ���� �Ҹ���
};

// �簢�� Ŭ����
class RectangleShape : public Shape {
    RECT rect;
    COLORREF color;

public:
    RectangleShape(RECT rect, COLORREF color) : rect(rect), color(color) {}

    void draw(HDC hdc,RECT rect) const override {
        HBRUSH hBrush = CreateSolidBrush(color);
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);
    }
};

// �ﰢ�� Ŭ����
class RectangleShape : public Shape {
    RECT rect;
    COLORREF color;

public:
    RectangleShape(RECT rect, COLORREF color) : rect(rect), color(color) {}

    void draw(HDC hdc, RECT rect) const override {
        HBRUSH hBrush = CreateSolidBrush(color);
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);
    }
};


#define DEFAULT_COL 16

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uidX(0, 3);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC;
    HPEN hPen, oldPen;
    HBRUSH hBrush, oldBrush;
    static RECT rect;
    static SIZE size;
 
    
    switch (uMsg)
    {
    case WM_CREATE:
        
        break;
    case WM_CHAR:
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &rect);

        hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        oldPen = (HPEN)SelectObject(hDC, hPen);
        hBrush = CreateSolidBrush(RGB(128, 128, 128));
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        Rectangle(hDC, (rect.right / 2) - 100, (rect.bottom / 2) - 100, (rect.right / 2) + 100, (rect.bottom / 2) + 100);

        SelectObject(hDC, oldPen); // ���ڸ� ���ư���
        SelectObject(hDC, oldBrush);
        DeleteObject(hPen); // ���ο� ��ü ����
        DeleteObject(hBrush);

        EndPaint(hWnd, &ps);
        ReleaseDC(hWnd, hDC);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        break;

    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}