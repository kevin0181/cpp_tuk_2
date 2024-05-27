#include <Windows.h>
#include <tchar.h>
#include <vector>
#include "resource.h"

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

    HWND hDialog = NULL;

    while (GetMessage(&Message, NULL, 0, 0)) {
        if (hDialog == NULL || !IsDialogMessage(hDialog, &Message)) {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
    }

    return static_cast<int>(Message.wParam);
}
struct Line {
    int start_x, start_y;
    int end_x, end_y;
    COLORREF color;
};

bool draw_status = false;

BOOL CALLBACK DialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg) {
    case WM_INITDIALOG: {
        HWND hListBox = GetDlgItem(hDlg, IDC_LIST4);
        if (hListBox) {
            // List Box에 항목 추가
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Hello");
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"TUKOREA");
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Winapi");
        }
        else {
            MessageBox(hDlg, L"Could not find list box in dialog.", L"Error", MB_OK | MB_ICONERROR);
        }
        return TRUE; // WM_INITDIALOG 메시지를 처리하고 TRUE 반환
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            EndDialog(hDlg, IDOK);
            return TRUE; // IDOK 처리 후 TRUE 반환
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            return TRUE; // IDCANCEL 처리 후 TRUE 반환
        case IDC_RADIO1:
            break;
        case IDC_RADIO2:
            break;
        case IDC_RADIO3:
            break;
        case IDC_RADIO4:
            break;
        case IDC_RADIO5:
            break;
        case IDC_BUTTON1:
            draw_status = true;
            break;
        case IDC_BUTTON2:
            break;
        case IDC_BUTTON3:
            break;
        case IDC_BUTTON4:
            break;
        case IDC_BUTTON5:
            break;
        case IDC_BUTTON6:
            break;
        case IDC_BUTTON7:
            break;
        }
        break;
    case WM_CLOSE:
        EndDialog(hDlg, IDCANCEL);
        return TRUE; // WM_CLOSE 처리 후 TRUE 반환
    }
    return FALSE; // 나머지 메시지는 FALSE 반환
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HBITMAP hOldBitmap;
    RECT rect;

    static vector<Line> lines;
    static bool Drag = false;
    static HWND hDialog = NULL;

    switch (uMsg) {
    case WM_CREATE:
        break;
    case WM_LBUTTONUP:
        if (Drag && draw_status) {
            Drag = false;
            lines.back().end_x = LOWORD(lParam);
            lines.back().end_y = HIWORD(lParam);

            if (lines.size() == 7) {
                Line line;
                line.start_x = lines[6].end_x;
                line.start_y = lines[6].end_y;
                line.end_x = lines[0].start_x;
                line.end_y = lines[0].start_y;
                line.color = RGB(0, 0, 0);
                lines.push_back(line);
            }

            InvalidateRect(hWnd, NULL, false);
        }
        break;
    case WM_LBUTTONDOWN:

        if (lines.size() >= 7) {
            break;
        }

        if (draw_status) {
            Drag = true;
            Line line;

            if (lines.size() != 0) {
                line.start_x = lines[lines.size() - 1].end_x;
                line.start_y = lines[lines.size() - 1].end_y;
            }
            else {
                line.start_x = LOWORD(lParam);
                line.start_y = HIWORD(lParam);
            }
            line.end_x = LOWORD(lParam);
            line.end_y = HIWORD(lParam);
            line.color = RGB(0, 0, 0);
            lines.push_back(line);
        }
        break;
    case WM_MOUSEMOVE:
        if (Drag && draw_status) {
            lines.back().end_x = LOWORD(lParam);
            lines.back().end_y = HIWORD(lParam);
            InvalidateRect(hWnd, NULL, false);
        }
        break;
    case WM_CHAR:
        if (wParam == 'q') {
            if (!hDialog) {
                hDialog = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)DialogProc);
                ShowWindow(hDialog, SW_SHOW);
            }
            else {
                SetForegroundWindow(hDialog);
            }
        }
        break;
    case WM_PAINT: {
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        GetClientRect(hWnd, &rect);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        hOldBitmap = static_cast<HBITMAP>(SelectObject(mDC, hBitmap));

        FillRect(mDC, &rect, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));

        for (const auto& line : lines) {
            HPEN hPen = CreatePen(PS_SOLID, 2, line.color);
            HPEN hOldPen = (HPEN)SelectObject(mDC, hPen);
            MoveToEx(mDC, line.start_x, line.start_y, NULL);
            LineTo(mDC, line.end_x, line.end_y);
            SelectObject(mDC, hOldPen);
            DeleteObject(hPen);
        }

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);

        SelectObject(mDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        if (hDialog) {
            DestroyWindow(hDialog);
        }
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}