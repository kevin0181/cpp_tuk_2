#include <windows.h>
#include <tchar.h>
#include <ctime> // srand를 위한 헤더
#include <cstdlib> // rand를 위한 헤더

#define WINDOW_WIDTH 3000
#define WINDOW_HEIGHT 600
#define BRICK_ROWS 3
#define BRICK_COLUMNS 10
#define BRICK_WIDTH 60
#define BRICK_HEIGHT 30

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


HINSTANCE hInst;
HWND hWndMain;
RECT brickRect[3][10];
HDC hdc;
PAINTSTRUCT ps;
static RECT clientRect;

struct Ball {
    int x; // x 좌표
    int y; // y 좌표
    int radius; // 반지름
};

Ball ball;

int r = rand() % 3;
int t1 = 30;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    WNDCLASSEX WndClass;
    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = (WNDPROC)WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_HAND);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = _T("Window Class Name");
    WndClass.hIconSm = LoadIcon(NULL, IDI_QUESTION);
    RegisterClassEx(&WndClass);

    HWND hWnd = CreateWindow(_T("Window Class Name"), _T("벽돌깨기"), WS_OVERLAPPEDWINDOW, 100, 50, 1000, 800, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return 0;
    }
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG Message;
    while (GetMessage(&Message, NULL, 0, 0))
    {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return (int)Message.wParam;
}
static bool ballMove = false;

void InitializeBall()
{
    ball.x = clientRect.right / 2; // 화면 가로 중앙
    ball.y = 20; // 상단에 위치
    ball.radius = 10; // 공 반지름
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {



    int r = 0;

    switch (message) {
    case WM_CREATE:
        GetClientRect(hWnd, &clientRect);


        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 10; ++j) {
                brickRect[i][j] = { j * BRICK_WIDTH + 200, i * BRICK_HEIGHT + 400, (j + 1) * BRICK_WIDTH + 200, (i + 1) * BRICK_HEIGHT + 400 };
            }
        }


        InitializeBall();
        break;


    case WM_KEYDOWN:
        switch (wParam)
        {
        case 's':
        case 'S':
            SetTimer(hWnd, 3, t1, NULL);
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case 'p':
        case 'P':
            if (ballMove == false)
            {
                KillTimer(hWnd, 3);
                KillTimer(hWnd, 4);
                ballMove = true;
            }
            else if (ballMove == true)
            {
                SetTimer(hWnd, 3, t1, NULL);
                InvalidateRect(hWnd, NULL, TRUE);
                ballMove = false;
                break;
            }
            break;

        case 'n':
        case 'N':
        {
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 10; ++j) {
                    brickRect[i][j] = { j * BRICK_WIDTH + 200, i * BRICK_HEIGHT + 400, (j + 1) * BRICK_WIDTH + 200, (i + 1) * BRICK_HEIGHT + 400 };
                }
            }
            KillTimer(hWnd, 1);
            KillTimer(hWnd, 2);
            KillTimer(hWnd, 3);
            KillTimer(hWnd, 4);
            InitializeBall();
            InvalidateRect(hWnd, NULL, TRUE);

        }
        break;

        case 'q':
        case 'Q':
            PostQuitMessage(0);
            break;

        case VK_OEM_PLUS:
            if (t1 <= 0)
            {
                break;
            }
            t1 -= 2;
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case VK_OEM_MINUS:
            t1 += 5;
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        }



    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        Ellipse(hdc, ball.x - ball.radius, ball.y - ball.radius,
            ball.x + ball.radius, ball.y + ball.radius);


        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 10; ++j) {
                if ((ball.x + ball.radius >= brickRect[i][j].left &&
                    ball.x - ball.radius <= brickRect[i][j].right) && (ball.y + 10 == brickRect[i][j].top))
                {
                    KillTimer(hWnd, 3);
                    r = rand() % 3;
                    SetTimer(hWnd, 4, t1, NULL);
                }
                else if (ball.y - ball.radius == clientRect.top)
                {
                    KillTimer(hWnd, 4);
                    SetTimer(hWnd, 3, t1, NULL);
                }

                Rectangle(hdc, brickRect[i][j].left, brickRect[i][j].top,
                    brickRect[i][j].right, brickRect[i][j].bottom);

                if (ball.x + ball.radius >= brickRect[i][j].left &&
                    ball.x - ball.radius <= brickRect[i][j].right &&
                    ball.y + ball.radius >= brickRect[i][j].top &&
                    ball.y - ball.radius <= brickRect[i][j].bottom)
                {
                    brickRect[i][j].left = -1;
                    brickRect[i][j].top = -1;
                    brickRect[i][j].right = -1;
                    brickRect[i][j].bottom = -1;
                }
            }
        }
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_TIMER:
    {
        UINT_PTR timerID = wParam;

        switch (timerID)
        {
        case 1:
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 10; ++j)
                {
                    brickRect[i][j].left -= 2;
                    brickRect[i][j].right -= 2;
                }
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case 2:
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 10; ++j)
                {
                    brickRect[i][j].left += 2;
                    brickRect[i][j].right += 2;
                }
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case 3:
            if (r == 0)
            {
                ball.y += 10;
            }
            else if (r == 1)
            {
                ball.y += 20;
            }
            else if (r == 2)
            {
                ball.y += 1;
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case 4:
            if (r == 0)
            {
                ball.y -= 10;
            }
            else if (r == 1)
            {
                ball.y -= 20;
            }
            else if (r == 2)
            {
                ball.y -= 1;
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;

        case 5:
            r = rand() % 3;
            break;


        default:
            break;
        }


        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }

    case WM_LBUTTONDOWN:
        SetTimer(hWnd, 1, 10, NULL);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 10; ++j)
            {
                if ((ball.x + ball.radius >= brickRect[i][j].left &&
                    ball.x - ball.radius <= brickRect[i][j].right) && (ball.y + ball.radius == brickRect[i][j].top))
                {
                    KillTimer(hWnd, 3);
                    r = rand() % 3;
                    SetTimer(hWnd, 4, t1, NULL);
                }
            }
        }
        break;

    case WM_LBUTTONUP:
        KillTimer(hWnd, 1);
        break;

    case WM_RBUTTONDOWN:

        SetTimer(hWnd, 2, 10, NULL);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 10; ++j)
            {
                if ((ball.x + ball.radius >= brickRect[i][j].left &&
                    ball.x - ball.radius <= brickRect[i][j].right) && (ball.y - ball.radius == brickRect[i][j].bottom))
                {
                    KillTimer(hWnd, 4);
                    SetTimer(hWnd, 3, t1, NULL);
                }
            }
        }
        break;

    case WM_RBUTTONUP:

        KillTimer(hWnd, 2);
        break;

    case WM_DESTROY:
        KillTimer(hWnd, 1);
        KillTimer(hWnd, 2);
        KillTimer(hWnd, 3);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

