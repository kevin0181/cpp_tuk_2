#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

#define M_PI 3.14159265358979323846

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 2-9번";
LPCTSTR lpszWindowName = L"실습 2-9번";

#define WIDTH 1000
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

#define DEFAULT_COL 16


class Shape {
public:
    virtual ~Shape() {} // 가상 소멸자
};

class Triangle : public Shape {
public:
    POINT p[3];
    COLORREF color;
    Triangle() {}
    Triangle(POINT getP[3], COLORREF color) : color(color) {
        for (int i = 0; i < 3; ++i) {
            p[i] = getP[i];
        }
    }

    POINT* getPoints() {
        return p;
    }

    // 설정자 함수: 꼭짓점 배열 설정
    void setPoints(POINT newP[3]) {
        for (int i = 0; i < 3; ++i) {
            p[i] = newP[i];
        }
    }
};

class Hourglass : public Shape {
public:
    POINT center;
    int width, height;
    // 상단 삼각형의 꼭짓점 계산
    POINT topTriangle[3] = {
        {center.x - width / 2, center.y - height / 2}, // 상단 삼각형 왼쪽 꼭지점
        {center.x, center.y},                         // 상단 삼각형 끝점(모래시계 중심)
        {center.x + width / 2, center.y - height / 2} // 상단 삼각형 오른쪽 꼭지점
    };
    // 하단 삼각형의 꼭짓점 계산
    POINT bottomTriangle[3] = {
        {center.x - width / 2, center.y + height / 2}, // 하단 삼각형 왼쪽 꼭지점
        {center.x, center.y},                          // 하단 삼각형 끝점(모래시계 중심)
        {center.x + width / 2, center.y + height / 2}  // 하단 삼각형 오른쪽 꼭지점
    };
    COLORREF color;

    Hourglass() {}
    Hourglass(POINT center, int width, int height, COLORREF color) : color(color), center(center), width(width), height(height) {
        topTriangle[0] = { center.x - width / 2, center.y - height / 2 };
        topTriangle[1] = { center.x, center.y };
        topTriangle[2] = { center.x + width / 2, center.y - height / 2 };

        bottomTriangle[0] = { center.x - width / 2, center.y + height / 2 };
        bottomTriangle[1] = { center.x, center.y };
        bottomTriangle[2] = { center.x + width / 2, center.y + height / 2 };
    }

    POINT* getTopTriangle() {
        return topTriangle;
    }

    POINT* getBottomTriangle() {
        return bottomTriangle;
    }

    POINT getCenter() {
        return center;
    }

};

class Pentagon : public Shape {
public:
    POINT p[5];
    COLORREF color;
    Pentagon() {}
    Pentagon(POINT getP[5], COLORREF color) : color(color) {
        for (int i = 0; i < 5; ++i) {
            p[i] = getP[i];
        }
    }

    POINT* getPoints() {
        return p;
    }

   /* void draw(HDC hdc) override {
        HBRUSH hBrush = CreateSolidBrush(color);
        HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);
        Polygon(hdc, p, 5);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hBrush);
    }*/
};

class Pacman : public Shape {
public:
    POINT p[4];
    COLORREF color;

    /*POINT topLeft = { xLeft, yTop };
    POINT bottomRight = { xRight, yBottom };
    POINT start = { xStart, yStart };
    POINT end = { xEnd, yEnd };

    Pie(hDC, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, start.x, start.y, end.x, end.y);*/

    Pacman() {}
    Pacman(POINT getP[4], COLORREF color) : color(color) {
        for (int i = 0; i < 4; ++i) {
            p[i] = getP[i];
        }
    }
    POINT* getPoints() {
        return p;
    }

    /*void draw(HDC hdc) override {
        HBRUSH hBrush = CreateSolidBrush(color);
        HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);
        Pie(hdc, p[0].x, p[0].y, p[1].x, p[1].y, p[2].x, p[2].y, p[3].x, p[3].y);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hBrush);
    }*/
};

struct PositionXY {
    int x;
    int y;
};

void printShape(HDC hDC, char shape_c, vector<unique_ptr<Shape>>& shapes, PositionXY positionXY, HBRUSH hBrush, HGDIOBJ hOldBrush);
void default_shape(vector<unique_ptr<Shape>>& shapes, int midX, int midY);


random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uidX(0, 3);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC;
    HPEN hPen, oldPen;
    HBRUSH hBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static char shape_position[4];

    vector<unique_ptr<Shape>> shapes;
    // Triangle 인스턴스를 생성하고 shapes 벡터에 추가

    static PositionXY positionXY[4] = {
        {0,-200},
        {-300,0},
        {0,200},
        {300,0}
    };

    int midX, midY;

    Triangle triangle;

    switch (uMsg)
    {
    case WM_CREATE:
        shape_position[0] = 'T'; //삼각형
        shape_position[1] = 'H'; //모래시계
        shape_position[2] = 'P'; //오각형
        shape_position[3] = 'C'; //팩맨
        break;
    case WM_CHAR:
        break;
    case WM_SIZE:
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &rect);

        midX = (rect.right / 2);
        midY = (rect.bottom / 2);

        hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        oldPen = (HPEN)SelectObject(hDC, hPen);
        hBrush = CreateSolidBrush(RGB(128, 128, 128));
        oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        Rectangle(hDC, midX - 100, midY - 100, midX + 100, midY + 100);
        default_shape(shapes, midX, midY);
        SelectObject(hDC, oldPen); // 제자리 돌아가기
        SelectObject(hDC, oldBrush);
        DeleteObject(hPen); // 새로운 객체 삭제
        DeleteObject(hBrush);

        //---------------------------------------------------------------

        for (int i = 0; i < 4; ++i) {
            switch (i) {
            case 0: //가운데
                printShape(hDC, shape_position[i], shapes, positionXY[i], hBrush, oldBrush);
                break;
            case 1: //왼쪽
                printShape(hDC, shape_position[i], shapes, positionXY[i], hBrush, oldBrush);
                break;
            case 2: //아래
                printShape(hDC, shape_position[i], shapes, positionXY[i], hBrush, oldBrush);
                break;
            case 3: //오른쪽
                printShape(hDC, shape_position[i], shapes, positionXY[i], hBrush, oldBrush);
                break;
            }
        }

        EndPaint(hWnd, &ps);
        ReleaseDC(hWnd, hDC);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        break;

    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void printShape(HDC hDC, char shape_c, vector<unique_ptr<Shape>>& shapes, PositionXY positionXY, HBRUSH hBrush, HGDIOBJ hOldBrush) {
    Triangle* triangle;
    POINT* p1;
    POINT* p2;
    POINT p3;
    Hourglass *hourglass;
    Pentagon* pentagon;
    Pacman* pacman;
    switch (shape_c)
    {
    case 'T':
        triangle = dynamic_cast<Triangle*>(shapes[0].get());
        p1 = triangle->getPoints();
        // 새로운 꼭짓점 좌표 계산
        for (int i = 0; i < 3; ++i) {
            p1[i].x += positionXY.x;
            p1[i].y += positionXY.y;
        }
        hBrush = CreateSolidBrush(triangle->color);
        hOldBrush = SelectObject(hDC, hBrush);
        Polygon(hDC, p1, 3);
        SelectObject(hDC, hOldBrush);
        DeleteObject(hBrush);
        break;
    case 'H':
        hourglass = dynamic_cast<Hourglass*>(shapes[1].get());
        
        hBrush = CreateSolidBrush(hourglass->color);
        hOldBrush = SelectObject(hDC, hBrush);
        p1 = hourglass->getTopTriangle();
        p2 = hourglass->getBottomTriangle();

        for (int i = 0; i < 3; ++i) {
            p1[i].x += positionXY.x;
            p1[i].y += positionXY.y;
        }

        for (int i = 0; i < 3; ++i) {
            p2[i].x += positionXY.x;
            p2[i].y += positionXY.y;
        }

        // 상단 삼각형 그리기
        Polygon(hDC, p1, 3);
        // 하단 삼각형 그리기
        Polygon(hDC, p2, 3);
        SelectObject(hDC, hOldBrush);
        DeleteObject(hBrush);
        break;
    case 'P':
        pentagon = dynamic_cast<Pentagon*>(shapes[2].get());
        hBrush = CreateSolidBrush(pentagon->color);
        hOldBrush = SelectObject(hDC, hBrush);
        p1 = pentagon->getPoints();
        for (int i = 0; i < 5; ++i) {
            p1[i].x += positionXY.x;
            p1[i].y += positionXY.y;
        }
        Polygon(hDC, p1, 5);
        SelectObject(hDC, hOldBrush);
        DeleteObject(hBrush);
        break;
    case 'C':
        pacman = dynamic_cast<Pacman*>(shapes[3].get());
        hBrush = CreateSolidBrush(pacman->color);
        hOldBrush = SelectObject(hDC, hBrush);
        p1 = pacman->getPoints();
        for (int i = 0; i < 5; ++i) {
            p1[i].x += positionXY.x;
            p1[i].y += positionXY.y;
        }
        Pie(hDC, p1[0].x, p1[0].y, p1[1].x, p1[1].y, p1[2].x, p1[2].y, p1[3].x, p1[3].y);
        SelectObject(hDC, hOldBrush);
        DeleteObject(hBrush);
        break;
    default:
        break;
    }
}

void default_shape(vector<unique_ptr<Shape>>& shapes, int midX, int midY) {
    // Triangle 인스턴스를 생성하고 shapes 벡터에 추가
    POINT trianglePoints[3] = { {100, 200}, {200, 300}, {0, 300} };
    int offsetX = midX - ((trianglePoints[0].x + trianglePoints[1].x + trianglePoints[2].x) / 3);
    int offsetY = midY - ((trianglePoints[0].y + trianglePoints[1].y + trianglePoints[2].y) / 3);

    // 새로운 꼭짓점 좌표 계산
    for (int i = 0; i < 3; ++i) {
        trianglePoints[i].x += offsetX;
        trianglePoints[i].y += offsetY;
    }
    COLORREF triangleColor = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
    shapes.push_back(std::make_unique<Triangle>(trianglePoints, triangleColor));

    // Hourglass 인스턴스를 생성하고 shapes 벡터에 추가
    POINT hourglassCenter = { midX, midY };
    int hourglassWidth = 150;
    int hourglassHeight = 150;
    COLORREF hourglassColor = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
    shapes.push_back(std::make_unique<Hourglass>(hourglassCenter, hourglassWidth, hourglassHeight, hourglassColor));
   
    // Pentagon 인스턴스를 생성하고 shapes 벡터에 추가
    POINT pentagonPoints[5] = { {midX + 150, midY + 100}, {midX + 250, midY + 25}, {midX + 350, midY + 100}, {midX + 310, midY + 200}, {midX + 190, midY + 200} };
    for (int i = 0; i < 5; ++i) {
        pentagonPoints[i].x -= 250;
        pentagonPoints[i].y -= 100;
    }
    COLORREF pentagonColor = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
    shapes.push_back(std::make_unique<Pentagon>(pentagonPoints, pentagonColor));

    // Pacman 인스턴스를 생성하고 shapes 벡터에 추가
   // POINT pacmanPoints[4] = { {0,0}, {200, 200}, {100, 0},{200, 100} };
    POINT pacmanPoints[4] = { {midX - 100, midY - 100}, {midX + 100, midY + 100}, {midX, midY - 50},{midX, midY} };

    COLORREF pacmanColor = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
    shapes.push_back(std::make_unique<Pacman>(pacmanPoints, pacmanColor));
}