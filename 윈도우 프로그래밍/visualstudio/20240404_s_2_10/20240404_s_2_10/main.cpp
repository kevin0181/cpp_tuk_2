#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

#define M_PI 3.14159265358979323846

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 2-10번";
LPCTSTR lpszWindowName = L"실습 2-10번";

#define WIDTH 830
#define HEIGHT 850

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

// 설정: 바둑판 칸의 크기를 20x20 픽셀로 정의
#define cellSize 20
#define gridSize 40

class shape {
public:
    virtual void print_(HDC hDC) = 0;
};

class RedBarricade : public shape{
public:
    RECT sizeRect = {
        0,0,20,20
    };
    int positionX{};
    int positionY{};
    COLORREF color = RGB(255, 0, 0);
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;

    void print_(HDC hDC) override {
        hBrush = CreateSolidBrush(color);
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        hPen = CreatePen(PS_SOLID, 1, color);
        oldPen = (HPEN)SelectObject(hDC, hPen);
        Rectangle(hDC, sizeRect.left + (positionX - 1) * cellSize, sizeRect.top + (positionY - 1) * cellSize, sizeRect.right + (positionX - 1) * cellSize, sizeRect.bottom + (positionY - 1) * cellSize);
        SelectObject(hDC, oldBrush);
        DeleteObject(hBrush);
        DeleteObject(hPen);
    }

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

    void print_(HDC hDC) override {
        hBrush = CreateSolidBrush(color);
        oldBrush = (HBRUSH) SelectObject(hDC, hBrush);
        hPen = CreatePen(PS_SOLID, 1, color);
        oldPen = (HPEN)SelectObject(hDC, hPen);
        Rectangle(hDC, sizeRect.left + (positionX - 1) * cellSize, sizeRect.top + (positionY - 1) * cellSize, sizeRect.right + (positionX - 1) * cellSize, sizeRect.bottom + (positionY - 1) * cellSize);
        SelectObject(hDC, oldBrush);
        DeleteObject(hBrush);
        DeleteObject(hPen);
    }

};

class ChangeShape : public shape{
public:
    POINT *sizePoint;
    int positionX{};
    int positionY{};
    COLORREF color = RGB(255, 165, 0); //주황
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
    char shape_c;

    void print_(HDC hDC) override {
        hBrush = CreateSolidBrush(color);
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

        switch (shape_c)
        {
        case 'T': //세모

            break;
        case 'R': //네모
            break;
        case 'C': //원
            break;
        case 'E': //타원
            break;
        default:
            break;
        }

        SelectObject(hDC, oldBrush);
        DeleteObject(hBrush);
        DeleteObject(hPen);
    }

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

    void print_(HDC hDC) override {
        hBrush = CreateSolidBrush(color);
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        hPen = CreatePen(PS_SOLID, 1, color);
        oldPen = (HPEN)SelectObject(hDC, hPen);
        Rectangle(hDC, sizeRect.left + (positionX - 1) * cellSize + 6, sizeRect.top + (positionY - 1) * cellSize + 6, sizeRect.right + (positionX - 1) * cellSize + 6, sizeRect.bottom + (positionY - 1) * cellSize + 6);
        SelectObject(hDC, oldBrush);
        DeleteObject(hBrush);
        DeleteObject(hPen);
    }

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

    void print_(HDC hDC) override {
        hBrush = CreateSolidBrush(color);
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        hPen = CreatePen(PS_SOLID, 1, color);
        oldPen = (HPEN)SelectObject(hDC, hPen);
        Rectangle(hDC, sizeRect.left + (positionX - 1) * cellSize, sizeRect.top + (positionY - 1) * cellSize, sizeRect.right + (positionX - 1)* cellSize, sizeRect.bottom + (positionY - 1)* cellSize);
        SelectObject(hDC, oldBrush);
        DeleteObject(hBrush);
    }

};

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uid_BlockSize(20, 30);
uniform_int_distribution<int> uid_Block(0, 4);
uniform_int_distribution<int> uid_Position(1, 40);
uniform_int_distribution<int> uid_BlockShape(0, 3);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC;
    HPEN hPen, oldPen;
    HBRUSH hBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static vector<shape*> shapes;
    RedBarricade* redBarricade;
    ChangeColor* changeColor;
    SizeUp* sizeUp;
    SizeDown* sizeDown;
    ChangeShape* changeShape;

    switch (uMsg)
    {
    case WM_CREATE:

        for (int i = 0; i < uid_BlockSize(gen); ++i) {
            switch (uid_Block(gen))
            {
            case 0: //장애물
                redBarricade = new RedBarricade();
                redBarricade->positionX = uid_Position(gen);
                redBarricade->positionY = uid_Position(gen);
                shapes.push_back(redBarricade);
                break;
            case 1: //색상변경
                changeColor = new ChangeColor();
                changeColor->positionX = uid_Position(gen);
                changeColor->positionY = uid_Position(gen);
                changeColor->color = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
                shapes.push_back(changeColor);
                break;
            case 2: //크기변경(확대)
                sizeUp = new SizeUp();
                sizeUp->positionX = uid_Position(gen);
                sizeUp->positionY = uid_Position(gen);
                shapes.push_back(sizeUp);
                break;
            case 3: //크기변경(축소)
                sizeDown = new SizeDown();
                sizeDown->positionX = uid_Position(gen);
                sizeDown->positionY = uid_Position(gen);
                shapes.push_back(sizeDown);
                break;
            case 4: //모양변경
                changeShape = new ChangeShape();
                changeShape->positionX = uid_Position(gen);
                changeShape->positionY = uid_Position(gen);

                switch (uid_BlockShape(gen))
                {
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                default:
                    break;
                }


                shapes.push_back(changeShape);
                break;
            default:
                break;
            }
        }

        break;
    case WM_KEYUP:
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_KEYDOWN:
        break;
    case WM_CHAR:
        break;
    case WM_SIZE:
        break;
    case WM_PAINT:

        hDC = BeginPaint(hWnd, &ps);

        // 바둑판 선 그리기
        for (int y = 0; y <= gridSize; ++y) {
            MoveToEx(hDC, y * cellSize, 0, NULL);
            LineTo(hDC, y * cellSize, gridSize * cellSize);
        }

        for (int x = 0; x <= gridSize; ++x) {
            MoveToEx(hDC, 0, x * cellSize, NULL);
            LineTo(hDC, gridSize * cellSize, x * cellSize);
        }

        for (auto& shape : shapes) {
            shape->print_(hDC);
        }

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}