#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 3-2";
LPCTSTR lpszWindowName = L"실습 3-2";

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
struct Block {
    int stack{};
    int x;
    int y;
    RECT rect;
    COLORREF color = RGB(255, 0, 0);
    bool visible = true;  // 추가: 벽돌이 보이는지 여부
};

struct Circle {
    int x, y, radius;
};

struct Velocity {
    int dx;
    int dy;
};

bool CheckCollision(const Circle& ball, const RECT& block);

#define BLOCK_WIDTH 50  // 블록의 가로 크기
#define BLOCK_HEIGHT 30 // 블록의 세로 크기
#define BLOCK_COUNT 20  // 한 줄에 들어가는 블록 수
#define ROW_COUNT 3     // 줄 수

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uid_ran_3(0, 2);
uniform_int_distribution<int> uid_ran_4(0, 4);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static int Timer1Count = 0;
    static vector<Block> blocks;
    static int blockSpeed = 5;   // 벽돌의 움직임 속도
    static int blockDirection = 1; // 벽돌의 움직임 방향 (1: 오른쪽, -1: 왼쪽)
    static int speedY = -1;
    static int speedYr = -1;
    static Circle ball;
    static int speed1 = 0;
    static Velocity ballVelocity = { 5, -5 }; // 초기 속도 (예시 값)
    static bool puzz = false;
    switch (uMsg)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, 1, 50, NULL); // 50ms 마다 WM_TIMER 메시지 발생

        int startX = (WIDTH - (BLOCK_WIDTH * BLOCK_COUNT)) / 2; // 중앙 정렬을 위한 시작 X 좌표
        int startY = HEIGHT - (BLOCK_HEIGHT * ROW_COUNT) - 50;   // 하단 정렬을 위한 시작 Y 좌표 (50은 바닥 여백)

        blocks.reserve(ROW_COUNT * BLOCK_COUNT);
        for (int i = 0; i < ROW_COUNT; ++i) {
            for (int j = 0; j < BLOCK_COUNT; ++j) {
                Block b;
                b.x = startX + (j * BLOCK_WIDTH);
                b.y = startY + (i * BLOCK_HEIGHT);
                b.rect = { b.x, b.y, b.x + BLOCK_WIDTH, b.y + BLOCK_HEIGHT };
                blocks.push_back(b);
            }
        }

        // 공의 초기 위치를 윈도우의 상단 중앙에 설정합니다.
        ball.x = WIDTH / 2;
        ball.y = 30; // 상단에서 30픽셀 아래에 위치
        ball.radius = 20; // 반지름은 20픽셀로 설정

        break;
    }
    case WM_RBUTTONDOWN:
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때
        break;
    case WM_CHAR:
        switch (wParam)
        {
        case '-':
            if (speed1 < 0) {
                break;
            }
            else {
                speed1--;
            }
            break;
        case '=':
            speed1++;
            break;
        case 'n':
        {
            blocks.clear();
            int startX = (WIDTH - (BLOCK_WIDTH * BLOCK_COUNT)) / 2; // 중앙 정렬을 위한 시작 X 좌표
            int startY = HEIGHT - (BLOCK_HEIGHT * ROW_COUNT) - 50;   // 하단 정렬을 위한 시작 Y 좌표 (50은 바닥 여백)

            blocks.reserve(ROW_COUNT * BLOCK_COUNT);
            for (int i = 0; i < ROW_COUNT; ++i) {
                for (int j = 0; j < BLOCK_COUNT; ++j) {
                    Block b;
                    b.x = startX + (j * BLOCK_WIDTH);
                    b.y = startY + (i * BLOCK_HEIGHT);
                    b.rect = { b.x, b.y, b.x + BLOCK_WIDTH, b.y + BLOCK_HEIGHT };
                    blocks.push_back(b);
                }
            }

            // 공의 초기 위치를 윈도우의 상단 중앙에 설정합니다.
            ball.x = WIDTH / 2;
            ball.y = 30; // 상단에서 30픽셀 아래에 위치
            ball.radius = 20; // 반지름은 20픽셀로 설정
            break;
        }
        case 'p':
            puzz = !puzz;
            break;
        case 's':
            speedY = uid_ran_3(gen);
            SetTimer(hWnd, 2, 10, NULL);
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
        Rectangle(mDC, 0, 0, rect.right, rect.bottom);

        
        for (const auto& block : blocks) {
            // Draw blocks
            if (block.visible) {
                mBrush = CreateSolidBrush(block.color); // 빨간색 브러시 생성
                oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
                Rectangle(mDC, block.rect.left, block.rect.top, block.rect.right, block.rect.bottom);
                SelectObject(mDC, oldBrush); // 이전 브러시로 복원
                DeleteObject(mBrush); // 브러시 객체 삭제
            }
        }
        

        // 공 그리기
        mBrush = CreateSolidBrush(RGB(0, 0, 255)); // 파란색 브러시 생성
        oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
        Ellipse(mDC, ball.x - ball.radius, ball.y - ball.radius, ball.x + ball.radius, ball.y + ball.radius);
        SelectObject(mDC, oldBrush); // 이전 브러시로 복원
        DeleteObject(mBrush); // 브러시 객체 삭제

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);
        DeleteObject(hBitmap); // 생성한 비트맵 삭제
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        if (!puzz) {
            switch (wParam) {
            case 1:

                // 왼쪽이나 오른쪽 끝에 도달했는지 확인
                if (blocks.front().x <= 0 || blocks.back().x + BLOCK_WIDTH >= WIDTH) {
                    blockDirection *= -1; // 방향 반전
                }

                // 모든 벽돌의 위치 업데이트
                for (auto& block : blocks) {
                    block.x += blockSpeed * blockDirection;
                    block.rect = { block.x, block.y, block.x + BLOCK_WIDTH, block.y + BLOCK_HEIGHT };
                }

                break;
            case 2:
                if (speedY != -1) {
                    switch (speedY)
                    {
                    case 0:
                        ball.y = ball.y + 2 + speed1;
                        break;
                    case 1:
                        ball.y = ball.y + 3 + speed1;
                        break;
                    case 2:
                        ball.y = ball.y + 5 + speed1;
                        break;
                    default:
                        break;
                    }
                }

                if (speedYr != -1) {
                    switch (speedYr)
                    {
                    case 0: //직선으로 올라가기
                        ball.y = ball.y - 2 - speed1;
                        break;
                    case 1: // 45도로 왼쪽~ 가기
                        ball.x = ball.x - 3 - speed1;
                        ball.y = ball.y - 3 - speed1;
                        break;
                    case 2: //45도로 오른쪽~가기
                        ball.x = ball.x + 3 + speed1;
                        ball.y = ball.y - 3 - speed1;
                        break;
                    case 3: // ->
                        ball.x = ball.x + 6 + speed1;
                        ball.y = ball.y - 2 - speed1;
                        break;
                    case 4: // <-
                        ball.x = ball.x - 6 - speed1;
                        ball.y = ball.y - 2 - speed1;
                        break;
                    default:
                        break;
                    }
                }

                // left wall
                if ((ball.x - ball.radius) <= 0) {
                    ball.x = ball.radius;
                    if (speedYr == 1) {
                        speedYr = 2;
                    }
                    else if (speedYr == 4) {
                        speedYr = 3;
                    }
                }

                // 공이 오른쪽 벽에 닿았을 때
                if ((ball.x + ball.radius) >= WIDTH) {
                    ball.x = WIDTH - ball.radius;  // 공이 화면 밖으로 나가지 않도록 위치 조정
                    if (speedYr == 2) {
                        speedYr = 1;
                    }
                    else if (speedYr == 3) {
                        speedYr = 4;
                    }
                }

                //공이 천장에 닿았을때
                if ((ball.y - ball.radius) <= 0) {
                    ball.y = ball.radius; // 볼이 화면 밖으로 나가지 않도록 설정
                    speedYr = -1;
                    speedY = uid_ran_3(gen);
                }

                for (auto& block : blocks) {
                    if (CheckCollision(ball, block.rect) && block.visible) { //충돌!
                        speedY = -1;
                        speedYr = uid_ran_4(gen);
                        block.stack += 1;
                        block.color = RGB(0, 0, 0);
                        if (block.stack == 2) { // 두번 부딪히면
                            block.visible = false;
                        }
                        break; // 다른 블록과의 충돌 검사를 중단
                    }
                }

                if (ball.y >= HEIGHT) {
                    blocks.clear();

                    int startX = (WIDTH - (BLOCK_WIDTH * BLOCK_COUNT)) / 2; // 중앙 정렬을 위한 시작 X 좌표
                    int startY = HEIGHT - (BLOCK_HEIGHT * ROW_COUNT) - 50;   // 하단 정렬을 위한 시작 Y 좌표 (50은 바닥 여백)

                    blocks.reserve(ROW_COUNT * BLOCK_COUNT);
                    for (int i = 0; i < ROW_COUNT; ++i) {
                        for (int j = 0; j < BLOCK_COUNT; ++j) {
                            Block b;
                            b.x = startX + (j * BLOCK_WIDTH);
                            b.y = startY + (i * BLOCK_HEIGHT);
                            b.rect = { b.x, b.y, b.x + BLOCK_WIDTH, b.y + BLOCK_HEIGHT };
                            blocks.push_back(b);
                        }
                    }

                    // 공의 초기 위치를 윈도우의 상단 중앙에 설정합니다.
                    ball.x = WIDTH / 2;
                    ball.y = 30; // 상단에서 30픽셀 아래에 위치
                    ball.radius = 20; // 반지름은 20픽셀로 설정
                    break;
                }

                break;
            default:
                break;
            }
        }
       
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1); // 타이머 종료
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool CheckCollision(const Circle& ball, const RECT& block) {
    int nearestX = max(block.left, min(ball.x, block.right));
    int nearestY = max(block.top, min(ball.y, block.bottom));

    int deltaX = ball.x - nearestX;
    int deltaY = ball.y - nearestY;

    return (deltaX * deltaX + deltaY * deltaY) < (ball.radius * ball.radius);
}