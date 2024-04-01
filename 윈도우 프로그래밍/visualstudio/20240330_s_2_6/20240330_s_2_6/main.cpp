#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 2-6번";
LPCTSTR lpszWindowName = L"실습 2-6번";

#define WIDTH 800
#define HEIGHT 600

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

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid(2, 10);
uniform_int_distribution<int> char_uid(65, 122);
uniform_int_distribution<int> choose_uid(0, 5);
uniform_int_distribution<int> rgb_uid(0, 255);

void line_print(int x, int y, HDC hDC, RECT& rect, HWND& hWnd);
void print_shape(int& x, int& y, HDC& hDC, RECT& rect, HWND& hWnd, TCHAR shape[6]);
void printX(TCHAR c, int x, int y, HWND& hWnd, RECT& rect, HDC& hDC, int i, int j);
void printF(TCHAR c, int x, int y, HWND& hWnd, RECT& rect, HDC& hDC, int i, int j);
void printDia(TCHAR c, int x, int y, HWND& hWnd, RECT& rect, HDC& hDC, int i, int j);
void printNaVi(TCHAR c, int x, int y, HWND& hWnd, RECT& rect, HDC& hDC, int i, int j);
void printC2(TCHAR c, int x, int y, HWND& hWnd, RECT& rect, HDC& hDC, int i, int j);
void printR3(TCHAR c, int x, int y, HWND& hWnd, RECT& rect, HDC& hDC, int i, int j);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC;
    static TCHAR lpOut[100];
    static RECT rect;
    static SIZE size;
    static int lineNumX{};
    static int lineNumY{};
    static TCHAR shape[6];
    switch (uMsg)
    {
    case WM_CREATE:
        lineNumX = uid(gen);
        lineNumY = uid(gen);

        for (size_t i = 0; i < 6; ++i) {
            shape[i] = char_uid(gen);
        }

        break;
    case WM_CHAR:
        switch (wParam)
        {
        case VK_BACK:
            break;
        case VK_RETURN:
            break;
        default:
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);

        line_print(lineNumX, lineNumY, hDC, rect, hWnd);

        print_shape(lineNumX, lineNumY, hDC, rect, hWnd, shape);

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

void print_shape(int& x, int& y, HDC& hDC, RECT& rect, HWND& hWnd, TCHAR shape[6]) {

    int screenWidth, screenHeight;
    GetClientRect(hWnd, &rect);
    screenWidth = rect.right - rect.left;
    screenHeight = rect.bottom - rect.top;
    int x_n{};
    x_n = screenWidth / x;
    int y_n{};
    y_n = screenHeight / y;
    static RECT rect2;

    for (int i = 0; i < y; ++i) {
        for (int j = 0; j < x; ++j) {
            switch (choose_uid(gen))
            {
            case 0:
                printX(shape[choose_uid(gen)], x_n, y_n, hWnd, rect2, hDC, i, j);
                break;
            case 1:
                printF(shape[choose_uid(gen)], x_n, y_n, hWnd, rect2, hDC, i, j);
                break;
            case 2:
                printDia(shape[choose_uid(gen)], x_n, y_n, hWnd, rect2, hDC, i, j);
                break;
            case 3:
                printNaVi(shape[choose_uid(gen)], x_n, y_n, hWnd, rect2, hDC, i, j);
                break;
            case 4:
                printC2(shape[choose_uid(gen)], x_n, y_n, hWnd, rect2, hDC, i, j);
                break;
            case 5:
                printR3(shape[choose_uid(gen)], x_n, y_n, hWnd, rect2, hDC, i, j);
                break;
            default:
                break;
            }
        }
    }

};


void printF(TCHAR c, int x, int y, HWND& hWnd, RECT& rect, HDC& hDC, int i, int j) {

    static SIZE size;

    int screenWidth, screenHeight;
    GetClientRect(hWnd, &rect);
    screenWidth = rect.right - rect.left;
    screenHeight = rect.bottom - rect.top;

    TCHAR arr[5][6] = {
      {c, c, c, c, c},
      {c},
      {c, c, c, c, c},
      {c},
      {c, c, c, c, c}
    };

    SetBkColor(hDC, RGB(rgb_uid(gen), rgb_uid(gen), rgb_uid(gen)));
    GetTextExtentPoint32(hDC, arr[0], lstrlen(arr[0]), &size);

    if (y < 85) {
        rect.left = j * x;
        rect.top = y * i;
        rect.right = x * (j + 1);
        rect.bottom = y * (i + 1);
        DrawText(hDC, L"ㄹ", 1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }
    else {
        for (size_t k = 0; k < 5; ++k) {
            if (k == 1) {
                rect.left = j * x + (size.cy * 2) + 10;
                rect.top = (i * y) + (k * size.cy) - 65;
                rect.right = x * (j + 1);
                rect.bottom = (y * (i + 1)) + ((k + 1) * size.cy);

            }
            else if (k == 3) {
                rect.left = j * x + 10;
                rect.top = (i * y) + (k * size.cy) - 65;
                rect.right = x * (j + 1) - (size.cy * 2);
                rect.bottom = (y * (i + 1)) + ((k + 1) * size.cy);
            }
            else {
                rect.left = j * x + 10;
                rect.top = (i * y) + (k * size.cy) - 65;
                rect.right = x * (j + 1);
                rect.bottom = (y * (i + 1)) + ((k + 1) * size.cy);
            }

            DrawText(hDC, arr[k], lstrlen(arr[k]), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
    }

}

void printX(TCHAR c, int x, int y, HWND& hWnd, RECT& rect, HDC& hDC, int i, int j) {

    static SIZE size;
    static TCHAR arr[5][6]; // 2차원 배열 선언

    // 배열 초기화
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            arr[i][j] = ' ';
        }
    }

    // X자 모양으로 o 할당
    for (int i = 0; i < 5; ++i) {
        arr[i][i] = c; // 왼쪽 위에서 오른쪽 아래로
        arr[i][5 - i - 1] = c; // 오른쪽 위에서 왼쪽 아래로
    }

    SetBkColor(hDC, RGB(rgb_uid(gen), rgb_uid(gen), rgb_uid(gen)));

    GetTextExtentPoint32(hDC, arr[0], lstrlen(arr[0]), &size);

    if (y < 85) {
        rect.left = j * x;
        rect.top = y * i;
        rect.right = x * (j + 1);
        rect.bottom = y * (i + 1);
        DrawText(hDC, L"X", 1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }
    else {
        for (size_t k = 0; k < 5; ++k) {
            rect.left = (j * x) + 10;
            rect.top = (k * size.cy) + (y * i) - 65;
            rect.right = x * (j + 1);
            rect.bottom = ((k + 1) * size.cy) + (y * (i + 1));
            DrawText(hDC, arr[k], lstrlen(arr[k]), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
    }

}

void printDia(TCHAR c, int x, int y, HWND& hWnd, RECT& rect, HDC& hDC, int i, int j) {
    static SIZE size;

    int screenWidth, screenHeight;
    GetClientRect(hWnd, &rect);
    screenWidth = rect.right - rect.left;
    screenHeight = rect.bottom - rect.top;

    static TCHAR arr[5][6]; // 2차원 배열 선언

    // 배열 초기화
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            arr[i][j] = ' ';
        }
    }

    // 마름모 모양 그리기
    int mid = 5 / 2;
    for (int i = 0; i <= mid; ++i) {
        for (int j = mid - i; j <= mid + i; ++j) {
            arr[i][j] = 'o';
            arr[5 - 1 - i][j] = 'o';
        }
    }

    SetBkColor(hDC, RGB(rgb_uid(gen), rgb_uid(gen), rgb_uid(gen)));

    GetTextExtentPoint32(hDC, arr[0], lstrlen(arr[0]), &size);

    if (y < 85) {
        rect.left = j * x + 10;
        rect.top = y * i;
        rect.right = x * (j + 1);
        rect.bottom = y * (i + 1);
        DrawText(hDC, L"◇", 1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }
    else {
        for (size_t k = 0; k < 5; ++k) {
            rect.left = (j * x) + 10;
            rect.top = (k * size.cy) + (y * i) - 65;
            rect.right = x * (j + 1);
            rect.bottom = ((k + 1) * size.cy) + (y * (i + 1));
            DrawText(hDC, arr[k], lstrlen(arr[k]), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
    }
}

void printNaVi(TCHAR c, int x, int y, HWND& hWnd, RECT& rect, HDC& hDC, int i, int j) {
    static SIZE size;

    int screenWidth, screenHeight;
    GetClientRect(hWnd, &rect);
    screenWidth = rect.right - rect.left;
    screenHeight = rect.bottom - rect.top;

    TCHAR arr[5][11] = {
        {c,' ',' ',' ',' ',' ',' ',' ',c},
        {c,c,' ',' ',' ',c,c},
        {c,c,c,c,c,c},
        {c,c,' ',' ',' ',c,c},
        {c,' ',' ',' ',' ',' ',' ',' ',c}
    };


    SetBkColor(hDC, RGB(rgb_uid(gen), rgb_uid(gen), rgb_uid(gen)));

    GetTextExtentPoint32(hDC, arr[0], lstrlen(arr[0]), &size);

    if (y < 85) {
        rect.left = j * x + 10;
        rect.top = y * i;
        rect.right = x * (j + 1);
        rect.bottom = y * (i + 1);
        DrawText(hDC, L"▷◁", 2, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }
    else {
        for (size_t k = 0; k < 5; ++k) {
            rect.left = (j * x) + 10;
            rect.top = (k * size.cy) + (y * i) - 65;
            rect.right = x * (j + 1);
            rect.bottom = ((k + 1) * size.cy) + (y * (i + 1));
            DrawText(hDC, arr[k], lstrlen(arr[k]), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
    }

}

void printC2(TCHAR c, int x, int y, HWND& hWnd, RECT& rect, HDC& hDC, int i, int j) {
    static SIZE size;

    int screenWidth, screenHeight;
    GetClientRect(hWnd, &rect);
    screenWidth = rect.right - rect.left;
    screenHeight = rect.bottom - rect.top;

    static TCHAR arr[5][6]; // 2차원 배열 선언

    // 초기화
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            arr[i][j] = ' ';
        }
    }

    int width = 5; // 삼각형의 너비
    int height = 5; // 삼각형의 높이

    // 삼각형 채우기
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (j >= (width - 1) / 2 - i && j <= (width - 1) / 2 + i) {
                arr[i][j] = c;
            }
        }
    }


    SetBkColor(hDC, RGB(rgb_uid(gen), rgb_uid(gen), rgb_uid(gen)));

    GetTextExtentPoint32(hDC, arr[0], lstrlen(arr[0]), &size);

    if (y < 110) {
        rect.left = j * x + 10;
        rect.top = y * i;
        rect.right = x * (j + 1);
        rect.bottom = y * (i + 1);
        DrawText(hDC, L"△", 1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }
    else {
        for (size_t k = 0; k < 3; ++k) {
            rect.left = (j * x) + 10;
            rect.top = (k * size.cy) + (y * i) - 65;
            rect.right = x * (j + 1);
            rect.bottom = ((k + 1) * size.cy) + (y * (i + 1));
            DrawText(hDC, arr[k], lstrlen(arr[k]), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
    }

    if (y < 110) {
        GetTextExtentPoint32(hDC, L"△", 1, &size);

        rect.left = j * x + 10;
        rect.top = y * i + size.cy;
        rect.right = x * (j + 1);
        rect.bottom = y * (i + 1) + size.cy;
        DrawText(hDC, L"△", 1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }
    else {
        for (size_t k = 0; k < 3; ++k) {
            rect.left = (j * x) + 10;
            rect.top = ((k * size.cy) + (y * i) + 28);
            rect.right = x * (j + 1);
            rect.bottom = ((k + 1) * size.cy) + (y * (i + 1));
            DrawText(hDC, arr[k], lstrlen(arr[k]), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
    }

}


void printR3(TCHAR c, int x, int y, HWND& hWnd, RECT& rect, HDC& hDC, int i, int j) {
    static SIZE size;

    int screenWidth, screenHeight;
    GetClientRect(hWnd, &rect);
    screenWidth = rect.right - rect.left;
    screenHeight = rect.bottom - rect.top;

    static TCHAR arr[5][3]; // 2차원 배열 선언

    // 초기화
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 2; ++j) {
            arr[i][j] = c;
        }
    }


    SetBkColor(hDC, RGB(rgb_uid(gen), rgb_uid(gen), rgb_uid(gen)));

    GetTextExtentPoint32(hDC, arr[0], lstrlen(arr[0]), &size);

    if (y < 85) {
        rect.left = j * x + 10;
        rect.top = y * i;
        rect.right = x * (j + 1);
        rect.bottom = y * (i + 1);
        DrawText(hDC, L"▪ ▪ ▪", 5, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }
    else {
        for (size_t k = 0; k < 3; ++k) {
            rect.left = (j * x) - 30;
            rect.top = (k * size.cy) + (y * i) - 65;
            rect.right = x * (j + 1) - 30;
            rect.bottom = ((k + 1) * size.cy) + (y * (i + 1));
            DrawText(hDC, arr[k], lstrlen(arr[k]), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
    }


    if (y < 85) {

    }
    else {
        for (size_t k = 0; k < 3; ++k) {
            rect.left = (j * x) + 10;
            rect.top = (k * size.cy) + (y * i) - 65;
            rect.right = x * (j + 1) + 10;
            rect.bottom = ((k + 1) * size.cy) + (y * (i + 1));
            DrawText(hDC, arr[k], lstrlen(arr[k]), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
    }


    if (y < 85) {

    }
    else {
        for (size_t k = 0; k < 3; ++k) {
            rect.left = (j * x) + 40;
            rect.top = (k * size.cy) + (y * i) - 65;
            rect.right = x * (j + 1) + 40;
            rect.bottom = ((k + 1) * size.cy) + (y * (i + 1));
            DrawText(hDC, arr[k], lstrlen(arr[k]), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
    }

}


void line_print(int x, int y, HDC hDC, RECT& rect, HWND& hWnd) { //문자로 바둑판 그리기

    static SIZE size;
    vector<TCHAR> width_l;

    int screenWidth, screenHeight;

    GetClientRect(hWnd, &rect);
    screenWidth = rect.right - rect.left;
    screenHeight = rect.bottom - rect.top;


    GetTextExtentPoint32(hDC, TEXT("-"), 1, &size);

    int dashWidth = size.cx;

    // 랜덤한 글자의 길이를 화면의 너비로 설정
    int widthLength = screenWidth / dashWidth;

    for (size_t i = 0; i < widthLength; ++i) {
        width_l.push_back('-');
    }

    int lineNumY = screenHeight / y;

    for (size_t i = 1; i < y; ++i) {
        TextOut(hDC, 0, lineNumY * i, width_l.data(), width_l.size());
    }

    //-----------------------------------------------------------------------

    GetTextExtentPoint32(hDC, TEXT("│"), 1, &size);

    int dashHeight = size.cy;
    int heightLength = screenHeight / dashHeight;

    int lineNumX = screenWidth / x;

    for (size_t i = 0; i < lineNumX; ++i) {
        for (size_t j = 0; j < heightLength; ++j) {
            if (i == 0)
                break;
            TextOut(hDC, lineNumX * i, j * dashHeight, _T("│"), 1);
        }
    }

    for (size_t i = 1; i < y; ++i) {
        for (size_t j = 1; j < x; ++j) {
            TextOut(hDC, lineNumX * j, (lineNumY * i) + 1, _T("┼"), 1);
        }
    }
}