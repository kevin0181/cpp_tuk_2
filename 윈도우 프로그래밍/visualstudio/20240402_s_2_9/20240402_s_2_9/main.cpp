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
    COLORREF originalColor; // 원래 색상을 저장하는 변수
    POINT p[3];
    COLORREF color;
    Triangle() {}
    Triangle(POINT getP[3], COLORREF color) : color(color) {
        for (int i = 0; i < 3; ++i) {
            p[i] = getP[i];
        }
        originalColor = color;
    }

    POINT* getPoints() {
        return p;
    }

    void setColor(COLORREF color1) {
        color = color1;
    }

    // 설정자 함수: 꼭짓점 배열 설정
    void setPoints(POINT newP[3]) {
        for (int i = 0; i < 3; ++i) {
            p[i] = newP[i];
        }
    }
    void restoreOriginalColor() {
        color = originalColor; // 원래 색상으로 복구하는 함수
    }
};

class Hourglass : public Shape {
public:
    COLORREF originalColor; // 원래 색상을 저장하는 변수
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
        originalColor = color;
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

    void setColor(COLORREF color1) {
        color = color1;
    }

    void restoreOriginalColor() {
        color = originalColor; // 원래 색상으로 복구하는 함수
    }

};

class Pentagon : public Shape {
public:
    COLORREF originalColor; // 원래 색상을 저장하는 변수
    POINT p[5];
    COLORREF color;
    Pentagon() {}
    Pentagon(POINT getP[5], COLORREF color) : color(color) {
        for (int i = 0; i < 5; ++i) {
            p[i] = getP[i];
        }
        originalColor = color;
    }

    POINT* getPoints() {
        return p;
    }

    void setColor(COLORREF color1) {
        color = color1;
    }
    void restoreOriginalColor() {
        color = originalColor; // 원래 색상으로 복구하는 함수
    }
};

class Pacman : public Shape {
public:
    COLORREF originalColor; // 원래 색상을 저장하는 변수
    POINT p[4];
    COLORREF color;

    Pacman() {}
    Pacman(POINT getP[4], COLORREF color) : color(color) {
        for (int i = 0; i < 4; ++i) {
            p[i] = getP[i];
        }
        originalColor = color;
    }
    POINT* getPoints() {
        return p;
    }

    void setColor(COLORREF color1) {
        color = color1;
    }
    void restoreOriginalColor() {
        color = originalColor; // 원래 색상으로 복구하는 함수
    }
};

struct PositionXY {
    int x;
    int y;
};

void printMidShape(HDC hDC, char shape_c, vector<unique_ptr<Shape>>& shapes, PositionXY positionXY, HBRUSH hBrush, HGDIOBJ hOldBrush);
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
    char shape_c;
    static vector<unique_ptr<Shape>> shapes;
    // Triangle 인스턴스를 생성하고 shapes 벡터에 추가

    static PositionXY positionXY[4] = {
        {0,-200},
        {-300,0},
        {0,200},
        {300,0}
    };

    int midX, midY;

    Triangle* triangle;
    Hourglass* hourglass;
    Pentagon* pentagon;
    Pacman* pacman;
    
    switch (uMsg)
    {
    case WM_CREATE:
        shape_position[0] = 'T'; //삼각형
        shape_position[1] = 'H'; //모래시계
        shape_position[2] = 'P'; //오각형
        shape_position[3] = 'C'; //팩맨

        //// HDC를 얻기 위해 GetDC를 사용합니다.
        //hDC = GetDC(hWnd);
        //GetClientRect(hWnd, &rect);

        //midX = (rect.right / 2);
        //midY = (rect.bottom / 2);
        //default_shape(shapes, midX, midY);

        //// 더 이상 필요하지 않으므로 DC를 해제합니다.
        //ReleaseDC(hWnd, hDC);

        break;
    case WM_KEYUP:
        switch (wParam)
        {
        case 0x53:
            hourglass = dynamic_cast<Hourglass*>(shapes[1].get());
            hourglass->restoreOriginalColor();
            break;
        case 0x54:
            triangle = dynamic_cast<Triangle*>(shapes[0].get());
            triangle->restoreOriginalColor();
            break;
        case 0x50:
            pentagon = dynamic_cast<Pentagon*>(shapes[2].get());
            pentagon->restoreOriginalColor();
            break;
        case 0x45:
            pacman = dynamic_cast<Pacman*>(shapes[3].get());
            pacman->restoreOriginalColor();
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, TRUE);

        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case 0x51:
            PostQuitMessage(0);
            break;
        case 0x53:
            hourglass = dynamic_cast<Hourglass*>(shapes[1].get());
            hourglass->setColor(RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen)));
            while (true) {
                if ('H' == shape_position[0]) {
                    break;
                }

                shape_c = shape_position[0];
                for (int i = 0; i < 3; ++i) {
                    shape_position[i] = shape_position[i + 1];
                }
                shape_position[3] = shape_c;

            }
            break;
        case 0x54:
            triangle = dynamic_cast<Triangle*>(shapes[0].get());
            triangle->setColor(RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen)));
            while (true) {
                if ('T' == shape_position[0]) {
                    break;
                }

                shape_c = shape_position[0];
                for (int i = 0; i < 3; ++i) {
                    shape_position[i] = shape_position[i + 1];
                }
                shape_position[3] = shape_c;

            }
            break;
        case 0x50:
            pentagon = dynamic_cast<Pentagon*>(shapes[2].get());
            pentagon->setColor(RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen)));
            while (true) {
                if ('P' == shape_position[0]) {
                    break;
                }

                shape_c = shape_position[0];
                for (int i = 0; i < 3; ++i) {
                    shape_position[i] = shape_position[i + 1];
                }
                shape_position[3] = shape_c;

            }
            break;
        case 0x45:
            pacman = dynamic_cast<Pacman*>(shapes[3].get());
            pacman->setColor(RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen)));
            while (true) {
                if ('C' == shape_position[0]) {
                    break;
                }

                shape_c = shape_position[0];
                for (int i = 0; i < 3; ++i) {
                    shape_position[i] = shape_position[i + 1];
                }
                shape_position[3] = shape_c;

            }
            break;
        case VK_LEFT:
            shape_c = shape_position[3];

            for (int i = 3; i >= 0; --i) {
                shape_position[i] = shape_position[i - 1];
            }
            shape_position[0] = shape_c;

            break;
        case VK_RIGHT:

            shape_c = shape_position[0];
            for (int i = 0; i < 3; ++i) {
                shape_position[i] = shape_position[i + 1];
            }

            shape_position[3] = shape_c;
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, TRUE);
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

        if (shapes.size() != 0) {

            for (int i = 0; i < 4; ++i) {
                switch (i) {
                case 0: //가운데
                    printMidShape(hDC, shape_position[i], shapes, positionXY[i], hBrush, oldBrush);
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

        }

        EndPaint(hWnd, &ps);
        ReleaseDC(hWnd, hDC);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
void printMidShape(HDC hDC, char shape_c, vector<unique_ptr<Shape>>& shapes, PositionXY positionXY, HBRUSH hBrush, HGDIOBJ hOldBrush) {
    POINT* p1;
    POINT* p2;
    POINT p4[3]{};
    Triangle* triangle;
    Hourglass* hourglass;
    Pentagon* pentagon;
    Pacman* pacman;
    POINT centroid;
    switch (shape_c)
    {
    case 'T':
        triangle = dynamic_cast<Triangle*>(shapes[0].get());
        p1 = triangle->getPoints();
        hBrush = CreateSolidBrush(triangle->color);
        hOldBrush = SelectObject(hDC, hBrush);
        for (int i = 0; i < 3; ++i) {
            p4[i] = p1[i];
        }
        /*centroid.x = (p4[0].x + p4[1].x + p4[2].x) / 3;
        centroid.y = (p4[0].y + p4[1].y + p4[2].y) / 3;
        for (int i = 0; i < 3; ++i) {
            p4[i].x = centroid.x + static_cast<LONG>((p4[i].x - centroid.x) * 1.2);
            p4[i].y = centroid.y + static_cast<LONG>((p4[i].y - centroid.y) * 1.2);
        }*/
        Polygon(hDC, p4, 3);
        SelectObject(hDC, hOldBrush);
        DeleteObject(hBrush);
        break;
    case 'H':
        hourglass = dynamic_cast<Hourglass*>(shapes[1].get());

        hBrush = CreateSolidBrush(hourglass->color);
        hOldBrush = SelectObject(hDC, hBrush);
        p1 = hourglass->getTopTriangle();
        p2 = hourglass->getBottomTriangle();

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
        Polygon(hDC, p1, 5);
        SelectObject(hDC, hOldBrush);
        DeleteObject(hBrush);
        break;
    case 'C':
        pacman = dynamic_cast<Pacman*>(shapes[3].get());
        hBrush = CreateSolidBrush(pacman->color);
        hOldBrush = SelectObject(hDC, hBrush);
        p1 = pacman->getPoints();
        Pie(hDC, p1[0].x, p1[0].y, p1[1].x, p1[1].y, p1[2].x, p1[2].y, p1[3].x, p1[3].y);
        SelectObject(hDC, hOldBrush);
        DeleteObject(hBrush);
        break;
    default:
        break;
    }
}

void printShape(HDC hDC, char shape_c, vector<unique_ptr<Shape>>& shapes, PositionXY positionXY, HBRUSH hBrush, HGDIOBJ hOldBrush) {
    Triangle* triangle;
    POINT* p1;
    POINT* p2;
    POINT p3;
    Hourglass* hourglass;
    Pentagon* pentagon;
    Pacman* pacman;

    POINT p4[3]{};
    POINT p5[3]{};
    POINT p6[5]{};
    switch (shape_c)
    {
    case 'T':
        triangle = dynamic_cast<Triangle*>(shapes[0].get());
        p1 = triangle->getPoints();
        // 새로운 꼭짓점 좌표 계산

        for (int i = 0; i < 3; ++i) {
            p4[i].x += p1[i].x;
            p4[i].y += p1[i].y;
        }

        for (int i = 0; i < 3; ++i) {
            p4[i].x += positionXY.x;
            p4[i].y += positionXY.y;
        }
        hBrush = CreateSolidBrush(triangle->color);
        hOldBrush = SelectObject(hDC, hBrush);
        Polygon(hDC, p4, 3);
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
            p4[i].x += p1[i].x;
            p4[i].y += p1[i].y;
        }


        for (int i = 0; i < 3; ++i) {
            p5[i].x += p2[i].x;
            p5[i].y += p2[i].y;
        }

        for (int i = 0; i < 3; ++i) {
            p4[i].x += positionXY.x;
            p4[i].y += positionXY.y;
        }

        for (int i = 0; i < 3; ++i) {
            p5[i].x += positionXY.x;
            p5[i].y += positionXY.y;
        }

        // 상단 삼각형 그리기
        Polygon(hDC, p4, 3);
        // 하단 삼각형 그리기
        Polygon(hDC, p5, 3);
        SelectObject(hDC, hOldBrush);
        DeleteObject(hBrush);
        break;
    case 'P':
        pentagon = dynamic_cast<Pentagon*>(shapes[2].get());
        hBrush = CreateSolidBrush(pentagon->color);
        hOldBrush = SelectObject(hDC, hBrush);
        p1 = pentagon->getPoints();

        for (int i = 0; i < 5; ++i) {
            p6[i].x += p1[i].x;
            p6[i].y += p1[i].y;
        }

        for (int i = 0; i < 5; ++i) {
            p6[i].x += positionXY.x;
            p6[i].y += positionXY.y;
        }
        Polygon(hDC, p6, 5);
        SelectObject(hDC, hOldBrush);
        DeleteObject(hBrush);
        break;
    case 'C':
        pacman = dynamic_cast<Pacman*>(shapes[3].get());
        hBrush = CreateSolidBrush(pacman->color);
        hOldBrush = SelectObject(hDC, hBrush);
        p1 = pacman->getPoints();

        for (int i = 0; i < 5; ++i) {
            p6[i].x += p1[i].x;
            p6[i].y += p1[i].y;
        }
        for (int i = 0; i < 5; ++i) {
            p6[i].x += positionXY.x;
            p6[i].y += positionXY.y;
        }
        Pie(hDC, p6[0].x, p6[0].y, p6[1].x, p6[1].y, p6[2].x, p6[2].y, p6[3].x, p6[3].y);
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