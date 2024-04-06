#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 2-12번";
LPCTSTR lpszWindowName = L"실습 2-12번";

#define WIDTH 1200
#define HEIGHT 860

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

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_VSCROLL, 0, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
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
//#define gridSize 40

class shape {
public:
    virtual void print_(HDC hDC) = 0;
    virtual int getX() = 0;
    virtual int getY() = 0;
    virtual void sizeUp() = 0;
    virtual void sizeDown() = 0;
    virtual COLORREF getColor() = 0;
    virtual void setOriginalShape(char c) = 0;
    virtual void resetShape() = 0;
};

class ChangeShape : public shape {
public:
    POINT sizePoint[6];
    int positionX{};
    int positionY{};
    COLORREF color = RGB(255, 165, 0); //주황
    COLORREF bolderColor = RGB(255, 255, 255);
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
    char shape_c;
    char original_c;
    bool mini_status = false; // false = 큰, true = 작은

    void print_(HDC hDC) override {
        hBrush = CreateSolidBrush(color);
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        hPen = CreatePen(PS_SOLID, 2, bolderColor);
        oldPen = (HPEN)SelectObject(hDC, hPen);
        switch (shape_c)
        {
        case 'T': //세모

            if (mini_status) {
                sizePoint[0] = { 0, 10 };
                sizePoint[1] = { 5, static_cast<LONG>(round(1.34)) };
                sizePoint[2] = { 10, 10 };
                for (int i = 0; i < 3; ++i) {
                    sizePoint[i].x += positionX * cellSize + 5;
                    sizePoint[i].y += positionY * cellSize + 5;
                }
            }
            else {
                sizePoint[0] = { 0, 20 };
                sizePoint[1] = { 10, static_cast<LONG>(round(2.68)) };
                sizePoint[2] = { 20, 20 };
                for (int i = 0; i < 3; ++i) {
                    sizePoint[i].x += positionX * cellSize;
                    sizePoint[i].y += positionY * cellSize - 2;
                }
            }

            Polygon(hDC, sizePoint, 3);
            break;
        case 'R': //네모
            if (mini_status) {
                RoundRect(hDC, 0 + (positionX)*cellSize + 3, 0 + (positionY)*cellSize + 3, 15 + (positionX)*cellSize + 3, 15 + (positionY)*cellSize + 3, 5, 5);
            }
            else {
                RoundRect(hDC, 0 + (positionX)*cellSize, 0 + (positionY)*cellSize, 20 + (positionX)*cellSize, 20 + (positionY)*cellSize, 7, 7);
            }
            break;
        case 'C': //원
            if (mini_status) {
                Ellipse(hDC, 0 + (positionX)*cellSize + 3, 0 + (positionY)*cellSize + 3, 15 + (positionX)*cellSize + 3, 15 + (positionY)*cellSize + 3);
            }
            else {
                Ellipse(hDC, 0 + (positionX)*cellSize, 0 + (positionY)*cellSize, 20 + (positionX)*cellSize, 20 + (positionY)*cellSize);
            }
            break;
        case 'E': //타원
            if (mini_status) {
                Ellipse(hDC, 0 + (positionX)*cellSize, 7 + (positionY)*cellSize, 20 + (positionX)*cellSize, 17 + (positionY)*cellSize);
            }
            else {
                Ellipse(hDC, 0 + (positionX)*cellSize, 3 + (positionY)*cellSize, 20 + (positionX)*cellSize, 17 + (positionY)*cellSize);
            }
            break;
        default:
            break;
        }

        SelectObject(hDC, oldBrush);
        DeleteObject(hBrush);
        DeleteObject(hPen);
    }

    int getX() {
        return positionX;
    }

    int getY() {
        return positionY;
    }

    void sizeUp() {
        mini_status = false;
    }

    void sizeDown() {
        mini_status = true;
    }

    COLORREF getColor() {
        return color;
    }

    void setOriginalShape(char c) {
        shape_c = original_c;
        shape_c = c;
    }
    void resetShape() {
        shape_c = original_c;
    }
};

void printStartShape(vector<shape*>& shapes, char order_ch);
bool IsPositionUsed(const vector<shape*>& shapes, int x, int y);

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uid_RGB2(0, 254);
uniform_int_distribution<int> uid_BlockSize(30, 60);
uniform_int_distribution<int> uid_Block(0, 4);

uniform_int_distribution<int> uid_BlockShape(0, 3);
uniform_int_distribution<int> uid_SizeStatus(0, 1);

int gridSize{};
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC;
    HPEN hPen, oldPen;
    HBRUSH hBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static vector<shape*> shapes;
    static int selectShape;
    static bool c_status = false;
    switch (uMsg)
    {
    case WM_CREATE:

        break;
    case WM_KEYUP:
        if (wParam == 'P' || wParam == 'p') {

            for (auto& shape : shapes) {
                delete shape;
            }

            shapes.clear();

            InvalidateRect(hWnd, NULL, TRUE);
        }

        break;
    case WM_KEYDOWN:

        hDC = BeginPaint(hWnd, &ps);
        
        if (shapes.size() >= 1) {
            ChangeShape* changeShape = static_cast<ChangeShape*>(shapes[selectShape]);
            switch (wParam) // myShape 이동
            {
            case VK_LEFT:
                changeShape->positionX -= 1;
                if (changeShape->positionX == -1) {
                    changeShape->positionX = 39;
                }
                break;
            case VK_RIGHT:
                changeShape->positionX += 1;
                if (changeShape->positionX == 40) {
                    changeShape->positionX = 0;
                }
                break;
            case VK_UP:
                changeShape->positionY -= 1;
                if (changeShape->positionY == -1) {
                    changeShape->positionY = 39;
                }
                break;
            case VK_DOWN:
                changeShape->positionY += 1;
                if (changeShape->positionY == 40) {
                    changeShape->positionY = 0;
                }
                break;
            default:
                break;
            }
        }
        

        InvalidateRect(hWnd, NULL, TRUE);

        EndPaint(hWnd, &ps);
        break; 
    case WM_CHAR:

        switch (wParam)
        {
        case 'q':
            PostQuitMessage(0);
            break;
        case 's':
            gridSize = 30;
            break;
        case 'm':
            gridSize = 40;
            break;
        case 'l':
            gridSize = 50;
            break;
        case 'e':
            printStartShape(shapes, 'C');
            break;
        case 't':
            printStartShape(shapes, 'T');
            break;
        case 'r':
            printStartShape(shapes, 'R');
            break;
        case 'd':
            if (shapes.size() != 0) {
                for (int i = selectShape; i < shapes.size() - 1; ++i)
                {
                    shapes[i] = shapes[i + 1];
                }
                shapes.pop_back();
                selectShape = 0;
            }
            break;
        case'c':
        {
            ChangeShape* changeShape = static_cast<ChangeShape*>(shapes[selectShape]);
            if (c_status) {
                for (auto& shape : shapes) {
                    if (changeShape->color == shape->getColor()) {
                        changeShape->shape_c = changeShape->original_c;
                        shape->resetShape();
                    }
                }
                c_status = false;
            }
            else {
               
                for (auto& shape : shapes) {
                    if (changeShape->color == shape->getColor()) {
                        changeShape->original_c = changeShape->shape_c;
                        changeShape->shape_c = 'E';

                        shape->setOriginalShape('E');
                    }
                }
                c_status = true;
            }
           
        }
            break;
        case ']':
        {
            auto& changeShape = shapes[selectShape];
            changeShape->sizeUp();
            break;
        }
        case '[':
        {
            auto& changeShape = shapes[selectShape];
            changeShape->sizeDown();
            break;
        }
        default:
            break;
        }

        if (wParam >= '0' && wParam <= '9') {
            if (wParam - '0' < shapes.size()) {
                //변경 전
                ChangeShape* changeShapeB = static_cast<ChangeShape*>(shapes[selectShape]);
                changeShapeB->bolderColor = RGB(255, 255, 255);


                //변경
                selectShape = wParam - '0';
                ChangeShape* changeShapeA = static_cast<ChangeShape*>(shapes[selectShape]);
                changeShapeA->bolderColor = RGB(100, 100, 100);
            }
        }

        InvalidateRect(hWnd, NULL, TRUE);

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

        if (shapes.size() != 0) {
            auto& shapeP = shapes[selectShape];
            shapeP->print_(hDC);
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

void printStartShape(vector<shape*>& shapes, char order_ch) {

    if (shapes.size() >= 10) {
        for (int i = 1; i < shapes.size(); ++i)
        {
            shapes[i - 1] = shapes[i];
        }
        shapes.pop_back();
    }

    uniform_int_distribution<int> uid_Position(0, gridSize-1);
    int pX;
    int pY;
    int maxBlockSize;
    int num{};

    ChangeShape* changeShape;

    maxBlockSize = uid_BlockSize(gen);

    do {
        pX = uid_Position(gen);
        pY = uid_Position(gen);
    } while (IsPositionUsed(shapes, pX, pY));

    changeShape = new ChangeShape();
    changeShape->positionX = pX;
    changeShape->positionY = pY;
    changeShape->color = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
    switch (order_ch)
    {
    case 'T': //세모
        changeShape->shape_c = 'T';
        break;
    case 'R': //네모
        changeShape->shape_c = 'R';
        break;
    case 'C': //원
        changeShape->shape_c = 'C';
        break;
        break;
    default:
        break;
    }
    shapes.push_back(changeShape);
}


bool IsPositionUsed(const vector<shape*>& shapes, int x, int y) { //중복 관리

    for (const auto& shape : shapes) {
        if (shape->getX() == x && shape->getY() == y) {
            return true; // 이미 사용 중인 위치 발견
        }
    }

    if (x == 0 && y == 0)
        return true;

    if (x == 39 && y == 39)
        return true;

    return false; // 사용되지 않은 위치
}