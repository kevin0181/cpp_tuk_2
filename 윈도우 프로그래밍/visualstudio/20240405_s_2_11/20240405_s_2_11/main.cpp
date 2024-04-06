#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 2-11번";
LPCTSTR lpszWindowName = L"실습 2-11번";

#define WIDTH 1000
#define HEIGHT 800
#define COL 16

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

class shape {
public:
};

class RedBarricade : public shape {
public:
    RECT sizeRect = {
        0,0,20,20
    };
    int positionX{};
    int positionY{};
    COLORREF color = RGB(255, 0, 0);
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
};

class ChangeColor : public shape {
public:
    RECT sizeRect = {
        0,0,20,20
    };
    int positionX{};
    int positionY{};
    COLORREF color;
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
};

class ChangeShape : public shape {
public:
    POINT sizePoint[6];
    int positionX{};
    int positionY{};
    COLORREF color = RGB(255, 165, 0); //주황
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
    char shape_c;
    bool mini_status = false; // false = 큰, true = 작은
};

class SizeDown : public shape {
public:
    RECT sizeRect = {
        0,0,10,10
    };
    int positionX{};
    int positionY{};
    COLORREF color = RGB(200, 200, 0); //노랑
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
};

class SizeUp : public shape {
public:
    RECT sizeRect = {
        0,0,20,20
    };
    int positionX{};
    int positionY{};
    COLORREF color = RGB(135, 206, 235); //하늘색
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
};

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);

struct Shape {
    int shapeType = -1;
    int x1 = -1;
    int y1 = -1;
    int x2 = -1;
    int y2 = -1;
    int thickness = -1;
    COLORREF color;
    COLORREF borderColor;
};
void printShape(vector<Shape>& shapeList, int sh_cnt, HDC &hDC);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC;
    HPEN hPen, oldPen;
    HBRUSH hBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static vector<shape*> shapes;
    static TCHAR str[30];
    static int cnt_x;
    Shape shape;
    static int sh_cnt;
    static vector<Shape> shapeList;
    switch (uMsg)
    {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        cnt_x = 0;
        sh_cnt = 0;
        break;
    case WM_KEYUP:

        break;
    case WM_KEYDOWN:

        if (shapeList.size() == 0) {
            break;
        }
        else {
            auto& lastShape = shapeList.back();

            switch (wParam)
            {
            case VK_LEFT:
                lastShape.x1 -= 10; // 왼쪽으로 10 단위 이동
                lastShape.x2 -= 10;
                break;
            case VK_RIGHT:
                lastShape.x1 += 10; // 오른쪽으로 10 단위 이동
                lastShape.x2 += 10;
                break;
            case VK_UP:
                lastShape.y1 -= 10; // 위로 10 단위 이동
                lastShape.y2 -= 10;
                break;
            case VK_DOWN:
                lastShape.y1 += 10; // 아래로 10 단위 이동
                lastShape.y2 += 10;
                break;
            default:
                break;
            }
        }
        InvalidateRect(hWnd, NULL, TRUE);

        break;
    case WM_CHAR:

        if (wParam == VK_BACK && cnt_x > 0) {
            cnt_x--;
            str[cnt_x] = '\0';
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }

        if (wParam == VK_RETURN) {
            wistringstream iss(str);
            iss >> shape.shapeType >> shape.x1 >> shape.y1 >> shape.x2 >> shape.y2 >> shape.thickness;

            if (shape.shapeType == -1 || shape.x1 == -1 || shape.y1 == -1 || shape.x2 == -1 || shape.y2 == -1 || shape.thickness == -1) {
                for (int i = 29; i >= 0; --i) {
                    str[i] = '\0';
                }

                cnt_x = 0;

                InvalidateRect(hWnd, NULL, TRUE);
                break;
            }

            shape.color = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
            shape.borderColor = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
            shapeList.push_back(shape);
            sh_cnt++;

            for (int i = 29; i >= 0; --i) {
                str[i] = '\0';
            }
            cnt_x = 0;

            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }

        if (wParam >= '0' && wParam <= '9' || wParam == ' ') {
            str[cnt_x++] = wParam;
            str[cnt_x] = '\0';
        }
        else {

           auto& lastShape = shapeList.back();

            switch (wParam)
            {
            case 'p':
                
                if (lastShape.thickness > 10) {
                    lastShape.x1--;
                    lastShape.y1--;
                    lastShape.x2++;
                    lastShape.y2++;
                }
                else {
                    lastShape.thickness++; // 굵기 증가
                }
                break;
            case 'o':
                if (lastShape.thickness <= 1) {
                    lastShape.x1++;
                    lastShape.y1++;
                    lastShape.x2--;
                    lastShape.y2--;
                }
                else {
                    lastShape.thickness = max(1, lastShape.thickness - 1);
                }
                break;
           
            default:
                break;
            }
        }


        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_SIZE:
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &rect);
        
        MoveToEx(hDC, 0, rect.bottom - (COL + 10), NULL);
        LineTo(hDC, rect.right, rect.bottom - (COL + 10));
        GetTextExtentPoint32(hDC, str, lstrlen(str), &size);
        TextOut(hDC, 0, rect.bottom - (COL + 5), str, lstrlen(str));// 입력 받은거 출력
        SetCaretPos((size.cx), rect.bottom - (COL + 5)); //caret 위치 표시
        ShowCaret(hWnd);

        printShape(shapeList, sh_cnt, hDC);

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        HideCaret(hWnd);
        DestroyCaret();
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

#define M_PI 3.14159265358979323846

void printShape(vector<Shape>& shapeList, int sh_cnt, HDC &hDC) {

    if (sh_cnt == 0)
        return;

    HBRUSH hBrush = CreateSolidBrush(shapeList[sh_cnt - 1].color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    RECT rect;
    HPEN hPen = CreatePen(PS_SOLID, shapeList[sh_cnt - 1].thickness, shapeList[sh_cnt - 1].borderColor);
    HPEN oldPen = (HPEN)SelectObject(hDC, hPen);

    int centerX;
    int centerY;
    int radius;
    float angle;
    POINT points1[5];

    switch (shapeList[sh_cnt - 1].shapeType)
    {
    case 1:
        hPen = CreatePen(PS_SOLID, shapeList[sh_cnt - 1].thickness, shapeList[sh_cnt - 1].borderColor);
        oldPen = (HPEN)SelectObject(hDC, hPen);
        hBrush = CreateSolidBrush(shapeList[sh_cnt - 1].color);
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        rect = { shapeList[sh_cnt - 1].x1 - shapeList[sh_cnt - 1].thickness / 2, shapeList[sh_cnt - 1].y1 - shapeList[sh_cnt - 1].thickness / 2, shapeList[sh_cnt - 1].x2 + shapeList[sh_cnt - 1].thickness / 2, shapeList[sh_cnt - 1].y2 + shapeList[sh_cnt - 1].thickness / 2 };
        Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
        break;
    case 2:
        hPen = CreatePen(PS_SOLID, shapeList[sh_cnt - 1].thickness, shapeList[sh_cnt - 1].borderColor);
        oldPen = (HPEN)SelectObject(hDC, hPen);
        MoveToEx(hDC, shapeList[sh_cnt - 1].x1, shapeList[sh_cnt - 1].y1, NULL);
        LineTo(hDC, shapeList[sh_cnt - 1].x2, shapeList[sh_cnt - 1].y2);
        break;
    case 3:

        hPen = CreatePen(PS_SOLID, shapeList[sh_cnt - 1].thickness, shapeList[sh_cnt - 1].borderColor);

        POINT points[3];
        points[0] = { shapeList[sh_cnt - 1].x1, shapeList[sh_cnt - 1].y1 };
        points[1] = { shapeList[sh_cnt - 1].x2, shapeList[sh_cnt - 1].y1 };
        points[2] = { shapeList[sh_cnt - 1].x1, shapeList[sh_cnt - 1].y2 };

        SelectObject(hDC, hPen);
        SelectObject(hDC, hBrush);

        Polygon(hDC, points, 3);
        break;
    case 4:
        hPen = CreatePen(PS_SOLID, shapeList[sh_cnt - 1].thickness, shapeList[sh_cnt - 1].borderColor);
        oldPen = (HPEN)SelectObject(hDC, hPen);
        hBrush = CreateSolidBrush(shapeList[sh_cnt - 1].color);
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        rect = { shapeList[sh_cnt - 1].x1 - shapeList[sh_cnt - 1].thickness / 2, shapeList[sh_cnt - 1].y1 - shapeList[sh_cnt - 1].thickness / 2, shapeList[sh_cnt - 1].x2 + shapeList[sh_cnt - 1].thickness / 2, shapeList[sh_cnt - 1].y2 + shapeList[sh_cnt - 1].thickness / 2 };
        Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
        break;
    case 5:
        centerX = (shapeList[sh_cnt - 1].x1 + shapeList[sh_cnt - 1].x2) / 2;
        centerY = (shapeList[sh_cnt - 1].y1 + shapeList[sh_cnt - 1].y2) / 2;

        radius = min(shapeList[sh_cnt - 1].x2 - centerX, shapeList[sh_cnt - 1].y2 - centerY);
        
        for (int i = 0; i < 5; ++i) {
            angle = 2 * M_PI / 5 * i;
            points1[i].x = static_cast<LONG>(centerX + radius * sin(angle));
            points1[i].y = static_cast<LONG>(centerY - radius * cos(angle)); 
        }

        hPen = CreatePen(PS_SOLID, shapeList[sh_cnt - 1].thickness, shapeList[sh_cnt - 1].borderColor);
        hBrush = CreateSolidBrush(shapeList[sh_cnt - 1].color);

        oldPen = (HPEN)SelectObject(hDC, hPen);
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

        Polygon(hDC, points1, 5);

        break;
        break;
    case 6:
        hPen = CreatePen(PS_SOLID, shapeList[sh_cnt - 1].thickness, shapeList[sh_cnt - 1].borderColor);
        oldPen = (HPEN)SelectObject(hDC, hPen);
        hBrush = CreateSolidBrush(shapeList[sh_cnt - 1].color);
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

        rect = {
            shapeList[sh_cnt - 1].x1,
            shapeList[sh_cnt - 1].y1,
            shapeList[sh_cnt - 1].x1 + shapeList[sh_cnt - 1].x2,
            shapeList[sh_cnt - 1].y1 + shapeList[sh_cnt - 1].y2 
        };

        Ellipse(hDC, rect.left, rect.top, rect.right, rect.bottom);
        break;
    default:
        break;
    }


    // Restore the old pen and brush.
    SelectObject(hDC, oldPen);
    SelectObject(hDC, oldBrush);
    DeleteObject(hBrush);
    DeleteObject(hPen);

}