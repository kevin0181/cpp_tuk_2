#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"�ǽ� 3-3";
LPCTSTR lpszWindowName = L"�ǽ� 3-3";

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

struct carS {
    RECT rect;
    COLORREF color;
    int line_status;
    int speed;
};

void print_line(HDC& mDC, RECT& rect);
void print_crosswalk1(HDC& mDC, RECT& rect);
void print_crosswalk2(HDC& mDC, RECT& rect);
void print_traffic_light1(HDC& mDC, RECT& rect);
void print_traffic_light2(HDC& mDC, RECT& rect);
void print_traffic_light3(HDC& mDC, RECT& rect);
void print_traffic_light4(HDC& mDC, RECT& rect);
void line_set(HDC& mDC, RECT& rect, carS& car);
void car_move(HDC& mDC, RECT& rect, carS& car, carS& over_cars);

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uid_speed(1, 20);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static int Timer1Count = 0;
    static vector<carS> cars;
    static vector<carS> over_cars;
    static bool status = true;
    switch (uMsg)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, 1, 1, NULL);

        for (int i = 0; i < 4; ++i) {
            carS car;
            carS ov_car;
            over_cars.push_back(ov_car);
            car.color = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
            car.line_status = i;
            if (i == 0 || i == 1) {
                car.rect.left = 0;
                car.rect.top = 0;
                car.rect.right = 150;
                car.rect.bottom = 60;
            }
            else {
                car.rect.left = 0;
                car.rect.top = 0;
                car.rect.right = 60;
                car.rect.bottom = 150;
            }
            car.speed = uid_speed(gen);
            cars.push_back(car);
        }
        break;
    }
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // Ű���� Ű�� ������ ��
        break;
    case WM_LBUTTONDOWN: {  // ���콺 ���� ��ư Ŭ��
        break;
    }
    case WM_RBUTTONDOWN: {  // ���콺 ������ ��ư Ŭ��
        break;
    }
    case WM_CHAR:
        switch (wParam)
        {
        case 'q':
            PostQuitMessage(0);
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
        Rectangle(mDC, 0, 0, rect.right, rect.bottom); //���� ���

        print_line(mDC, rect); //���� �׸���
        print_crosswalk1(mDC, rect); //Ⱦ�ܺ��� �׸���
        print_crosswalk2(mDC, rect); //Ⱦ�ܺ���2 �׸���
        print_traffic_light1(mDC, rect); //��ȣ��1 �׸���
        print_traffic_light2(mDC, rect); //��ȣ��2 �׸���
        print_traffic_light3(mDC, rect); //��ȣ��3 �׸���
        print_traffic_light4(mDC, rect); //��ȣ��4 �׸���

        if (status) {
            for (int i = 0; i < cars.size(); ++i) {
                line_set(mDC, rect, cars[i]);
            }
            status = false;
        }

        for (int i = 0; i < cars.size(); ++i) {
            HBRUSH hBrush = CreateSolidBrush(cars[i].color);
            FillRect(mDC, &cars[i].rect, hBrush);
            DeleteObject(hBrush);
        }

        //�Ѿ �κ� ǥ���ϴ� ��
        for (int i = 0; i < over_cars.size(); ++i) {
            HBRUSH hBrush = CreateSolidBrush(over_cars[i].color);
            FillRect(mDC, &over_cars[i].rect, hBrush);
            DeleteObject(hBrush);
        }


        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);
        DeleteObject(hBitmap); // ������ ��Ʈ�� ����
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        switch (wParam) {
        case 1:
            for (int i = 0; i < 4; ++i) {
                car_move(mDC, rect, cars[i], over_cars[i]);
            }

            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1); // Ÿ�̸� ����
        PostQuitMessage(0);
        break;
    default:
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void car_move(HDC& mDC, RECT& rect, carS& car, carS& over_cars) {
    switch (car.line_status)
    {
    case 0: {
        car.rect.left += car.speed;   // ������ �ӵ��� �ش��ϴ� ���� ��ǥ�� ����
        car.rect.right += car.speed;  // ������ �ӵ��� �ش��ϴ� ���� ��ǥ�� ����

        if (car.rect.right >= rect.right) {
            if (car.rect.left > rect.right) {
                car.rect.left = 0;
                car.rect.right = 150;
                //-----
                over_cars.rect.left = 0;
                over_cars.rect.top = 0;
                over_cars.rect.right = 0;
                over_cars.rect.bottom = 0;
            }
            over_cars.rect.left = 0;
            over_cars.rect.top = car.rect.top;
            over_cars.rect.right = car.rect.right - rect.right;
            over_cars.rect.bottom = car.rect.bottom;
            over_cars.color = car.color;
            over_cars.line_status = car.line_status;
        }
        break;
    }
    default:
        break;
    }
}

void line_set(HDC& mDC, RECT& rect, carS& car) {
    HBRUSH hBrush = CreateSolidBrush(car.color);
    switch (car.line_status)
    {
    case 0: // ���ʿ��� ������
    {
        car.rect.top += rect.bottom / 2 + 23;
        car.rect.bottom += rect.bottom / 2 + 23;

        break;
    }
    case 1: // �����ʿ��� ����
        car.rect.left += rect.right - 150;
        car.rect.right += rect.right;
        car.rect.top += rect.bottom / 2 - 60 - 23;
        car.rect.bottom += rect.bottom / 2 - 60 - 23;
        break;
    case 2: // ������ �Ʒ���
        car.rect.left += rect.right / 2 - 60 - 23;
        car.rect.right += rect.right / 2 - 60 - 23;
        break;
    case 3: // �Ʒ����� ����
        car.rect.top += rect.bottom - 150;
        car.rect.bottom += rect.bottom;
        car.rect.left += rect.right / 2 + 23;
        car.rect.right += rect.right / 2 + 23;
        break;
    default:
        break;
    }
}

void print_line(HDC &mDC, RECT& rect) {
    HPEN mPen, oldPen;
    RECT centerRect;

    HBRUSH hBrush = CreateSolidBrush(RGB(125, 125, 125)); // ������ �귯�� ����

    centerRect.left = 0;
    centerRect.top = rect.bottom / 2 - 100;
    centerRect.right = rect.right;
    centerRect.bottom = rect.bottom / 2 + 100;
    FillRect(mDC, &centerRect, hBrush);

    centerRect.left = rect.right / 2 - 100;
    centerRect.top = 0;
    centerRect.right = rect.right / 2 + 100;
    centerRect.bottom = rect.bottom;
    FillRect(mDC, &centerRect, hBrush);

    // �β��� ���� �� ����
    LOGBRUSH lb;
    lb.lbStyle = BS_SOLID;
    lb.lbColor = RGB(200, 200, 0);
    lb.lbHatch = 0;

    DWORD penStyle = PS_DOT | PS_GEOMETRIC | PS_ENDCAP_FLAT;
    mPen = ExtCreatePen(penStyle, 3, &lb, 0, NULL);  // �β� 10�� ����
    oldPen = (HPEN)SelectObject(mDC, mPen);

    MoveToEx(mDC, 0, rect.bottom / 2, NULL);
    LineTo(mDC, rect.right, rect.bottom / 2);

    mPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    oldPen = (HPEN)SelectObject(mDC, mPen);

    MoveToEx(mDC, 0, rect.bottom / 2 - 100, NULL);
    LineTo(mDC, rect.right, rect.bottom / 2 - 100);

    MoveToEx(mDC, 0, rect.bottom / 2 + 100, NULL);
    LineTo(mDC, rect.right, rect.bottom / 2 + 100);

    // ------------------------------------

    mPen = ExtCreatePen(penStyle, 3, &lb, 0, NULL);  // �β� 10�� ����
    oldPen = (HPEN)SelectObject(mDC, mPen);

    MoveToEx(mDC, rect.right / 2, 0, NULL);
    LineTo(mDC, rect.right / 2, rect.bottom);

    mPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    oldPen = (HPEN)SelectObject(mDC, mPen);

    MoveToEx(mDC, rect.right / 2 - 100, 0, NULL);
    LineTo(mDC, rect.right / 2 - 100, rect.bottom);

    MoveToEx(mDC, rect.right / 2 + 100, 0, NULL);
    LineTo(mDC, rect.right / 2 + 100, rect.bottom);

    hBrush = CreateSolidBrush(RGB(125, 125, 125));
    centerRect.left = rect.right / 2 - 100 - 2;
    centerRect.top = rect.bottom / 2 - 100 - 2;
    centerRect.right = rect.right / 2 + 100 + 2;
    centerRect.bottom = rect.bottom / 2 + 100 + 2;
    FillRect(mDC, &centerRect, hBrush);

    SelectObject(mDC, oldPen);
    DeleteObject(mPen);
    DeleteObject(oldPen);
}

void print_crosswalk1(HDC& mDC, RECT& rect) {
    // Ⱦ�ܺ����� �׸��� �Լ��� ���� ������ �� �ֽ��ϴ�.

    const int crosswalkHeight = 20; // Ⱦ�ܺ��� �� ���� ����
    const int crosswalkInterval = 20; // Ⱦ�ܺ��� �� ������ ����
    const int crosswalkStart = rect.bottom / 2 - 100 + 10; // Ⱦ�ܺ����� ���۵Ǵ� y ��ǥ
    const int crosswalkEnd = rect.bottom / 2 + 100 - 5; // Ⱦ�ܺ����� ������ y ��ǥ

    HBRUSH hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255)); // ��� �귯�� ����
    HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hWhiteBrush);

    for (int y = crosswalkStart; y < crosswalkEnd; y += crosswalkHeight + crosswalkInterval) {
        RECT crosswalkRect = { rect.right / 2 + 100, y, rect.right / 2 + 170, y + crosswalkHeight };
        FillRect(mDC, &crosswalkRect, hWhiteBrush); // Ⱦ�ܺ��� ��Ʈ������ �׸���
    }

    for (int y = crosswalkStart; y < crosswalkEnd; y += crosswalkHeight + crosswalkInterval) {
        RECT crosswalkRect = { rect.right / 2 - 170, y, rect.right / 2 - 100, y + crosswalkHeight };
        FillRect(mDC, &crosswalkRect, hWhiteBrush); // Ⱦ�ܺ��� ��Ʈ������ �׸���
    }


    // �귯�� ��ȯ �� ����
    SelectObject(mDC, oldBrush);
    DeleteObject(hWhiteBrush);
}

void print_crosswalk2(HDC& mDC, RECT& rect) {

    const int crosswalkHeight = 20; // Ⱦ�ܺ��� �� ���� ����
    const int crosswalkWidth = 20;  // Ⱦ�ܺ��� �� ���� �ʺ�
    const int crosswalkInterval = 20; // Ⱦ�ܺ��� �� ������ ����
    const int crosswalkStart = rect.right / 2 - 100 +10; // Ⱦ�ܺ����� ���۵Ǵ� x ��ǥ
    const int crosswalkEnd = rect.right / 2 + 100 - 5;   // Ⱦ�ܺ����� ������ x ��ǥ

    HBRUSH hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255)); // ��� �귯�� ����
    HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hWhiteBrush);

    for (int x = crosswalkStart; x < crosswalkEnd; x += crosswalkWidth + crosswalkInterval) {
        RECT crosswalkRect = { x, rect.bottom / 2 - 170, x + crosswalkWidth, rect.bottom / 2 - 100 };
        FillRect(mDC, &crosswalkRect, hWhiteBrush); // ���� ���� Ⱦ�ܺ��� ��Ʈ������ �׸���
    }

    for (int x = crosswalkStart; x < crosswalkEnd; x += crosswalkWidth + crosswalkInterval) {
        RECT crosswalkRect = { x, rect.bottom / 2 + 100 , x + crosswalkWidth, rect.bottom / 2 + 170 };
        FillRect(mDC, &crosswalkRect, hWhiteBrush); // ���� ���� Ⱦ�ܺ��� ��Ʈ������ �׸���
    }

    // �귯�� ��ȯ �� ����
    SelectObject(mDC, oldBrush);
    DeleteObject(hWhiteBrush);
}

void print_traffic_light1(HDC& mDC, RECT& rect) {
    // ��ȣ�� ��ġ ����
    int lightCenterX = rect.right / 2 - 200; // ��ȣ�� �߽� x ��ǥ
    int lightCenterY = rect.bottom / 2 - 150; // ��ȣ�� �߽� y ��ǥ

    int lightLength = 80; // ��ȣ�� ��ü ����
    int lightWidth = 30; // ��ȣ�� �ʺ�

    // ��ȣ�� ��� �׸���
    HBRUSH hGreyBrush = CreateSolidBrush(RGB(50, 50, 50));
    RECT poleRect = { lightCenterX, lightCenterY + 15, lightCenterX + lightLength, lightCenterY + 35 };
    FillRect(mDC, &poleRect, hGreyBrush);

    // ��ȣ�� �Һ� �׸���
    int radius = 10; // �Һ��� ������
    HBRUSH hRedLightBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH hYellowLightBrush = CreateSolidBrush(RGB(255, 255, 0));
    HBRUSH hGreenLightBrush = CreateSolidBrush(RGB(0, 255, 0));

    // ������
    HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hRedLightBrush);
    Ellipse(mDC, lightCenterX, (lightCenterY - radius) + 25, lightCenterX + 2 * radius, (lightCenterY + radius) + 25);

    // �����
    SelectObject(mDC, hYellowLightBrush);
    Ellipse(mDC, lightCenterX + 30, (lightCenterY - radius) + 25, lightCenterX + 2 * radius + 30, (lightCenterY + radius) + 25);

    // �ʷϺ�
    SelectObject(mDC, hGreenLightBrush);
    Ellipse(mDC, lightCenterX + 60, (lightCenterY - radius) + 25, lightCenterX + 2 * radius + 60, (lightCenterY + radius) + 25);

    // �귯�� ��ȯ �� ����
    SelectObject(mDC, oldBrush);
    DeleteObject(hGreyBrush);
    DeleteObject(hRedLightBrush);
    DeleteObject(hYellowLightBrush);
    DeleteObject(hGreenLightBrush);
    DeleteObject(oldBrush);
}

void print_traffic_light2(HDC& mDC, RECT& rect) {
    // ��ȣ�� ��ġ ����
    int lightCenterX = rect.right / 2 + 120; // ��ȣ�� �߽� x ��ǥ
    int lightCenterY = rect.bottom / 2 + 100; // ��ȣ�� �߽� y ��ǥ

    int lightLength = 80; // ��ȣ�� ��ü ����
    int lightWidth = 30; // ��ȣ�� �ʺ�

    // ��ȣ�� ��� �׸���
    HBRUSH hGreyBrush = CreateSolidBrush(RGB(50, 50, 50));
    RECT poleRect = { lightCenterX, lightCenterY + 15, lightCenterX + lightLength, lightCenterY + 35 };
    FillRect(mDC, &poleRect, hGreyBrush);

    // ��ȣ�� �Һ� �׸���
    int radius = 10; // �Һ��� ������
    HBRUSH hRedLightBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH hYellowLightBrush = CreateSolidBrush(RGB(255, 255, 0));
    HBRUSH hGreenLightBrush = CreateSolidBrush(RGB(0, 255, 0));

    // ������
    HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hRedLightBrush);
    Ellipse(mDC, lightCenterX, (lightCenterY - radius) + 25, lightCenterX + 2 * radius, (lightCenterY + radius) + 25);

    // �����
    SelectObject(mDC, hYellowLightBrush);
    Ellipse(mDC, lightCenterX + 30, (lightCenterY - radius) + 25, lightCenterX + 2 * radius + 30, (lightCenterY + radius) + 25);

    // �ʷϺ�
    SelectObject(mDC, hGreenLightBrush);
    Ellipse(mDC, lightCenterX + 60, (lightCenterY - radius) + 25, lightCenterX + 2 * radius + 60, (lightCenterY + radius) + 25);

    // �귯�� ��ȯ �� ����
    SelectObject(mDC, oldBrush);
    DeleteObject(hGreyBrush);
    DeleteObject(hRedLightBrush);
    DeleteObject(hYellowLightBrush);
    DeleteObject(hGreenLightBrush);
    DeleteObject(oldBrush);
}


void print_traffic_light3(HDC& mDC, RECT& rect) {
    // ��ȣ�� ��ġ ����
    int lightX = rect.right / 2 + 130; // ��ȣ�� �߽� x ��ǥ
    int lightY = rect.bottom / 2 - 200; // ��ȣ�� �߽� y ��ǥ

    int lightWidth = 30; // ��ȣ�� �ʺ�
    int lightHeight = 80; // ��ȣ�� ����

    // ��ȣ�� ��� �׸���
    HBRUSH hGreyBrush = CreateSolidBrush(RGB(50, 50, 50));
    RECT poleRect = { lightX - 10, lightY, lightX + 10, lightY + lightHeight };
    FillRect(mDC, &poleRect, hGreyBrush);

    // ��ȣ�� �Һ� �׸���
    int radius = 10; // �Һ��� ������
    HBRUSH hRedLightBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH hYellowLightBrush = CreateSolidBrush(RGB(255, 255, 0));
    HBRUSH hGreenLightBrush = CreateSolidBrush(RGB(0, 255, 0));

    // ������
    HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hRedLightBrush);
    Ellipse(mDC, lightX - radius, lightY, lightX + radius, lightY + 2 * radius);

    // �����
    SelectObject(mDC, hYellowLightBrush);
    Ellipse(mDC, lightX - radius, lightY + 30, lightX + radius, lightY + 2 * radius + 30);

    // �ʷϺ�
    SelectObject(mDC, hGreenLightBrush);
    Ellipse(mDC, lightX - radius, lightY + 60, lightX + radius, lightY + 2 * radius + 60);

    // �귯�� ��ȯ �� ����
    SelectObject(mDC, oldBrush);
    DeleteObject(hGreyBrush);
    DeleteObject(hRedLightBrush);
    DeleteObject(hYellowLightBrush);
    DeleteObject(hGreenLightBrush);
}

void print_traffic_light4(HDC& mDC, RECT& rect) {
    // ��ȣ�� ��ġ ����
    int lightX = rect.right / 2 - 130; // ��ȣ�� �߽� x ��ǥ
    int lightY = rect.bottom / 2 + 120; // ��ȣ�� �߽� y ��ǥ

    int lightWidth = 30; // ��ȣ�� �ʺ�
    int lightHeight = 80; // ��ȣ�� ����

    // ��ȣ�� ��� �׸���
    HBRUSH hGreyBrush = CreateSolidBrush(RGB(50, 50, 50));
    RECT poleRect = { lightX - 10, lightY, lightX + 10, lightY + lightHeight };
    FillRect(mDC, &poleRect, hGreyBrush);

    // ��ȣ�� �Һ� �׸���
    int radius = 10; // �Һ��� ������
    HBRUSH hRedLightBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH hYellowLightBrush = CreateSolidBrush(RGB(255, 255, 0));
    HBRUSH hGreenLightBrush = CreateSolidBrush(RGB(0, 255, 0));

    // ������
    HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hRedLightBrush);
    Ellipse(mDC, lightX - radius, lightY, lightX + radius, lightY + 2 * radius);

    // �����
    SelectObject(mDC, hYellowLightBrush);
    Ellipse(mDC, lightX - radius, lightY + 30, lightX + radius, lightY + 2 * radius + 30);

    // �ʷϺ�
    SelectObject(mDC, hGreenLightBrush);
    Ellipse(mDC, lightX - radius, lightY + 60, lightX + radius, lightY + 2 * radius + 60);

    // �귯�� ��ȯ �� ����
    SelectObject(mDC, oldBrush);
    DeleteObject(hGreyBrush);
    DeleteObject(hRedLightBrush);
    DeleteObject(hYellowLightBrush);
    DeleteObject(hGreenLightBrush);
}