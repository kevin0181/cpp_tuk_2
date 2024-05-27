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
#define RECT_WIDTH 150
#define RECT_HEIGHT 150
#define TIMER_ID 2

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

struct Shape {
    int status = -1;
    RECT rect;
    COLORREF color;

    void print(HDC& mDC, COLORREF color_s, const TCHAR* buffer) {
        HPEN hPen = CreatePen(PS_SOLID, 2, color_s);
        HPEN hOldPen = (HPEN)SelectObject(mDC, hPen);
        HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
        HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
        if (status == 0) {
            Ellipse(mDC, rect.left, rect.top, rect.right, rect.bottom);
        }
        else if (status == 1) {
            Rectangle(mDC, rect.left, rect.top, rect.right, rect.bottom);
        }
        SelectObject(mDC, oldBrush);
        SelectObject(mDC, hOldPen);
        DeleteObject(hPen);

        // Draw the buffer text next to the shape
        SetBkMode(mDC, TRANSPARENT);
        TextOut(mDC, rect.right + 5, rect.top, buffer, lstrlen(buffer));
    }
};

struct Line {
    int start_x, start_y;
    int end_x, end_y;
    COLORREF color;
};

static vector<Line> lines;
bool draw_status = false;
bool move_status = false;

COLORREF select_color = RGB(0, 0, 0);
Shape shape;

TCHAR buffer[256] = L"";
RECT string_rect;

int current_line_index = 0;
double progress = 0.0;

bool reverse_status = false;

RECT dialogRect = { 50, 50, 50 + RECT_WIDTH, 50 + RECT_HEIGHT };
int dx = 5; // Speed of movement

void insert_color(vector<Line>& lines, COLORREF color) {
    for (auto& line : lines) {
        line.color = color;
    }
}

BOOL CALLBACK DialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg) {
    case WM_INITDIALOG: {
        HWND hListBox = GetDlgItem(hDlg, IDC_LIST4);
        if (hListBox) {
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Hello");
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"TUKOREA");
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)L"Winapi");
        }
        else {
            MessageBox(hDlg, L"Could not find list box in dialog.", L"Error", MB_OK | MB_ICONERROR);
        }
        return TRUE;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_LIST4:
            if (HIWORD(wParam) == LBN_SELCHANGE) {
                HWND hListBox = GetDlgItem(hDlg, IDC_LIST4);
                if (hListBox) {
                    int index = (int)SendMessage(hListBox, LB_GETCURSEL, 0, 0);
                    if (index != LB_ERR) {
                        SendMessage(hListBox, LB_GETTEXT, index, (LPARAM)buffer);
                    }
                    else {
                        MessageBox(hDlg, L"No item selected.", L"Error", MB_OK | MB_ICONERROR);
                    }
                }
                return TRUE;
            }
            break;
        case IDCANCEL:
            ShowWindow(hDlg, SW_HIDE);
            return TRUE;
        case IDC_RADIO1:
            shape.status = 0;
            break;
        case IDC_RADIO2:
            shape.status = 1;
            break;
        case IDC_RADIO3:
            select_color = RGB(255, 0, 0);
            insert_color(lines, select_color);
            InvalidateRect(GetParent(hDlg), NULL, false);
            break;
        case IDC_RADIO4:
            select_color = RGB(0, 255, 0);
            insert_color(lines, select_color);
            InvalidateRect(GetParent(hDlg), NULL, false);
            break;
        case IDC_RADIO5:
            select_color = RGB(0, 0, 255);
            insert_color(lines, select_color);
            InvalidateRect(GetParent(hDlg), NULL, false);
            break;
        case IDC_BUTTON1:
            SetTimer(GetParent(hDlg), 1, 10, NULL); // Adjust the timer interval as needed
            draw_status = true;
            break;
        case IDC_BUTTON2:
            move_status = !move_status;
            if (move_status) {
                current_line_index = 0;
                progress = 0.0;
            }
            break;
        case IDC_BUTTON3:
            reverse_status = !reverse_status;
            break;
        case IDC_BUTTON6: // Start the rectangle movement timer
            SetTimer(hDlg, TIMER_ID, 50, NULL); // Set a timer for moving the rectangle
            break;
        case IDC_BUTTON4:
            move_status = false;
            break;
        case IDC_BUTTON5:
            move_status = false;
            lines.clear();
            InvalidateRect(GetParent(hDlg), NULL, false);
            break;
        case IDC_BUTTON7:
            ShowWindow(hDlg, SW_HIDE);
            break;
        }
        break;
    case WM_TIMER:
        if (wParam == TIMER_ID) {
            // Move the rectangle
            dialogRect.left += dx;
            dialogRect.right += dx;

            // Check boundaries and reverse direction if needed
            RECT dlgRect;
            GetClientRect(hDlg, &dlgRect);
            if (dialogRect.right > dlgRect.right || dialogRect.left < dlgRect.left) {
                dx = -dx; // Reverse direction
            }

            InvalidateRect(hDlg, NULL, TRUE); // Request to repaint the dialog
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hDlg, &ps);

        if (!IsRectEmpty(&dialogRect)) {
            HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
            HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

            Rectangle(hDC, dialogRect.left, dialogRect.top, dialogRect.right, dialogRect.bottom);

            SelectObject(hDC, hOldPen);
            SelectObject(hDC, oldBrush);
            DeleteObject(hPen);
            DeleteObject(hBrush);
        }

        EndPaint(hDlg, &ps);
        return TRUE;
    }
    case WM_CLOSE:
        KillTimer(hDlg, TIMER_ID); // Stop the timer when the dialog is closed
        ShowWindow(hDlg, SW_HIDE);
        return TRUE;
    }
    return FALSE;
}

void MoveShapeAlongLines() {
    if (lines.empty() || !move_status) return;

    const double step = 0.01;
    progress += step;

    if (progress >= 1.0) {
        progress = 0.0;
        if (reverse_status) {
            current_line_index = (current_line_index - 1 + lines.size()) % lines.size();
        }
        else {
            current_line_index = (current_line_index + 1) % lines.size();
        }
    }

    const Line& line = lines[current_line_index];
    int shapeWidth = 50;
    int shapeHeight = 50;

    int centerX, centerY;

    if (reverse_status) {
        centerX = static_cast<int>((1 - progress) * line.end_x + progress * line.start_x);
        centerY = static_cast<int>((1 - progress) * line.end_y + progress * line.start_y);
    }
    else {
        centerX = static_cast<int>((1 - progress) * line.start_x + progress * line.end_x);
        centerY = static_cast<int>((1 - progress) * line.start_y + progress * line.end_y);
    }

    shape.rect.left = centerX - shapeWidth / 2;
    shape.rect.top = centerY - shapeHeight / 2;
    shape.rect.right = shape.rect.left + shapeWidth;
    shape.rect.bottom = shape.rect.top + shapeHeight;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HBITMAP hOldBitmap;
    RECT rect;

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
                line.color = select_color;
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
            }
            if (hDialog) {
                ShowWindow(hDialog, SW_SHOW);
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

        if (move_status) {
            shape.print(mDC, select_color, buffer);
        }

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);

        SelectObject(mDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        if (wParam == 1 && move_status) {
            MoveShapeAlongLines();
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
    case WM_DESTROY:
        if (hDialog) {
            DestroyWindow(hDialog);
            hDialog = NULL;
        }
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}
