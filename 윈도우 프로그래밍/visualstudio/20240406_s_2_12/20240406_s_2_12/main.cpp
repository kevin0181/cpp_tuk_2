#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"�ǽ� 2-10��";
LPCTSTR lpszWindowName = L"�ǽ� 2-10��";

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

// ����: �ٵ��� ĭ�� ũ�⸦ 20x20 �ȼ��� ����
#define cellSize 20
//#define gridSize 40

class shape {
public:
    virtual void print_(HDC hDC) = 0;
    virtual int getX() = 0;
    virtual int getY() = 0;
};

class ChangeShape : public shape {
public:
    POINT sizePoint[6];
    int positionX{};
    int positionY{};
    COLORREF color = RGB(255, 165, 0); //��Ȳ
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
    char shape_c;
    bool mini_status = false; // false = ū, true = ����

    void print_(HDC hDC) override {
        hBrush = CreateSolidBrush(color);
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        oldPen = (HPEN)SelectObject(hDC, hPen);
        switch (shape_c)
        {
        case 'T': //����

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
        case 'R': //�׸�
            if (mini_status) {
                RoundRect(hDC, 0 + (positionX)*cellSize + 3, 0 + (positionY)*cellSize + 3, 15 + (positionX)*cellSize + 3, 15 + (positionY)*cellSize + 3, 5, 5);
            }
            else {
                RoundRect(hDC, 0 + (positionX)*cellSize, 0 + (positionY)*cellSize, 20 + (positionX)*cellSize, 20 + (positionY)*cellSize, 7, 7);
            }
            break;
        case 'C': //��
            if (mini_status) {
                Ellipse(hDC, 0 + (positionX)*cellSize + 3, 0 + (positionY)*cellSize + 3, 15 + (positionX)*cellSize + 3, 15 + (positionY)*cellSize + 3);
            }
            else {
                Ellipse(hDC, 0 + (positionX)*cellSize, 0 + (positionY)*cellSize, 20 + (positionX)*cellSize, 20 + (positionY)*cellSize);
            }
            break;
        case 'E': //Ÿ��
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
};

void printStartShape(vector<shape*>& shapes, char order_ch);
bool IsPositionUsed(const vector<shape*>& shapes, int x, int y);

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uid_RGB2(0, 254);
uniform_int_distribution<int> uid_BlockSize(30, 60);
uniform_int_distribution<int> uid_Block(0, 4);
uniform_int_distribution<int> uid_Position(1, 39);
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
    int saveMyShapeX;
    int saveMyShapeY;

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

        //saveMyShapeX = myShape.positionX;
        //saveMyShapeY = myShape.positionY;

        //switch (wParam) // myShape �̵�
        //{
        //case VK_LEFT:
        //    myShape.positionX -= 1;
        //    if (myShape.positionX == -1) {
        //        myShape.positionX = 39;
        //    }
        //    break;
        //case VK_RIGHT:
        //    myShape.positionX += 1;
        //    if (myShape.positionX == 40) {
        //        myShape.positionX = 0;
        //    }
        //    break;
        //case VK_UP:
        //    myShape.positionY -= 1;
        //    if (myShape.positionY == -1) {
        //        myShape.positionY = 39;
        //    }
        //    break;
        //case VK_DOWN:
        //    myShape.positionY += 1;
        //    if (myShape.positionY == 40) {
        //        myShape.positionY = 0;
        //    }
        //    break;
        //default:
        //    break;
        //}


        InvalidateRect(hWnd, NULL, TRUE);

        EndPaint(hWnd, &ps);
        break; 
    case WM_CHAR:

        switch (wParam)
        {
        case 's':
            gridSize = 30;
            break;
        case 'm':
            gridSize = 40;
            break;
        case 'l':
            gridSize = 50;
            break;
        default:
            break;
        }

        InvalidateRect(hWnd, NULL, TRUE);

        break;
    case WM_SIZE:
        break;
    case WM_PAINT:

        hDC = BeginPaint(hWnd, &ps);

        // �ٵ��� �� �׸���
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

void printStartShape(vector<shape*>& shapes, char order_ch) {

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
    switch (order_ch)
    {
    case 0: //����
        changeShape->shape_c = 'T';
        break;
    case 1: //�׸�
        changeShape->shape_c = 'R';
        break;
    case 2: //��
        changeShape->shape_c = 'C';
        break;
    case 3: //Ÿ��
        changeShape->shape_c = 'E';
        break;
    default:
        break;
    }
    shapes.push_back(changeShape);
}


bool IsPositionUsed(const vector<shape*>& shapes, int x, int y) { //�ߺ� ����

    for (const auto& shape : shapes) {
        if (shape->getX() == x && shape->getY() == y) {
            return true; // �̹� ��� ���� ��ġ �߰�
        }
    }

    if (x == 0 && y == 0)
        return true;

    if (x == 39 && y == 39)
        return true;

    return false; // ������ ���� ��ġ
}