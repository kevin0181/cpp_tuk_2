#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>
#include "resource.h"

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"�ǽ� 4-2";
LPCTSTR lpszWindowName = L"�ǽ� 4-2";

#define WIDTH 1200
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
    WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_QUESTION);
    RegisterClassEx(&WndClass);

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_VSCROLL, 0, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return Message.wParam;

}

#define WIDTH_LINE 40
#define HEGHIT_LINE 40

struct Rec {
    int x;
    int y;
    RECT rect;
    COLORREF color;
    bool status;
};

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static int Timer1Count = 0;
    static int cellSizeX;
    static int cellSizeY;
    static bool LDrag = false;
    static bool LStatus = false;
    static bool RDrag = false;
    static RECT moseRect;
    static vector<Rec> rec;
    static int selectedRect = -1;
    static bool grid = true;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        moseRect = { 0,0,0,0 };
        cellSizeX = 20;
        cellSizeY = 20;
        break;
    }
    case WM_COMMAND:
        switch (wParam)
        {
        case ID_GRID_40001: //�Ǽ�
            grid = true;
            break;
        case ID_GRID_40002: //����
            grid = false;
            break;
        case ID_COLOR_RED:
            break;
        case ID_COLOR_40004: //���
            break;
        case ID_COLOR_40005: //�Ķ�
            break;
        case ID_COLOR_40006: //�ϴ�
            break;
        case ID_COLOR_40007: //��Ʈ
            break;
        case ID_BORDER_ON:
            break;
        case ID_BORDER_OFF:
            break;
        case ID_MOVE_ON:
            break;
        case ID_MOVE_OFF:
            break;
        case ID_INFORM_ON:
            break;
        case ID_INFORM_OFF:
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // Ű���� Ű�� ������ ��
    {
        int dx = 0, dy = 0;
        switch (wParam) {
        case VK_LEFT:
            dx = -cellSizeX;
            break;
        case VK_RIGHT:
            dx = cellSizeX;
            break;
        case VK_UP:
            dy = -cellSizeY;
            break;
        case VK_DOWN:
            dy = cellSizeY;
            break;
        default:
            break;
        }

        for (auto& r : rec) {
            if (r.status) {
                // ���� ��ġ ���
                int newLeft = r.rect.left + dx;
                int newTop = r.rect.top + dy;
                int newRight = r.rect.right + dx;
                int newBottom = r.rect.bottom + dy;

                // ȭ�� ��踦 �Ѿ���� Ȯ���ϰ�, �Ѿ�ٸ� �ݴ����� ������ ����
                if (newRight < 0) {
                    newLeft = rect.right - (r.rect.right - r.rect.left);
                    newRight = rect.right;
                }
                else if (newLeft > rect.right) {
                    newRight = (r.rect.right - r.rect.left);
                    newLeft = 0;
                }

                if (newBottom < 0) {
                    newTop = rect.bottom - (r.rect.bottom - r.rect.top);
                    newBottom = rect.bottom;
                }
                else if (newTop > rect.bottom) {
                    newBottom = (r.rect.bottom - r.rect.top);
                    newTop = 0;
                }

                // ������ ��ġ�� �簢���� ������Ʈ
                SetRect(&r.rect, newLeft, newTop, newRight, newBottom);
                r.x = newLeft; // �⺻ ��ġ�� ������Ʈ
                r.y = newTop;
            }
        }
        InvalidateRect(hWnd, NULL, false);  // �����츦 �ٽ� �׸��ϴ�.
        break;
    }
    case WM_LBUTTONDOWN: { 
        LDrag = true;
        moseRect.left = LOWORD(lParam);
        moseRect.top = HIWORD(lParam);
        
        InvalidateRect(hWnd, NULL, false);  // �����츦 �ٽ� �׸��ϴ�.
        break;
    }
    case WM_LBUTTONUP:
    {
        LDrag = false;
        LStatus = false;

        if (rec.size() >= 5) {
            rec.erase(rec.begin());
        }

        Rec newRect;
        // Snap the coordinates to the nearest grid lines
        newRect.x = (min(moseRect.left, moseRect.right) / cellSizeX) * cellSizeX;
        newRect.y = (min(moseRect.top, moseRect.bottom) / cellSizeY) * cellSizeY;
        int right = (max(moseRect.right, moseRect.left) / cellSizeX) * cellSizeX;
        int bottom = (max(moseRect.bottom, moseRect.top) / cellSizeY) * cellSizeY;

        // Ensure the rectangle is at least one grid cell in size
        if (right == newRect.x) right += cellSizeX;
        if (bottom == newRect.y) bottom += cellSizeY;

        right = right + cellSizeX;
        bottom = bottom + cellSizeY;

        newRect.rect = { newRect.x, newRect.y, right, bottom };
        newRect.color = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
        newRect.status = false;
        rec.push_back(newRect);

        moseRect = { 0,0,0,0 };
        InvalidateRect(hWnd, NULL, false);
        break;
    }
    case WM_RBUTTONDOWN: {
        RDrag = true;
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        for (int i = 0; i < rec.size(); ++i) {
            if (PtInRect(&rec[i].rect, { x, y })) {
                selectedRect = i; // ���� ����
                break;
            }
        }
        InvalidateRect(hWnd, NULL, false);  // �����츦 �ٽ� �׸��ϴ�.
        break;
    }
    case WM_RBUTTONUP:
    {
        RDrag = false;
        if (selectedRect != -1) {
            // �׸��忡 ���߱�
            Rec& selRect = rec[selectedRect];
            int newX = (selRect.x / cellSizeX) * cellSizeX;
            int newY = (selRect.y / cellSizeY) * cellSizeY;
            OffsetRect(&selRect.rect, newX - selRect.x, newY - selRect.y);
            selRect.x = newX;
            selRect.y = newY;
        }
        selectedRect = -1;
        InvalidateRect(hWnd, NULL, false);
    }
        break;
    case WM_MOUSEMOVE:
    {
        POINT moseP;
        moseP.x = LOWORD(lParam);
        moseP.y = HIWORD(lParam);

        if (LDrag) {
            LStatus = true;
            moseRect.right = moseP.x;
            moseRect.bottom = moseP.y;
        }

        if (RDrag) {
            if (RDrag && selectedRect != -1) {
                POINT moseP = { LOWORD(lParam), HIWORD(lParam) };
                Rec& selRect = rec[selectedRect];
                int dx = moseP.x - selRect.x;
                int dy = moseP.y - selRect.y;
                OffsetRect(&selRect.rect, dx, dy);
                selRect.x = moseP.x;
                selRect.y = moseP.y;
                InvalidateRect(hWnd, NULL, false);
            }
        }
        InvalidateRect(hWnd, NULL, false);
    }
        break;
    case WM_CHAR:
        switch (wParam)
        {
        case '1':
            if (rec.size() >= 1) {
                for (int i = 0; i < rec.size(); ++i) {
                    rec[i].status = false;
                }
                rec[0].status = true;
            }
            break;
        case '2':
            if (rec.size() >= 2) {
                for (int i = 0; i < rec.size(); ++i) {
                    rec[i].status = false;
                }
                rec[1].status = true;
            }
            break;
        case '3':
            if (rec.size() >= 3) {
                for (int i = 0; i < rec.size(); ++i) {
                    rec[i].status = false;
                }
                rec[2].status = true;
            }
            break;
        case '4':
            if (rec.size() >= 4) {
                for (int i = 0; i < rec.size(); ++i) {
                    rec[i].status = false;
                }
                rec[3].status = true;
            }
            break;
        case '5':
            if (rec.size() >= 5) {
                for (int i = 0; i < rec.size(); ++i) {
                    rec[i].status = false;
                }
                rec[4].status = true;
            }
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_PAINT:
    {
        GetClientRect(hWnd, &rect);
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        SelectObject(mDC, (HBITMAP)hBitmap);
        FillRect(mDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

        cellSizeX = rect.right / WIDTH_LINE;
        cellSizeY = rect.bottom / HEGHIT_LINE;

        if (!grid) {
             mPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));  // ��Ʈ ��Ÿ�� �� ����
             oldPen = (HPEN)SelectObject(mDC, mPen);
        }
        else {
            mPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));  // ��Ʈ ��Ÿ�� �� ����
            oldPen = (HPEN)SelectObject(mDC, mPen);
        }
        for (int y = 0; y <= WIDTH_LINE; ++y) {
            MoveToEx(mDC, y * cellSizeX, 0, NULL);
            LineTo(mDC, y * cellSizeX, WIDTH_LINE * cellSizeY);
        }

        for (int x = 0; x <= HEGHIT_LINE; ++x) {
            MoveToEx(mDC, 0, x * cellSizeY, NULL);
            LineTo(mDC, HEGHIT_LINE * cellSizeX, x * cellSizeY);
        }

        SelectObject(mDC, oldPen); // ���� �� ����
        DeleteObject(mPen);      // ����� ��Ʈ �� ����

        // �� �簢���� �׸��� ���� �귯�ø� �����մϴ�.
        for (auto& r : rec) {
            if (r.status) {
                mPen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
                oldPen = (HPEN)SelectObject(mDC, mPen);
            }
            else {
                mPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                oldPen = (HPEN)SelectObject(mDC, mPen);
            }
            mBrush = CreateSolidBrush(r.color);
            oldBrush = (HBRUSH) SelectObject(mDC, mBrush);
            
            Rectangle(mDC, r.rect.left, r.rect.top, r.rect.right, r.rect.bottom);
            SelectObject(mDC, oldBrush);
            DeleteObject(mBrush);
            SelectObject(mDC, oldPen);
            DeleteObject(mPen);
        }

        // ��ġ�� �簢���� ������ ȥ��
        for (size_t i = 0; i < rec.size(); i++) {
            for (size_t j = i + 1; j < rec.size(); j++) {
                RECT intersection;
                if (IntersectRect(&intersection, &rec[i].rect, &rec[j].rect)) {
                    int newRed = (GetRValue(rec[i].color) + GetRValue(rec[j].color)) / 2;
                    int newGreen = (GetGValue(rec[i].color) + GetGValue(rec[j].color)) / 2;
                    int newBlue = (GetBValue(rec[i].color) + GetBValue(rec[j].color)) / 2;
                    mBrush = CreateSolidBrush(RGB(newRed, newGreen, newBlue));
                    SelectObject(mDC, mBrush);
                    Rectangle(mDC, intersection.left, intersection.top, intersection.right, intersection.bottom);
                    DeleteObject(mBrush);
                }
            }
        }

        if (LStatus) {
            mBrush = (HBRUSH)GetStockObject(BLACK_BRUSH); // ������ �귯�� ���
            oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

            FrameRect(mDC, &moseRect, mBrush); // �簢���� �׵θ��� �׸�

            SelectObject(mDC, oldBrush);
            DeleteObject(mBrush);
        }


        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);
        DeleteObject(hBitmap); // ������ ��Ʈ�� ����
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}