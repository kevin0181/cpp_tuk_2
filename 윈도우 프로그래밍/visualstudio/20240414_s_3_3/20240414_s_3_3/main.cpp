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

struct TrafficLight {
    RECT rect;
    COLORREF color;
    bool status;
};

void print_line(HDC& mDC, RECT& rect);
void print_crosswalk1(HDC& mDC, RECT& rect);
void print_crosswalk2(HDC& mDC, RECT& rect);
void print_traffic_light1(HDC& mDC, RECT& rect, vector<TrafficLight> trafficLight1);
void print_traffic_light3(HDC& mDC, RECT& rect, vector<TrafficLight> trafficLight2);
void line_set(HDC& mDC, RECT& rect, carS& car);
void car_move(HDC& mDC, RECT& rect, carS& car, carS& over_cars, vector<TrafficLight> trafficLight2, vector<TrafficLight> trafficLight1);

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uid_speed(1, 20);
uniform_int_distribution<int> uid_time(3000, 10000);

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
    static vector<TrafficLight> trafficLight1; // ���Ʒ�
    static vector<TrafficLight> trafficLight2; // �¿�
    static bool status = true;
    static int ran_blue_time = 0;
    static int traffic_status = 0; // 0 => �¿�, 1 => ���Ʒ�

    switch (uMsg)
    {
    case WM_CREATE:
    {
        ran_blue_time = uid_time(gen);
        SetTimer(hWnd, 1, 1, NULL);
        SetTimer(hWnd, 2, ran_blue_time, NULL); // Blue

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

        // create traffic
        TrafficLight tr;
        for (int i = 0; i < 3; ++i) {
            if (i == 0) {
                tr.status = true;
            }
            else {
                tr.status = false;
            }
            trafficLight1.push_back(tr);
        }

        for (int i = 0; i < 3; ++i) {
            if (i == 2) {
                tr.status = true;
            }
            else {
                tr.status = false;
            }
            trafficLight2.push_back(tr);
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
        case '-':
            for (int i = 0; i < cars.size(); ++i) {
                if (cars[i].speed  < 2) {
                    break;
                }
                cars[i].speed--;
            }
            break;
        case '=':
            for (int i = 0; i < cars.size(); ++i) {
                cars[i].speed++;
            }
            break;
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
        print_traffic_light1(mDC, rect, trafficLight1); //��ȣ��1 �׸���
        print_traffic_light3(mDC, rect, trafficLight2); //��ȣ��3 �׸���

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
                car_move(mDC, rect, cars[i], over_cars[i], trafficLight2, trafficLight1);
            }
            break;
        case 2: // blue

            if (traffic_status == 0) {
                trafficLight2[2].status = false; // blue = false;
                trafficLight2[1].status = true; // yellow = true;
            }

            if (traffic_status == 1) {
                trafficLight1[2].status = false; // blue = false;
                trafficLight1[1].status = true; // yellow = true;
            }

            SetTimer(hWnd, 3, 1000, NULL); // Yello time
            KillTimer(hWnd, 2); // Ÿ�̸� ����
            break;
        case 3: //yello
            if (traffic_status == 0 && trafficLight2[1].status) {
                trafficLight2[1].status = false; // yellow = false;
                trafficLight2[0].status = true; // red = true;
                trafficLight1[0].status = false;
                trafficLight1[2].status = true;
                traffic_status = 1;
            }

            if (traffic_status == 1 && trafficLight1[1].status) {
                trafficLight1[1].status = false; // yellow = false;
                trafficLight1[0].status = true; // red = true;
                trafficLight2[0].status = false;
                trafficLight2[2].status = true;
                traffic_status = 0;
            }
            KillTimer(hWnd, 3); // Ÿ�̸� ����
            SetTimer(hWnd, 2, ran_blue_time, NULL); // Blue
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

bool status1 = false;
bool status2 = false;
bool status3 = false;
bool status4 = false;

void car_move(HDC& mDC, RECT& rect, carS& car, carS& over_cars, vector<TrafficLight> trafficLight2, vector<TrafficLight> trafficLight1) {
    switch (car.line_status)
    {
    case 0: {
 
        if (car.rect.right <= rect.right / 2 - 100 && trafficLight2[0].status && status1 == false) {
            status1 = true;
            break;
        }

        if (status1) { // �ǳʱ����� �������̸� Ⱦ�ܺ������� ������.
            if (car.rect.right <= rect.right / 2 - 100 - 80) {
                car.rect.left += car.speed;
                car.rect.right += car.speed;
            }
            else {
                if (trafficLight2[0].status)
                    status1 = true;
                if (!trafficLight2[0].status)
                    status1 = false;
            }
        }
        else {
            car.rect.left += car.speed;   // ������ �ӵ��� �ش��ϴ� ���� ��ǥ�� ����
            car.rect.right += car.speed;  // ������ �ӵ��� �ش��ϴ� ���� ��ǥ�� ����

            if (car.rect.right >= rect.right) {
                if (car.rect.left > rect.right) {
                    car.rect.left = 0;
                    car.rect.right = 150;
                }
                over_cars.rect.left = 0;
                over_cars.rect.top = car.rect.top;
                over_cars.rect.right = car.rect.right - rect.right;
                over_cars.rect.bottom = car.rect.bottom;
                over_cars.color = car.color;
                over_cars.line_status = car.line_status;
            }
        }
       

        break;
    }
    case 1:
    {

        if (car.rect.left >= rect.right / 2 + 100 && trafficLight2[0].status && status2 == false) {
            status2 = true;
            break;
        }

        if (status2) {
            if (car.rect.left >= rect.right / 2 + 100 + 80) {
                car.rect.left -= car.speed;
                car.rect.right -= car.speed;
            }
            else {
                if (trafficLight2[0].status)
                    status2 = true;
                if (!trafficLight2[0].status)
                    status2 = false;
            }
        }
        else {

            car.rect.left -= car.speed;   // ������ �ӵ��� �ش��ϴ� ���� ��ǥ�� ����
            car.rect.right -= car.speed;  // ������ �ӵ��� �ش��ϴ� ���� ��ǥ�� ����

            if (car.rect.left <= 0) {
                if (car.rect.right < 0) {
                    car.rect.left = rect.right - 150;
                    car.rect.right = rect.right;
                }
                over_cars.rect.left = rect.right + car.rect.left;
                over_cars.rect.top = car.rect.top;
                over_cars.rect.right = rect.right;
                over_cars.rect.bottom = car.rect.bottom;
                over_cars.color = car.color;
                over_cars.line_status = car.line_status;
            }
        }

        break;
    }
    case 2:

        if (car.rect.bottom <= rect.bottom / 2 - 100 && trafficLight1[0].status && status3 == false) {
            status3 = true;
            break;
        }
        if (status3) {
            if (car.rect.bottom <= rect.bottom / 2 - 100 - 78) {
                car.rect.top += car.speed;
                car.rect.bottom += car.speed;
            }
            else {
                if (trafficLight1[0].status)
                    status3 = true;
                if (!trafficLight1[0].status)
                    status3 = false;
            }
        }
        else {
            car.rect.top += car.speed;
            car.rect.bottom += car.speed;
            if (car.rect.bottom >= rect.bottom) {
                if (car.rect.top > rect.bottom) {
                    car.rect.top = 0;
                    car.rect.bottom = 150;
                }
                over_cars.rect.left = car.rect.left;
                over_cars.rect.top = 0;
                over_cars.rect.right = car.rect.right;
                over_cars.rect.bottom = car.rect.bottom - rect.bottom;
                over_cars.color = car.color;
                over_cars.line_status = car.line_status;
            }
        }

        break;
    case 3:
    {
        if (car.rect.top >= rect.bottom / 2 + 100 && trafficLight1[0].status && status4 == false) {
            status4 = true;
            break;
        }
        if (status4) {
            if (car.rect.top >= rect.bottom / 2 + 100 + 78) {
                car.rect.top -= car.speed;
                car.rect.bottom -= car.speed;
            }
            else {
                if (trafficLight1[0].status)
                    status4 = true;
                if (!trafficLight1[0].status)
                    status4 = false;
            }
        }
        else {
            car.rect.top -= car.speed;
            car.rect.bottom -= car.speed;
            if (car.rect.top <= 0) {
                if (car.rect.bottom < 0) {
                    car.rect.top = rect.bottom - 150;
                    car.rect.bottom = rect.bottom;
                }
                over_cars.rect.left = car.rect.left;
                over_cars.rect.top = rect.bottom + car.rect.top;
                over_cars.rect.right = car.rect.right;
                over_cars.rect.bottom = rect.bottom;
                over_cars.color = car.color;
                over_cars.line_status = car.line_status;
            }
        }
        
    }
        break;
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
        car.rect.left += rect.right;
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
        car.rect.bottom += rect.bottom - 150;
        car.rect.left += rect.right / 2 + 23;
        car.rect.right += rect.right / 2 + 23;
        break;
    default:
        break;
    }
    DeleteObject(hBrush);
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
    DeleteObject(hBrush);

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

    DeleteObject(mPen);
    DeleteObject(oldPen);
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
    DeleteObject(hBrush);
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
    DeleteObject(oldBrush);
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
    DeleteObject(oldBrush);
}

void print_traffic_light1(HDC& mDC, RECT& rect, vector<TrafficLight> trafficLight1) {

    if (trafficLight1.size() <= 0) {
        return;
    }

    // ��ȣ�� ��ġ ����
    int lightCenterX = rect.right / 2 - 200; // ��ȣ�� �߽� x ��ǥ
    int lightCenterY = rect.bottom / 2 - 150; // ��ȣ�� �߽� y ��ǥ


    int lightLength = 80; // ��ȣ�� ��ü ����
    int lightWidth = 30; // ��ȣ�� �ʺ�

    // ��ȣ�� ��� �׸���
    HBRUSH hGreyBrush = CreateSolidBrush(RGB(50, 50, 50));
    RECT poleRect = { lightCenterX, lightCenterY + 15, lightCenterX + lightLength, lightCenterY + 35 };
    RECT poleRect2 = { lightCenterX + 320, lightCenterY + 265, lightCenterX + 320 + lightLength, lightCenterY + 285 };
    FillRect(mDC, &poleRect, hGreyBrush);
    FillRect(mDC, &poleRect2, hGreyBrush);

    // ��ȣ�� �Һ� �׸���
    int radius = 10; // �Һ��� ������

    for (int i = 0; i < trafficLight1.size(); ++i) {
        TrafficLight& tr = trafficLight1[i];
        switch (i)
        {
        case 0: //red
            if (tr.status) {
                tr.color = RGB(255, 0, 0);
            }
            else {
                tr.color = RGB(125, 0, 0);
            }
            tr.rect = { lightCenterX,(lightCenterY - radius) + 25 ,lightCenterX + 2 * radius ,(lightCenterY + radius) + 25 };
            break;
        case 1: //yellow
            if (tr.status) {
                tr.color = RGB(255, 255, 0);
            }
            else {
                tr.color = RGB(125, 125, 0);
            }
            tr.rect = { lightCenterX + 30, (lightCenterY - radius) + 25, lightCenterX + 2 * radius + 30, (lightCenterY + radius) + 25 };
            break;
        case 2: //green
            if (tr.status) {
                tr.color = RGB(0, 255, 0);
            }
            else {

                tr.color = RGB(0, 125, 0);
            }
            tr.rect = { lightCenterX + 60, (lightCenterY - radius) + 25, lightCenterX + 2 * radius + 60, (lightCenterY + radius) + 25 };
            break;
        default:
            break;
        }
    }

    HBRUSH hRedLightBrush = CreateSolidBrush(trafficLight1[0].color);
    HBRUSH hYellowLightBrush = CreateSolidBrush(trafficLight1[1].color);
    HBRUSH hGreenLightBrush = CreateSolidBrush(trafficLight1[2].color);

    // ������
    HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hRedLightBrush);
    Ellipse(mDC, trafficLight1[0].rect.left, trafficLight1[0].rect.top, trafficLight1[0].rect.right, trafficLight1[0].rect.bottom);
    Ellipse(mDC, trafficLight1[2].rect.left + 320, trafficLight1[2].rect.top + 250, trafficLight1[2].rect.right + 320, trafficLight1[2].rect.bottom + 250);

    // �����
    SelectObject(mDC, hYellowLightBrush);
    Ellipse(mDC, trafficLight1[1].rect.left, trafficLight1[1].rect.top, trafficLight1[1].rect.right, trafficLight1[1].rect.bottom);
    Ellipse(mDC, trafficLight1[1].rect.left + 320, trafficLight1[1].rect.top + 250, trafficLight1[1].rect.right + 320, trafficLight1[1].rect.bottom + 250);

    // �ʷϺ�
    SelectObject(mDC, hGreenLightBrush);
    Ellipse(mDC, trafficLight1[2].rect.left, trafficLight1[2].rect.top, trafficLight1[2].rect.right, trafficLight1[2].rect.bottom);
    Ellipse(mDC, trafficLight1[0].rect.left + 320, trafficLight1[0].rect.top + 250, trafficLight1[0].rect.right + 320, trafficLight1[0].rect.bottom + 250);

    // �귯�� ��ȯ �� ����
    SelectObject(mDC, oldBrush);
    DeleteObject(hGreyBrush);
    DeleteObject(hRedLightBrush);
    DeleteObject(hYellowLightBrush);
    DeleteObject(hGreenLightBrush);
    DeleteObject(oldBrush);
}


void print_traffic_light3(HDC& mDC, RECT& rect, vector<TrafficLight> trafficLight2) {

    if (trafficLight2.size() <= 0) {
        return;
    }
    // ��ȣ�� ��ġ ����
    int lightCenterX = rect.right / 2 + 130; // ��ȣ�� �߽� x ��ǥ
    int lightCenterY = rect.bottom / 2 - 200; // ��ȣ�� �߽� y ��ǥ

    int lightWidth = 30; // ��ȣ�� �ʺ�
    int lightHeight = 80; // ��ȣ�� ����

    // ��ȣ�� ��� �׸���
    HBRUSH hGreyBrush = CreateSolidBrush(RGB(50, 50, 50));
    RECT poleRect = { lightCenterX - 10, lightCenterY, lightCenterX + 10, lightCenterY + lightHeight };
    RECT poleRect2 = { lightCenterX - 10 - 260, lightCenterY + 120 + 200, lightCenterX + 10 - 260, lightCenterY + lightHeight + 120 + 200 };

    FillRect(mDC, &poleRect, hGreyBrush);
    FillRect(mDC, &poleRect2, hGreyBrush);

    // ��ȣ�� �Һ� �׸���
    int radius = 10; // �Һ��� ������

    for (int i = 0; i < trafficLight2.size(); ++i) {
        TrafficLight& tr = trafficLight2[i];
        switch (i)
        {
        case 0: //red
            if (tr.status) {
                tr.color = RGB(255, 0, 0);
            }
            else {
                tr.color = RGB(125, 0, 0);
            }
            tr.rect = { lightCenterX - radius, lightCenterY, lightCenterX + radius, lightCenterY + 2 * radius };
            break;
        case 1: //yellow
            if (tr.status) {
                tr.color = RGB(255, 255, 0);
            }
            else {
                tr.color = RGB(125, 125, 0);
            }
            tr.rect = { lightCenterX - radius, lightCenterY + 30, lightCenterX + radius, lightCenterY + 2 * radius + 30 };
            break;
        case 2: //green
            if (tr.status) {
                tr.color = RGB(0, 255, 0);
            }
            else {

                tr.color = RGB(0, 125, 0);
            }
            tr.rect = { lightCenterX - radius, lightCenterY + 60, lightCenterX + radius, lightCenterY + 2 * radius + 60 };
            break;
        default:
            break;
        }
    }

    HBRUSH hRedLightBrush = CreateSolidBrush(trafficLight2[0].color);
    HBRUSH hYellowLightBrush = CreateSolidBrush(trafficLight2[1].color);
    HBRUSH hGreenLightBrush = CreateSolidBrush(trafficLight2[2].color);

    // ������
    HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hRedLightBrush);
    Ellipse(mDC, trafficLight2[0].rect.left, trafficLight2[0].rect.top, trafficLight2[0].rect.right, trafficLight2[0].rect.bottom);
    Ellipse(mDC, trafficLight2[2].rect.left - 260, trafficLight2[2].rect.top + 320, trafficLight2[2].rect.right - 260, trafficLight2[2].rect.bottom + 320);

    // �����
    SelectObject(mDC, hYellowLightBrush);
    Ellipse(mDC, trafficLight2[1].rect.left, trafficLight2[1].rect.top, trafficLight2[1].rect.right, trafficLight2[1].rect.bottom);
    Ellipse(mDC, trafficLight2[1].rect.left - 260, trafficLight2[1].rect.top + 320, trafficLight2[1].rect.right - 260, trafficLight2[1].rect.bottom + 320);

    // �ʷϺ�
    SelectObject(mDC, hGreenLightBrush);
    Ellipse(mDC, trafficLight2[2].rect.left, trafficLight2[2].rect.top, trafficLight2[2].rect.right, trafficLight2[2].rect.bottom);
    Ellipse(mDC, trafficLight2[0].rect.left - 260, trafficLight2[0].rect.top + 320, trafficLight2[0].rect.right - 260, trafficLight2[0].rect.bottom + 320);

    // �귯�� ��ȯ �� ����
    SelectObject(mDC, oldBrush);
    DeleteObject(hGreyBrush);
    DeleteObject(hRedLightBrush);
    DeleteObject(hYellowLightBrush);
    DeleteObject(hGreenLightBrush);
}
