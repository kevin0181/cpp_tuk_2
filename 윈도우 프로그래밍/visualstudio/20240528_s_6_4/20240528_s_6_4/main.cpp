#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <atlimage.h>
#include "resource.h"

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"INVERSUS";
LPCTSTR lpszWindowName = L"INVERSUS";

#define WIDTH 1200
#define HEIGHT 1000

bool draw_s = false; // 사각형 그리기 플래그

struct Shape {
    RECT rect = { 0,0,20,20 };
    int sh = 0;
    int x = 0;
    int y = 0;
    int step = 0;
};

struct Line {
    int start_x, start_y;
    int end_x, end_y;
    COLORREF color = RGB(0, 0, 0);
};

static vector<Line> lines;
bool draw_status = false;
bool Drag = false;
int shape_size = 10;
int current_line_index = 0;
int speed = 1;
bool speed_status = false;

Shape shape;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK DialogProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg) {
    case WM_CREATE:
        SetTimer(hDlg, 1, 10, NULL); // 10ms 간격으로 타이머 설정
        break;
    case WM_TIMER:
        if (wParam == 1&& speed_status) {
            if (draw_s && lines.size() > 0) {
                Line& line = lines[current_line_index];
                int dx = line.end_x - line.start_x;
                int dy = line.end_y - line.start_y;
                int length = static_cast<int>(sqrt(dx * dx + dy * dy));
                int steps = length / speed;
                if (shape.step >= steps) {
                    shape.step = 0;
                    current_line_index = (current_line_index + 1) % lines.size();
                }
                else {
                    shape.step++;
                }

                float ratio = static_cast<float>(shape.step) / steps;
                shape.x = line.start_x + static_cast<int>(dx * ratio);
                shape.y = line.start_y + static_cast<int>(dy * ratio);

                shape.rect.left = shape.x - shape_size;
                shape.rect.top = shape.y - shape_size;
                shape.rect.right = shape.x + shape_size;
                shape.rect.bottom = shape.y + shape_size;

                InvalidateRect(hDlg, NULL, FALSE);
            }
        }
        break;
    case WM_INITDIALOG:
        SetTimer(hDlg, 1, 10, NULL); // 10ms 간격으로 타이머 설정
        return TRUE; // WM_INITDIALOG 메시지를 처리하고 TRUE 반환
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            // MessageBox(hDlg, L"test", L"test", MB_OK);
            return TRUE; // IDOK 처리 후 TRUE 반환
        case IDCANCEL:
            PostQuitMessage(0);
            EndDialog(hDlg, 0);
            return TRUE; // IDCANCEL 처리 후 TRUE 반환
        case IDC_RADIO1:
            if (lines.size() != 0) {
                shape.sh = 0;
                draw_s = true; // 사각형 그리기 플래그 설정
                shape.step = 0;
                current_line_index = 0;
                shape.x = lines[0].start_x;
                shape.y = lines[0].start_y;
                shape.rect = { shape.x - shape_size, shape.y - shape_size, shape.x + shape_size, shape.y + shape_size };
                InvalidateRect(hDlg, NULL, FALSE); // 다이얼로그를 무효화하여 다시 그리도록 요청
            }
            return TRUE;
        case IDC_RADIO2:
            if (lines.size() != 0) {
                shape.sh = 1;
                draw_s = true; // 사각형 그리기 플래그 설정
                shape.step = 0;
                current_line_index = 0;
                shape.x = lines[0].start_x;
                shape.y = lines[0].start_y;
                shape.rect = { shape.x - shape_size, shape.y - shape_size, shape.x + shape_size, shape.y + shape_size };
                InvalidateRect(hDlg, NULL, FALSE); // 다이얼로그를 무효화하여 다시 그리도록 요청
            }
            return TRUE;
        case IDC_RADIO3:
            shape_size = 5;
            InvalidateRect(hDlg, NULL, FALSE);
            return TRUE;
        case IDC_RADIO4:
            shape_size = 10;
            InvalidateRect(hDlg, NULL, FALSE);
            return TRUE;
        case IDC_RADIO5:
            shape_size = 20;
            InvalidateRect(hDlg, NULL, FALSE);
            return TRUE;
        case IDC_RADIO6:
            speed = 5;
            return TRUE;
        case IDC_RADIO7:
            speed = 2;
            return TRUE;
        case IDC_BUTTON1:
            draw_status = true;
            return TRUE;
        case IDC_BUTTON2:
            speed_status = !speed_status;
            return TRUE;
        case IDC_BUTTON3:
            EndDialog(hDlg, 0);
            PostQuitMessage(0);
            return TRUE;
        }
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
                lines.push_back(line);
            }

            InvalidateRect(hDlg, NULL, FALSE);
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
            InvalidateRect(hDlg, NULL, FALSE);
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);
        HDC memDC = CreateCompatibleDC(hdc);
        RECT rect;
        GetClientRect(hDlg, &rect);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

        // 배경을 흰색으로 채움
        FillRect(memDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));

        for (const auto& line : lines) {
            HPEN hPen = CreatePen(PS_SOLID, 2, line.color);
            HPEN hOldPen = (HPEN)SelectObject(memDC, hPen);
            MoveToEx(memDC, line.start_x, line.start_y, NULL);
            LineTo(memDC, line.end_x, line.end_y);
            SelectObject(memDC, hOldPen);
            DeleteObject(hPen);
        }

        if (draw_s) {
            if (shape.sh == 0) {
                Rectangle(memDC, shape.rect.left, shape.rect.top, shape.rect.right, shape.rect.bottom); // 사각형 그리기
            }
            else if (shape.sh == 1) {
                Ellipse(memDC, shape.rect.left, shape.rect.top, shape.rect.right, shape.rect.bottom); // 타원 그리기
            }
        }

        // 백 버퍼에서 화면으로 복사
        BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, memDC, 0, 0, SRCCOPY);

        // 메모리 DC 정리
        SelectObject(memDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(memDC);

        EndPaint(hDlg, &ps);
    }
    return TRUE; // WM_PAINT 메시지를 처리하고 TRUE 반환
    case WM_CLOSE:
        EndDialog(hDlg, 0);
        PostQuitMessage(0);
        return TRUE; // WM_CLOSE 처리 후 TRUE 반환
    }
    return FALSE; // 나머지 메시지는 FALSE 반환
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
    HWND hWnd;
    MSG Message;
    WNDCLASSEX WndClass;
    g_hInst = hInstance;

    DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DialogProc);

    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return static_cast<int>(Message.wParam);
}
