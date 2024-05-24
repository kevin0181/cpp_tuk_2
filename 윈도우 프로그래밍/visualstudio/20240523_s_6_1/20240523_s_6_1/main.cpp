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

#define TIMER_ID_X 1
#define TIMER_ID_Y 2
#define TIMER_ID_CIRCLE 3

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

COLORREF InvertColor(COLORREF color) {
    BYTE r = 255 - GetRValue(color);
    BYTE g = 255 - GetGValue(color);
    BYTE b = 255 - GetBValue(color);
    return RGB(r, g, b);
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
    int xOffset; // 새로운 이동 오프셋 값
    int yOffset; // y축 이동 오프셋 값
    int circlePos; // 원의 현재 x 위치
    bool circleMoving; // 원 이동 상태
};

GraphicsData graphData = { RGB(0, 0, 255), 2, GraphicsType::None, 0, 0, 0, false }; // 기본 값 설정

void DrawCircle(HDC& hdc, int x, int y) {
    int radius = 10;
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
}

void DrawSinCurve(HDC& hdc, RECT rect, GraphicsData graphData) {
    HPEN hPen = CreatePen(PS_SOLID, graphData.lineWidth, graphData.lineColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    double xScale = 2.0 * 3.14 / width;
    double yScale = height / 4.0;

    MoveToEx(hdc, -graphData.xOffset, height / 2 - graphData.yOffset, NULL); // xOffset 및 yOffset 적용
    for (int x = 0; x < width; ++x) {
        int y = static_cast<int>(height / 2 - sin(4 * (x + graphData.xOffset) * xScale) * yScale - graphData.yOffset);
        LineTo(hdc, x, y);
    }

    if (graphData.circleMoving) {
        int circleY = static_cast<int>(height / 2 - sin(4 * (graphData.circlePos + graphData.xOffset) * xScale) * yScale - graphData.yOffset);
        DrawCircle(hdc, graphData.circlePos, circleY);
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
    int effectiveXOffset = graphData.xOffset % (2 * step); // xOffset을 step의 배수로 제한

    MoveToEx(hdc, -effectiveXOffset, height / 2 - graphData.yOffset, NULL); // xOffset 및 yOffset 적용
    for (int x = -effectiveXOffset; x < width; x += step) {
        LineTo(hdc, x, ((x + effectiveXOffset) / step) % 2 == 0 ? height / 4 - graphData.yOffset : (3 * height) / 4 - graphData.yOffset);
    }

    if (graphData.circleMoving) {
        int stepPos = (graphData.circlePos + effectiveXOffset) / step;
        int circleY = (stepPos % 2 == 0) ? height / 4 - graphData.yOffset : (3 * height) / 4 - graphData.yOffset;
        DrawCircle(hdc, graphData.circlePos, circleY);
    }

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void DrawSpringCurve(HDC hdc, RECT rect, GraphicsData graphData) {
    HPEN hPen = CreatePen(PS_SOLID, graphData.lineWidth, graphData.lineColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    int radius = 50; // radius of each circle
    int spacing = radius / 2; // overlap amount

    for (int x = spacing - graphData.xOffset; x < rect.right; x += spacing) {
        Arc(hdc, x - radius, rect.bottom / 2 - radius - graphData.yOffset, x + radius, rect.bottom / 2 + radius - graphData.yOffset, 0, 0, 0, 0);
    }

    if (graphData.circleMoving) {
        int circleX = (graphData.circlePos + graphData.xOffset) % (spacing * 2);
        int circleY = rect.bottom / 2 + sin(circleX * 3.14 / spacing) * (radius / 2) - graphData.yOffset;
        DrawCircle(hdc, graphData.circlePos, circleY);
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
    int effectiveXOffset = graphData.xOffset % (2 * step); // xOffset을 step의 배수로 제한

    MoveToEx(hdc, -effectiveXOffset, height / 2 - graphData.yOffset, NULL); // xOffset 및 yOffset 적용
    for (int x = -effectiveXOffset; x < width; x += step) {
        LineTo(hdc, x, ((x + effectiveXOffset) / step) % 2 == 0 ? height / 2 - amplitude - graphData.yOffset : height / 2 + amplitude - graphData.yOffset);
        LineTo(hdc, x + step, ((x + effectiveXOffset) / step) % 2 == 0 ? height / 2 - amplitude - graphData.yOffset : height / 2 + amplitude - graphData.yOffset);
    }

    if (graphData.circleMoving) {
        int stepPos = (graphData.circlePos + effectiveXOffset) / step;
        int circleY = (stepPos % 2 == 0) ? height / 2 - amplitude - graphData.yOffset : height / 2 + amplitude - graphData.yOffset;
        DrawCircle(hdc, graphData.circlePos, circleY);
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
        case IDC_CHECK1:
            graphData.lineColor = RGB(0, 255, 255); // Cyan
            InvalidateRect(GetParent(hDlg), NULL, TRUE); // 부모 창 무효화
            return TRUE;
        case IDC_CHECK2:
            graphData.lineColor = RGB(255, 0, 255); // Magenta
            InvalidateRect(GetParent(hDlg), NULL, TRUE); // 부모 창 무효화
            return TRUE;
        case IDC_CHECK3:
            graphData.lineColor = RGB(255, 255, 0); // Yellow
            InvalidateRect(GetParent(hDlg), NULL, TRUE); // 부모 창 무효화
            return TRUE;
        case IDC_CHECK4:
            graphData.lineColor = InvertColor(graphData.lineColor); // 반전 색상
            InvalidateRect(GetParent(hDlg), NULL, TRUE); // 부모 창 무효화
            return TRUE;
        case IDC_BUTTON4:
            SetTimer(GetParent(hDlg), TIMER_ID_X, 50, NULL); // x축 타이머 설정
            KillTimer(GetParent(hDlg), TIMER_ID_Y); // y축 타이머 중지
            graphData.circleMoving = false;
            return TRUE;
        case IDC_BUTTON5:
            SetTimer(GetParent(hDlg), TIMER_ID_Y, 50, NULL); // y축 타이머 설정
            KillTimer(GetParent(hDlg), TIMER_ID_X); // x축 타이머 중지
            graphData.circleMoving = false;
            return TRUE;
        case IDC_BUTTON3:
            KillTimer(GetParent(hDlg), TIMER_ID_Y); // y축 타이머 중지
            KillTimer(GetParent(hDlg), TIMER_ID_X); // x축 타이머 중지
            graphData.circleMoving = false;
            return TRUE;
        case IDC_BUTTON7:
            graphData.xOffset = 0; // xOffset 초기화
            graphData.yOffset = 0; // yOffset 초기화
            graphData.circlePos = 0; // circlePos 초기화
            graphData.circleMoving = false;
            InvalidateRect(GetParent(hDlg), NULL, TRUE); // 부모 창 무효화
            return TRUE;
        case IDC_BUTTON6:
            graphData.circlePos = 0; // 원의 시작 위치 초기화
            graphData.circleMoving = true;
            SetTimer(GetParent(hDlg), TIMER_ID_CIRCLE, 50, NULL); // 원 이동 타이머 설정
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
        switch (wParam) {
        case TIMER_ID_X:
            graphData.xOffset += 5; // xOffset 증가
            InvalidateRect(hWnd, NULL, FALSE); // 창 무효화
            break;
        case TIMER_ID_Y:
            graphData.yOffset += 5; // yOffset 증가
            InvalidateRect(hWnd, NULL, FALSE); // 창 무효화
            break;
        case TIMER_ID_CIRCLE:
            graphData.circlePos += 5; // 원의 위치 증가
            if (graphData.circlePos > rect.right) { // 원이 화면을 벗어나면 중지
                KillTimer(hWnd, TIMER_ID_CIRCLE);
                graphData.circleMoving = false;
            }
            InvalidateRect(hWnd, NULL, FALSE); // 창 무효화
            break;
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, TIMER_ID_X);
        KillTimer(hWnd, TIMER_ID_Y);
        KillTimer(hWnd, TIMER_ID_CIRCLE);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}
