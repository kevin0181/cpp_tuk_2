#include <Windows.h>
#include <tchar.h>
#include <random>
#include <atlimage.h>

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int selectedShape = 0; // 0: none, 1: circle, 2: rectangle, 3: triangle
HWND child_hWnd; // 전역 변수로 선언
HWND hWndRadioButtons[3]; // 라디오 버튼 핸들을 배열로 선언

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
    WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH); // 부모 윈도우 배경색 회색
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
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 차일드 윈도우 배경색 흰색
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
    case WM_LBUTTONDOWN: //--- 차일드 윈도우에 좌측 마우스 버튼을 누른 경우 
        //MessageBox(hWnd, L"Left Mouse Button", L"Mouse Test ", MB_OK);
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

        // 가운데 십자가 선 그리기
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HPEN oldPen = (HPEN)SelectObject(mDC, hPen);

        int midX = (rect.left + rect.right) / 2;
        int midY = (rect.top + rect.bottom) / 2;

        MoveToEx(mDC, midX, rect.top, NULL);
        LineTo(mDC, midX, rect.bottom);

        MoveToEx(mDC, rect.left, midY, NULL);
        LineTo(mDC, rect.right, midY);


        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH); // 투명 브러시 사용
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

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);

        SelectObject(mDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, iMsg, wParam, lParam);
    }
    return 0;
}

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_speed(3, 7);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static RECT rect;
    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HBITMAP hOldBitmap;

    switch (uMsg) {
    case WM_CREATE:
        child_hWnd = CreateWindow(L"ChildClass", NULL, WS_CHILD | WS_VISIBLE, 30, 30, 700, 700, hWnd, NULL, g_hInst, NULL);

        // 그룹 박스 추가
        CreateWindow(L"BUTTON", L"Shapes", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 900, 50, 200, 130, hWnd, (HMENU)IDC_GROUPBOX, g_hInst, NULL);

        // 라디오 버튼 추가
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
            InvalidateRect(child_hWnd, NULL, TRUE);
            break;
        case IDC_RADIO_RECTANGLE:
            selectedShape = 2;
            SendMessage(hWndRadioButtons[0], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtons[1], BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(hWndRadioButtons[2], BM_SETCHECK, BST_UNCHECKED, 0);
            InvalidateRect(child_hWnd, NULL, TRUE);
            break;
        case IDC_RADIO_TRIANGLE:
            selectedShape = 3;
            SendMessage(hWndRadioButtons[0], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtons[1], BM_SETCHECK, BST_UNCHECKED, 0);
            SendMessage(hWndRadioButtons[2], BM_SETCHECK, BST_CHECKED, 0);
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
