#include <Windows.h>
#include <tchar.h>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"ParentClass";
LPCTSTR lpszWindowName = L"7-1";

#define WIDTH 1200
#define HEIGHT 800

#define IDC_RADIO_CIRCLE 101
#define IDC_RADIO_RECTANGLE 102
#define IDC_RADIO_TRIANGLE 103
#define IDC_GROUPBOX 104

#define IDT_TIMER1 2001

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int selectedShape = 0; // 0: none, 1: circle, 2: rectangle, 3: triangle
HWND child_hWnd; // ���� ������ ����
HWND hWndRadioButtons[3]; // ���� ��ư �ڵ��� �迭�� ����

int pointIndex = 0; // ���� �̵��� ���� ���� �ε���
vector<POINT> pathPoints; // ������ ��� ������ ������ ����

void GeneratePathPoints(int shape, int midX, int midY, int size) {
    pathPoints.clear();
    switch (shape) {
    case 1: // Circle
    {
        int radius = size;
        int numPoints = 100; // ���� ��
        for (int i = 0; i < numPoints; ++i) {
            double angle = 2 * 3.14 * i / numPoints;
            POINT p = { midX + static_cast<int>(radius * cos(angle)), midY + static_cast<int>(radius * sin(angle)) };
            pathPoints.push_back(p);
        }
        break;
    }
    case 2: // Rectangle
    {
        int halfSize = size / 2;
        for (int x = -halfSize; x <= halfSize; ++x) {
            pathPoints.push_back({ midX + x, midY - halfSize }); // Top edge
            pathPoints.push_back({ midX + x, midY + halfSize }); // Bottom edge
        }
        for (int y = -halfSize; y <= halfSize; ++y) {
            pathPoints.push_back({ midX - halfSize, midY + y }); // Left edge
            pathPoints.push_back({ midX + halfSize, midY + y }); // Right edge
        }
        break;
    }
    case 3: // Triangle
    {
        POINT vertices[3] = { {midX, midY - size}, {midX - size, midY + size}, {midX + size, midY + size} };
        for (int i = 0; i < 3; ++i) {
            int next = (i + 1) % 3;
            int dx = vertices[next].x - vertices[i].x;
            int dy = vertices[next].y - vertices[i].y;
            int steps = max(abs(dx), abs(dy));
            for (int j = 0; j <= steps; ++j) {
                int x = vertices[i].x + j * dx / steps;
                int y = vertices[i].y + j * dy / steps;
                pathPoints.push_back({ x, y });
            }
        }
        break;
    }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
    WNDCLASSEX WndClass;

    HWND hWnd;
    MSG Message;
    g_hInst = hInstance;

    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_QUESTION);
    WndClass.hCursor = LoadCursor(NULL, IDC_HAND);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH); // �θ� ������ ���� ȸ��
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_QUESTION);
    RegisterClassEx(&WndClass);

    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ���ϵ� ������ ���� ���
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"ChildClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wc);

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 20, 20, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return static_cast<int>(Message.wParam);
}

LRESULT CALLBACK ChildProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    static RECT rect;
    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HBITMAP hOldBitmap;

    switch (iMsg) {
    case WM_CREATE:
        GetClientRect(hWnd, &rect);
        SetTimer(hWnd, IDT_TIMER1, 50, NULL); // 50ms���� Ÿ�̸� �̺�Ʈ �߻�
        break;
    case WM_TIMER:
        if (wParam == IDT_TIMER1) {
            if (!pathPoints.empty()) {
                pointIndex = (pointIndex + 1) % pathPoints.size(); // ���� �ε����� ������Ŵ
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        break;
    case WM_PAINT:
    {
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        hOldBitmap = static_cast<HBITMAP>(SelectObject(mDC, hBitmap));

        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

        Rectangle(mDC, rect.left, rect.top, rect.right, rect.bottom);

        SelectObject(mDC, oldBrush);
        DeleteObject(hBrush);

        // ��� ���ڰ� �� �׸���
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HPEN oldPen = (HPEN)SelectObject(mDC, hPen);

        int midX = (rect.left + rect.right) / 2;
        int midY = (rect.top + rect.bottom) / 2;

        MoveToEx(mDC, midX, rect.top, NULL);
        LineTo(mDC, midX, rect.bottom);

        MoveToEx(mDC, rect.left, midY, NULL);
        LineTo(mDC, rect.right, midY);

        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH); // ���� �귯�� ���
        oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

        if (selectedShape == 1) { // Circle
            Ellipse(mDC, midX - 300, midY - 300, midX + 300, midY + 300);
        }
        else if (selectedShape == 2) { // Rectangle
            Rectangle(mDC, midX - 300, midY - 300, midX + 300, midY + 300);
        }
        else if (selectedShape == 3) { // Triangle
            POINT points[3] = { {midX, midY - 300}, {midX - 300, midY + 300}, {midX + 300, midY + 300} };
            Polygon(mDC, points, 3);
        }

        SelectObject(mDC, oldBrush);
        DeleteObject(hBrush);
        SelectObject(mDC, oldPen);
        DeleteObject(hPen);

        // �� �׸���
        if (!pathPoints.empty()) {
            POINT p = pathPoints[pointIndex];
            hBrush = CreateSolidBrush(RGB(0, 0, 255)); // �Ķ��� �귯�� ���
            oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
            Ellipse(mDC, p.x - 20, p.y - 20, p.x + 20, p.y + 20); // ���� ũ��� 40x40 �ȼ�
            SelectObject(mDC, oldBrush);
            DeleteObject(hBrush);
        }

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);

        SelectObject(mDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        KillTimer(hWnd, IDT_TIMER1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, iMsg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static RECT rect;
    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HBITMAP hOldBitmap;

    switch (uMsg) {
    case WM_CREATE:
        child_hWnd = CreateWindow(L"ChildClass", NULL, WS_CHILD | WS_VISIBLE, 30, 30, 700, 700, hWnd, NULL, g_hInst, NULL);

        // �׷� �ڽ� �߰�
        CreateWindow(L"BUTTON", L"Shapes", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 900, 50, 200, 130, hWnd, (HMENU)IDC_GROUPBOX, g_hInst, NULL);

        // ���� ��ư �߰�
        hWndRadioButtons[0] = CreateWindow(L"BUTTON", L"Circle", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 920, 80, 160, 20, hWnd, (HMENU)IDC_RADIO_CIRCLE, g_hInst, NULL);
        hWndRadioButtons[1] = CreateWindow(L"BUTTON", L"Rectangle", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 920, 110, 160, 20, hWnd, (HMENU)IDC_RADIO_RECTANGLE, g_hInst, NULL);
        hWndRadioButtons[2] = CreateWindow(L"BUTTON", L"Triangle", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 920, 140, 160, 20, hWnd, (HMENU)IDC_RADIO_TRIANGLE, g_hInst, NULL);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_RADIO_CIRCLE:
            selectedShape = 1;
            SendMessage(hWndRadioButtons[0], BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(hWndRadioButtons[1], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtons[2], BM_SETCHECK, BST_UNCHECKED, 0);
            GeneratePathPoints(selectedShape, 350, 350, 300); // �߰��� (350, 350), ũ�� 300���� ��� ����
            pointIndex = 0; // ���� �ε��� �ʱ�ȭ
            InvalidateRect(child_hWnd, NULL, TRUE);
            break;
        case IDC_RADIO_RECTANGLE:
            selectedShape = 2;
            SendMessage(hWndRadioButtons[0], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtons[1], BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(hWndRadioButtons[2], BM_SETCHECK, BST_UNCHECKED, 0);
            GeneratePathPoints(selectedShape, 350, 350, 600); // �߰��� (350, 350), ũ�� 600���� ��� ����
            pointIndex = 0; // ���� �ε��� �ʱ�ȭ
            InvalidateRect(child_hWnd, NULL, TRUE);
            break;
        case IDC_RADIO_TRIANGLE:
            selectedShape = 3;
            SendMessage(hWndRadioButtons[0], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtons[1], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtons[2], BM_SETCHECK, BST_CHECKED, 0);
            GeneratePathPoints(selectedShape, 350, 350, 300); // �߰��� (350, 350), ũ�� 300���� ��� ����
            pointIndex = 0; // ���� �ε��� �ʱ�ȭ
            InvalidateRect(child_hWnd, NULL, TRUE);
            break;
        }
        break;
    case WM_PAINT: {
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        hOldBitmap = static_cast<HBITMAP>(SelectObject(mDC, hBitmap));

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);

        SelectObject(mDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}
