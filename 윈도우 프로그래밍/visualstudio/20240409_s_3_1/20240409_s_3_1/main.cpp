#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 3-1";
LPCTSTR lpszWindowName = L"실습 3-1";

#define WIDTH 900
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

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_VSCROLL, 0, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return Message.wParam;

}


#define gridSize 40

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uid_ran_4(0, 3);
uniform_int_distribution<int> uid_position(0, 39);
struct shape {
    int positionX;
    int positionY;
    HBRUSH mBrush, oldBrush;
    RECT circleRect;
    int cellSizeX;
    int cellSizeY;
    int move_i;
    COLORREF color = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
    bool m1_status = false;
    bool hero_lineX_status = false;
    bool hero_lineY_status = false;
    int move_R_i{ 0 };
    bool shape_status = false;
    int move_count = 0;
    void print_circle_shape(HDC& mDC) {

        circleRect.left = 0;
        circleRect.top = 0;
        circleRect.right = cellSizeX;
        circleRect.bottom = cellSizeY;

        circleRect.left += (positionX * cellSizeX);
        circleRect.right += (positionX * cellSizeX);
        circleRect.top += (positionY * cellSizeY);
        circleRect.bottom += (positionY * cellSizeY);

        mBrush = CreateSolidBrush(color);
        oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
        Ellipse(mDC, circleRect.left, circleRect.top, circleRect.right, circleRect.bottom);
        DeleteObject(mBrush);
    }

    void print_s(HDC& mDC) {
        if (shape_status) {
            print_circle_shape(mDC);
        }
        else {
            print_food_shape(mDC);
        }
    }

    void print_food_shape(HDC& mDC) {
        circleRect.left = 0;
        circleRect.top = 0;
        circleRect.right = cellSizeX;
        circleRect.bottom = cellSizeY;

        circleRect.left += (positionX * cellSizeX) + 2;
        circleRect.top += (positionY * cellSizeY) + 2;

        circleRect.right += (positionX * cellSizeX) - 2;
        circleRect.bottom += (positionY * cellSizeY) - 2;

        mBrush = CreateSolidBrush(color);
        oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
        Rectangle(mDC, circleRect.left, circleRect.top, circleRect.right, circleRect.bottom);
        DeleteObject(mBrush);
    }

};


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    int cellSizeX;
    int cellSizeY;
    static int hero_positionX{};
    static int hero_positionY{};
    static bool start_status = false;
    static int Timer1Count = 0;
    static int Timer2Count = 0;
    static int timerInterval = 50; // 초기 타이머 간격을 100ms로 설정


    static vector<shape> food_shapes;
    static vector<shape> hero_shapes;

    switch (uMsg)
    {
    case WM_CREATE:
    {

        SetTimer(hWnd, 2, 50, NULL);

        shape shape1;
        
        shape1.positionX = 0;
        shape1.positionY = 0;
        shape1.color = RGB(255, 0, 0);
        hero_shapes.push_back(shape1);

        for (int i = 0; i < 20; ++i) {
            //shape1.move_i = uid_ran_4(gen);
            shape1.positionX = uid_position(gen);
            shape1.positionY = uid_position(gen);
            shape1.color = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
            food_shapes.push_back(shape1);
        }

    }
    break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:

        hDC = BeginPaint(hWnd, &ps);

        InvalidateRect(hWnd, NULL, false);

        EndPaint(hWnd, &ps);
        break;
    case WM_CHAR:

        switch (wParam)
        {
        case  's': //start
            start_status = true;
            SetTimer(hWnd, 1, timerInterval, NULL);
            break;
        case '=':
            // 속도 증가: 간격 감소
            timerInterval = max(10, timerInterval - 10); // 최소 간격은 10ms로 제한
            SetTimer(hWnd, 1, timerInterval, NULL); // 타이머 재설정
            break;
        case '-':
            // 속도 감소: 간격 증가
            timerInterval = min(1000, timerInterval + 10); // 최대 간격은 1000ms로 제한
            SetTimer(hWnd, 1, timerInterval, NULL); // 타이머 재설정
            break;
        default:
            break;
        }

        InvalidateRect(hWnd, NULL, true);

        break;
    case WM_SIZE:
        break;
    case WM_PAINT:
    {
        GetClientRect(hWnd, &rect);
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        SelectObject(mDC, (HBITMAP)hBitmap);

        Rectangle(mDC, 0, 0, rect.right, rect.bottom);
        cellSizeX = rect.right / gridSize;
        cellSizeY = rect.bottom / gridSize;

        for (int y = 0; y <= gridSize; ++y) {
            MoveToEx(mDC, y * cellSizeX, 0, NULL);
            LineTo(mDC, y * cellSizeX, gridSize * cellSizeY);
        }

        for (int x = 0; x <= gridSize; ++x) {
            MoveToEx(mDC, 0, x * cellSizeY, NULL);
            LineTo(mDC, gridSize * cellSizeX, x * cellSizeY);
        }

        for (auto& shape : food_shapes) {
            shape.cellSizeX = cellSizeX;
            shape.cellSizeY = cellSizeY;
            shape.print_s(mDC);  // 먹이 도형
        }

        for (auto& shape : hero_shapes) {
            shape.cellSizeX = cellSizeX;
            shape.cellSizeY = cellSizeY;
            shape.print_circle_shape(mDC);  // 주인공 도형
        }

        for (auto& shape_f : food_shapes) {
            if (hero_shapes[0].positionX == shape_f.positionX && hero_shapes[0].positionY == shape_f.positionY && shape_f.shape_status == false) {
                shape_f.move_i = uid_ran_4(gen);
                shape_f.shape_status = true;
            }
        }

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        switch (wParam) {
        case 1:
            if (start_status) {
                if (Timer1Count % 2 == 0) {
                    for (int i = 0; i < hero_shapes.size(); ++i) {
                        // 수평 이동
                        hero_shapes[i].positionX += hero_shapes[i].hero_lineX_status ? -1 : 1;

                        // 수평 이동에 대한 경계 검사
                        if (hero_shapes[i].positionX > 39) {
                            hero_shapes[i].positionX = 39; // 최대값으로 제한
                            hero_shapes[i].hero_lineX_status = true; // 방향을 왼쪽으로 변경
                            hero_shapes[i].positionY += hero_shapes[i].hero_lineY_status ? -1 : 1; // 수직 이동
                        }
                        else if (hero_shapes[i].positionX < 0) {
                            hero_shapes[i].positionX = 0; // 최소값으로 제한
                            hero_shapes[i].hero_lineX_status = false; // 방향을 오른쪽으로 변경
                            hero_shapes[i].positionY += hero_shapes[i].hero_lineY_status ? -1 : 1; // 수직 이동
                        }

                        // 수직 경계 검사 및 방향 변경
                        if (hero_shapes[i].positionY > 39) {
                            hero_shapes[i].positionY = 39; // 아래쪽으로 제한
                            hero_shapes[i].hero_lineY_status = true; // 방향을 위로 변경
                        }
                        else if (hero_shapes[i].positionY < 0) {
                            hero_shapes[i].positionY = 0; // 위쪽으로 제한
                            hero_shapes[i].hero_lineY_status = false; // 방향을 아래로 변경
                        }
                    }
                }
            }
            Timer1Count++;
            break;
        case 2:
        {
           
            //먹이 생성
            if (Timer2Count % 2 == 0) {
                for (int i = 1; i < 20; ++i) {
                    switch (food_shapes[i].move_i)
                    {
                    case 0: // 먹이 이동 방법 1

                        if (food_shapes[i].positionX == 39)
                            food_shapes[i].m1_status = true;
                        if (food_shapes[i].positionX == 0)
                            food_shapes[i].m1_status = false;

                        food_shapes[i].positionX += (food_shapes[i].m1_status == true ? -1 : 1); // 방향에 따라 오른쪽 또는 왼쪽으로 이동
                        break;
                    case 1: // 먹이 이동 방법 2
                        if (food_shapes[i].positionY == 39)
                            food_shapes[i].m1_status = true; // 위로 이동하는 방향으로 변경
                        if (food_shapes[i].positionY == 0)
                            food_shapes[i].m1_status = false; // 아래로 이동하는 방향으로 변경

                        food_shapes[i].positionY += (food_shapes[i].m1_status ? -1 : 1); // 현재 방향에 따라 위 또는 아래로 이동
                        break;
                    case 2: // 먹이 이동 방법 3
                        if (food_shapes[i].move_count >= 5) { // 5칸을 이동했다면 방향 전환
                            food_shapes[i].move_count = 0; // 카운트 리셋
                            food_shapes[i].move_R_i = (food_shapes[i].move_R_i + 1) % 4; // 다음 방향 설정
                        }

                        switch (food_shapes[i].move_R_i) {
                        case 0: // 오른쪽으로 이동
                            food_shapes[i].positionX++;
                            break;
                        case 1: // 아래로 이동
                            food_shapes[i].positionY++;
                            break;
                        case 2: // 왼쪽으로 이동
                            food_shapes[i].positionX--;
                            break;
                        case 3: // 위로 이동
                            food_shapes[i].positionY--;
                            break;
                        default:
                            break;
                        }

                        food_shapes[i].move_count++; // 이동 카운트 증가
                        break;
                    case 3: // 먹이 이동 방법 4 
                        break;
                    default:
                        break;
                    }
                }
            }
            Timer2Count++;
            break;
        }
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}