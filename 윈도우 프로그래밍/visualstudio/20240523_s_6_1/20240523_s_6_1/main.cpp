#include <Windows.h>
#include <tchar.h>
#include <random>
#include <atlimage.h>
#include "resource1.h"

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"INVERSUS";
LPCTSTR lpszWindowName = L"INVERSUS";

#define WIDTH 1200
#define HEIGHT 1000

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

enum class GraphicsType {
    None,
    Sin,
    Zigzag,
    Spring,
    Pie
};

struct GraphicsData {
    COLORREF lineColor;
    int lineWidth;
    GraphicsType graphType;
};

GraphicsData graphData = { RGB(0, 0, 255), 2, GraphicsType::None }; // 기본 값 설정

void DrawSinCurve(HDC& hdc, RECT rect, GraphicsData graphData) {
    HPEN hPen = CreatePen(PS_SOLID, graphData.lineWidth, graphData.lineColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    double xScale = 2.0 * 3.14 / width;
    double yScale = height / 4.0;

    MoveToEx(hdc, 0, height / 2, NULL);
    for (int x = 0; x < width; ++x) {
        int y = static_cast<int>(height / 2 - sin(4 * x * xScale) * yScale);
        LineTo(hdc, x, y);
    }

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void DrawZigzag(HDC& hdc, RECT rect, GraphicsData graphData) {
    HPEN hPen = CreatePen(PS_SOLID, graphData.lineWidth, graphData.lineColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    int step = width / 20;

    MoveToEx(hdc, 0, height / 2, NULL);
    for (int x = 0; x < width; x += step) {
        LineTo(hdc, x, (x / step) % 2 == 0 ? height / 4 : (3 * height) / 4);
    }

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void DrawSpringCurve(HDC hdc, RECT rect, GraphicsData graphData) {
    HPEN hPen = CreatePen(PS_SOLID, graphData.lineWidth, graphData.lineColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    int radius = 50; // radius of each circle
    int spacing = radius / 2; // overlap amount

    for (int x = spacing; x < rect.right; x += spacing) {
        Arc(hdc, x - radius, rect.bottom / 2 - radius, x + radius, rect.bottom / 2 + radius, 0, 0, 0, 0);
    }

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void DrawPieChart(HDC hdc, RECT rect, GraphicsData graphData) {
    HPEN hPen = CreatePen(PS_SOLID, graphData.lineWidth, graphData.lineColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    int step = width / 20;
    int amplitude = height / 4;

    MoveToEx(hdc, 0, height / 2, NULL);
    for (int x = 0; x < width; x += step) {
        LineTo(hdc, x, (x / step) % 2 == 0 ? height / 2 - amplitude : height / 2 + amplitude);
        LineTo(hdc, x + step, (x / step) % 2 == 0 ? height / 2 - amplitude : height / 2 + amplitude);
    }

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void DrawGraphics(HDC& hdc, RECT rect, GraphicsData graphData) {
    switch (graphData.graphType) {
    case GraphicsType::Sin:
        DrawSinCurve(hdc, rect, graphData);
        break;
    case GraphicsType::Zigzag:
        DrawZigzag(hdc, rect, graphData);
        break;
    case GraphicsType::Spring:
        DrawSpringCurve(hdc, rect, graphData);
        break;
    case GraphicsType::Pie:
        DrawPieChart(hdc, rect, graphData);
        break;
    default:
        break;
    }
}

BOOL CALLBACK DialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg) {
    case WM_INITDIALOG:
        return TRUE; // WM_INITDIALOG 메시지를 처리하고 TRUE 반환
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            // MessageBox(hDlg, L"test", L"test", MB_OK);
            return TRUE; // IDOK 처리 후 TRUE 반환
        case IDCANCEL:
            EndDialog(hDlg, 0);
            return TRUE; // IDCANCEL 처리 후 TRUE 반환
        case IDC_RADIO1:
            graphData.graphType = GraphicsType::Sin;
            InvalidateRect(GetParent(hDlg), NULL, false); // 부모 창 무효화
            return TRUE;
        case IDC_RADIO2:
            graphData.graphType = GraphicsType::Zigzag;
            InvalidateRect(GetParent(hDlg), NULL, false); // 부모 창 무효화
            return TRUE;
        case IDC_RADIO3:
            graphData.graphType = GraphicsType::Spring;
            InvalidateRect(GetParent(hDlg), NULL, TRUE); // 부모 창 무효화
            return TRUE;
        case IDC_RADIO4:
            graphData.graphType = GraphicsType::Pie;
            InvalidateRect(GetParent(hDlg), NULL, TRUE); // 부모 창 무효화
            return TRUE;
        }
    case WM_CLOSE:
        EndDialog(hDlg, 0);
        return TRUE; // WM_CLOSE 처리 후 TRUE 반환
    }
    return FALSE; // 나머지 메시지는 FALSE 반환
}


random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_sub(1, 5);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static RECT rect;

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HBITMAP hOldBitmap;

    static bool drawSin = false;

    switch (uMsg) {
    case WM_CREATE:
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:
        break;
    case WM_CHAR:
        if (wParam == 'q') {
            DialogBox(g_hInst, MAKEINTRESOURCE(DIALOG_1), hWnd, (DLGPROC)DialogProc);
        }
        break;
    case WM_PAINT: {
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        hOldBitmap = static_cast<HBITMAP>(SelectObject(mDC, hBitmap));
        GetClientRect(hWnd, &rect);

        FillRect(mDC, &rect, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));

        DrawGraphics(mDC, rect, graphData);

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);

        SelectObject(mDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        //InvalidateRect(hWnd, NULL, FALSE); // Force a repaint
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}
