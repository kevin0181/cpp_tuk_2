#include <Windows.h>
#include <tchar.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"ParentClass";
LPCTSTR lpszWindowName = L"7-1";

#define WIDTH 1200
#define HEIGHT 800

#define IDC_RADIO_CIRCLE 101
#define IDC_RADIO_RECTANGLE 102
#define IDC_RADIO_TRIANGLE 103
#define IDC_GROUPBOX_SHAPE 104
#define IDC_GROUPBOX_SIZE 105
#define IDC_RADIO_SMALL 106
#define IDC_RADIO_MEDIUM 107
#define IDC_RADIO_LARGE 108
#define IDC_GROUPBOX_COLOR 109
#define IDC_RADIO_SKY 110
#define IDC_RADIO_PINK 111
#define IDC_RADIO_YELLOW 112

#define IDC_BUTTON_START 113
#define IDC_BUTTON_SPEED_UP 114
#define IDC_BUTTON_SLOW_DOWN 115
#define IDC_BUTTON_STOP 116

#define IDC_EDIT_TEXT 117

#define IDT_TIMER1 2001

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int selectedShape = 0; // 0: none, 1: circle, 2: rectangle, 3: triangle
int circleSize = 20; // ���� �ʱ� ũ�� (������)
COLORREF circleColor = RGB(0, 0, 255); // ���� �ʱ� ���� (�Ķ���)
HWND child_hWnd; // ���� ������ ����
HWND hWndRadioButtonsShape[3]; // ���� ���� ���� ��ư �ڵ��� �迭�� ����
HWND hWndRadioButtonsSize[3]; // ũ�� ���� ���� ��ư �ڵ��� �迭�� ����
HWND hWndRadioButtonsColor[3]; // ���� ���� ���� ��ư �ڵ��� �迭�� ����
HWND hWndButtons[4]; // �̵� ���� ��ư �ڵ��� �迭�� ����
HWND hWndEditText; // �ؽ�Ʈ �Է� �ʵ� �ڵ�

int pointIndex = 0; // ���� �̵��� ���� ���� �ε���
vector<POINT> pathPoints; // ������ ��� ������ ������ ����
int timerInterval = 50; // Ÿ�̸� ���� (ms)
vector<pair<wstring, POINT>> inputTexts; // �Էµ� �ؽ�Ʈ�� ��� ��ġ ����

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
        for (int x = -halfSize; x < halfSize; ++x) {
            pathPoints.push_back({ midX + x, midY - halfSize }); // Top edge
        }
        for (int y = -halfSize; y < halfSize; ++y) {
            pathPoints.push_back({ midX + halfSize, midY + y }); // Right edge
        }
        for (int x = halfSize; x > -halfSize; --x) {
            pathPoints.push_back({ midX + x, midY + halfSize }); // Bottom edge
        }
        for (int y = halfSize; y > -halfSize; --y) {
            pathPoints.push_back({ midX - halfSize, midY + y }); // Left edge
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
            hBrush = CreateSolidBrush(circleColor); // ������ ���� �귯�� ���
            oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
            Ellipse(mDC, p.x - circleSize, p.y - circleSize, p.x + circleSize, p.y + circleSize); // ���� ũ�� ����
            SelectObject(mDC, oldBrush);
            DeleteObject(hBrush);
        }

        // �ؽ�Ʈ �׸���
        for (const auto& text : inputTexts) {
            TextOut(mDC, text.second.x, text.second.y, text.first.c_str(), text.first.length());
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
    static int textYPos = 0; // �ؽ�Ʈ ��� Y ��ġ�� �����ϴ� ����

    switch (uMsg) {
    case WM_CREATE:
        child_hWnd = CreateWindow(L"ChildClass", NULL, WS_CHILD | WS_VISIBLE, 30, 30, 700, 700, hWnd, NULL, g_hInst, NULL);

        // �׷� �ڽ� �߰�
        CreateWindow(L"BUTTON", L"Shapes", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 900, 50, 200, 130, hWnd, (HMENU)IDC_GROUPBOX_SHAPE, g_hInst, NULL);
        CreateWindow(L"BUTTON", L"Size", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 900, 200, 200, 130, hWnd, (HMENU)IDC_GROUPBOX_SIZE, g_hInst, NULL);
        CreateWindow(L"BUTTON", L"Color", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 900, 350, 200, 130, hWnd, (HMENU)IDC_GROUPBOX_COLOR, g_hInst, NULL);

        // ���� ���� ���� ��ư �߰�
        hWndRadioButtonsShape[0] = CreateWindow(L"BUTTON", L"Circle", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 920, 80, 160, 20, hWnd, (HMENU)IDC_RADIO_CIRCLE, g_hInst, NULL);
        hWndRadioButtonsShape[1] = CreateWindow(L"BUTTON", L"Rectangle", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 920, 110, 160, 20, hWnd, (HMENU)IDC_RADIO_RECTANGLE, g_hInst, NULL);
        hWndRadioButtonsShape[2] = CreateWindow(L"BUTTON", L"Triangle", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 920, 140, 160, 20, hWnd, (HMENU)IDC_RADIO_TRIANGLE, g_hInst, NULL);

        // ũ�� ���� ���� ��ư �߰�
        hWndRadioButtonsSize[0] = CreateWindow(L"BUTTON", L"Small", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 920, 230, 160, 20, hWnd, (HMENU)IDC_RADIO_SMALL, g_hInst, NULL);
        hWndRadioButtonsSize[1] = CreateWindow(L"BUTTON", L"Medium", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 920, 260, 160, 20, hWnd, (HMENU)IDC_RADIO_MEDIUM, g_hInst, NULL);
        hWndRadioButtonsSize[2] = CreateWindow(L"BUTTON", L"Large", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 920, 290, 160, 20, hWnd, (HMENU)IDC_RADIO_LARGE, g_hInst, NULL);

        // ���� ���� ���� ��ư �߰�
        hWndRadioButtonsColor[0] = CreateWindow(L"BUTTON", L"Sky", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 920, 380, 160, 20, hWnd, (HMENU)IDC_RADIO_SKY, g_hInst, NULL);
        hWndRadioButtonsColor[1] = CreateWindow(L"BUTTON", L"Pink", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 920, 410, 160, 20, hWnd, (HMENU)IDC_RADIO_PINK, g_hInst, NULL);
        hWndRadioButtonsColor[2] = CreateWindow(L"BUTTON", L"Yellow", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 920, 440, 160, 20, hWnd, (HMENU)IDC_RADIO_YELLOW, g_hInst, NULL);

        // �̵� ���� ��ư �߰�
        hWndButtons[0] = CreateWindow(L"BUTTON", L"Start", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 920, 500, 160, 30, hWnd, (HMENU)IDC_BUTTON_START, g_hInst, NULL);
        hWndButtons[1] = CreateWindow(L"BUTTON", L"Speed Up", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 920, 540, 160, 30, hWnd, (HMENU)IDC_BUTTON_SPEED_UP, g_hInst, NULL);
        hWndButtons[2] = CreateWindow(L"BUTTON", L"Slow Down", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 920, 580, 160, 30, hWnd, (HMENU)IDC_BUTTON_SLOW_DOWN, g_hInst, NULL);
        hWndButtons[3] = CreateWindow(L"BUTTON", L"Stop", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 920, 620, 160, 30, hWnd, (HMENU)IDC_BUTTON_STOP, g_hInst, NULL);

        // �ؽ�Ʈ �Է� �ʵ� �߰�
        hWndEditText = CreateWindow(L"EDIT", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT, 30, 740, 700, 30, hWnd, (HMENU)IDC_EDIT_TEXT, g_hInst, NULL);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_RADIO_CIRCLE:
            selectedShape = 1;
            SendMessage(hWndRadioButtonsShape[0], BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(hWndRadioButtonsShape[1], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtonsShape[2], BM_SETCHECK, BST_UNCHECKED, 0);
            GeneratePathPoints(selectedShape, 350, 350, 300); // �߰��� (350, 350), ũ�� 300���� ��� ����
            pointIndex = 0; // ���� �ε��� �ʱ�ȭ
            InvalidateRect(child_hWnd, NULL, TRUE);
            break;
        case IDC_RADIO_RECTANGLE:
            selectedShape = 2;
            SendMessage(hWndRadioButtonsShape[0], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtonsShape[1], BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(hWndRadioButtonsShape[2], BM_SETCHECK, BST_UNCHECKED, 0);
            GeneratePathPoints(selectedShape, 350, 350, 600); // �߰��� (350, 350), ũ�� 600���� ��� ����
            pointIndex = 0; // ���� �ε��� �ʱ�ȭ
            InvalidateRect(child_hWnd, NULL, TRUE);
            break;
        case IDC_RADIO_TRIANGLE:
            selectedShape = 3;
            SendMessage(hWndRadioButtonsShape[0], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtonsShape[1], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtonsShape[2], BM_SETCHECK, BST_CHECKED, 0);
            GeneratePathPoints(selectedShape, 350, 350, 300); // �߰��� (350, 350), ũ�� 300���� ��� ����
            pointIndex = 0; // ���� �ε��� �ʱ�ȭ
            InvalidateRect(child_hWnd, NULL, TRUE);
            break;
        case IDC_RADIO_SMALL:
            circleSize = 10;
            SendMessage(hWndRadioButtonsSize[0], BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(hWndRadioButtonsSize[1], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtonsSize[2], BM_SETCHECK, BST_UNCHECKED, 0);
            InvalidateRect(child_hWnd, NULL, TRUE);
            break;
        case IDC_RADIO_MEDIUM:
            circleSize = 20;
            SendMessage(hWndRadioButtonsSize[0], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtonsSize[1], BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(hWndRadioButtonsSize[2], BM_SETCHECK, BST_UNCHECKED, 0);
            InvalidateRect(child_hWnd, NULL, TRUE);
            break;
        case IDC_RADIO_LARGE:
            circleSize = 30;
            SendMessage(hWndRadioButtonsSize[0], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtonsSize[1], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtonsSize[2], BM_SETCHECK, BST_CHECKED, 0);
            InvalidateRect(child_hWnd, NULL, TRUE);
            break;
        case IDC_RADIO_SKY:
            circleColor = RGB(135, 206, 235); // �ϴû�
            SendMessage(hWndRadioButtonsColor[0], BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(hWndRadioButtonsColor[1], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtonsColor[2], BM_SETCHECK, BST_UNCHECKED, 0);
            InvalidateRect(child_hWnd, NULL, TRUE);
            break;
        case IDC_RADIO_PINK:
            circleColor = RGB(255, 182, 193); // ��ȫ��
            SendMessage(hWndRadioButtonsColor[0], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtonsColor[1], BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(hWndRadioButtonsColor[2], BM_SETCHECK, BST_UNCHECKED, 0);
            InvalidateRect(child_hWnd, NULL, TRUE);
            break;
        case IDC_RADIO_YELLOW:
            circleColor = RGB(255, 255, 0); // �����
            SendMessage(hWndRadioButtonsColor[0], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtonsColor[1], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtonsColor[2], BM_SETCHECK, BST_CHECKED, 0);
            InvalidateRect(child_hWnd, NULL, TRUE);
            break;
        case IDC_BUTTON_START:
            SetTimer(child_hWnd, IDT_TIMER1, timerInterval, NULL); // Ÿ�̸� ����
            break;
        case IDC_BUTTON_SPEED_UP:
            if (timerInterval > 10) {
                timerInterval -= 10;
                SetTimer(child_hWnd, IDT_TIMER1, timerInterval, NULL); // Ÿ�̸� ���� ���̱�
            }
            break;
        case IDC_BUTTON_SLOW_DOWN:
            timerInterval += 10;
            SetTimer(child_hWnd, IDT_TIMER1, timerInterval, NULL); // Ÿ�̸� ���� �ø���
            break;
        case IDC_BUTTON_STOP:
            KillTimer(child_hWnd, IDT_TIMER1); // Ÿ�̸� ���߱�
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
    case WM_KEYDOWN:
        if (wParam == VK_RETURN) {
            wchar_t buffer[256];
            GetWindowText(hWndEditText, buffer, 256);
            POINT pos = { 50, textYPos }; // �ؽ�Ʈ�� ���ϵ� ������ ������ ��� ��ġ�� �׷����� ����
            inputTexts.push_back({ buffer, pos });
            textYPos += 30; // ���� �ؽ�Ʈ ��ġ�� �Ʒ��� �̵�
            SetWindowText(hWndEditText, L""); // �ؽ�Ʈ �ʵ� ����
            InvalidateRect(child_hWnd, NULL, TRUE); // ���ϵ� �����츦 �ٽ� �׸���
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}
