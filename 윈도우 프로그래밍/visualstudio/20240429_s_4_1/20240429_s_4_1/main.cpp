#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>
#include"resource.h"

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"�ǽ� 3-2";
LPCTSTR lpszWindowName = L"�ǽ� 3-2";

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
    WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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
    bool visible = true;  // �߰�: ������ ���̴��� ����
};

struct Circle {
    int x, y, radius;
    COLORREF color = RGB(0, 0, 255);
};

struct Velocity {
    int dx;
    int dy;
};

bool CheckCollision(const Circle& ball, const RECT& block);

#define BLOCK_WIDTH 50  // ����� ���� ũ��
#define BLOCK_HEIGHT 30 // ����� ���� ũ��
#define BLOCK_COUNT 20  // �� �ٿ� ���� ��� ��
#define ROW_COUNT 3     // �� ��

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
    static int blockSpeed = 5;   // ������ ������ �ӵ�
    static int blockDirection = 1; // ������ ������ ���� (1: ������, -1: ����)
    static int speedY = -1;
    static int speedYr = -1;
    static Circle ball;
    static int speed1 = 0;
    static Velocity ballVelocity = { 5, -5 }; // �ʱ� �ӵ� (���� ��)
    static bool puzz = true;
    switch (uMsg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_GAME_START:
            puzz = false;
            break;
        case ID_GAME_RESET:
        {
            blocks.clear();
            puzz = true;
            int startX = (WIDTH - (BLOCK_WIDTH * BLOCK_COUNT)) / 2; // �߾� ������ ���� ���� X ��ǥ
            int startY = HEIGHT - (BLOCK_HEIGHT * ROW_COUNT) - 50;   // �ϴ� ������ ���� ���� Y ��ǥ (50�� �ٴ� ����)

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

            // ���� �ʱ� ��ġ�� �������� ��� �߾ӿ� �����մϴ�.
            ball.x = WIDTH / 2;
            ball.y = 30; // ��ܿ��� 30�ȼ� �Ʒ��� ��ġ
            ball.radius = 20; // �������� 20�ȼ��� ����
        }
            break;
        case ID_GAME_END:
            PostQuitMessage(0);
            break;
        case ID_SPEED_FAST:
            speed1++;
            break;
        case ID_SPEED_MEDIUM:
            speed1 = -1;
            break;
        case ID_SPEED_SLOW:
            if (speed1 < 0) {
                break;
            }
            else {
                speed1--;
            }
            break;
        case ID_COLOR_CYAN:
            ball.color = RGB(0, 0, 0);
            break;
        case ID_COLOR_GREEN:
            ball.color = RGB(0, 255, 0);
            break;
        case ID_COLOR_YELLOW:
            ball.color = RGB(255, 255, 0);
            break;
        case ID_SIZE_SMALL:
            ball.radius = 10;
            break;
        case ID_SIZE_BIG:
            ball.radius = 30;
        case ID_NUMBER_3:
        {
            blocks.clear();
            int startX = (WIDTH - (BLOCK_WIDTH * BLOCK_COUNT)) / 2; // �߾� ������ ���� ���� X ��ǥ
            int startY = HEIGHT - (BLOCK_HEIGHT * ROW_COUNT) - 50;   // �ϴ� ������ ���� ���� Y ��ǥ (50�� �ٴ� ����)

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
        }
            break;
        case ID_NUMBER_4:
        {
            blocks.clear();
            int startX = (WIDTH - (BLOCK_WIDTH * BLOCK_COUNT)) / 2; // �߾� ������ ���� ���� X ��ǥ
            int startY = HEIGHT - (BLOCK_HEIGHT * 4) - 50;   // �ϴ� ������ ���� ���� Y ��ǥ (50�� �ٴ� ����)

            blocks.reserve(4 * BLOCK_COUNT);
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < BLOCK_COUNT; ++j) {
                    Block b;
                    b.x = startX + (j * BLOCK_WIDTH);
                    b.y = startY + (i * BLOCK_HEIGHT);
                    b.rect = { b.x, b.y, b.x + BLOCK_WIDTH, b.y + BLOCK_HEIGHT };
                    blocks.push_back(b);
                }
            }
        }
            break;
        case ID_NUMBER_5:
        {
            blocks.clear();
            int startX = (WIDTH - (BLOCK_WIDTH * BLOCK_COUNT)) / 2; // �߾� ������ ���� ���� X ��ǥ
            int startY = HEIGHT - (BLOCK_HEIGHT * 5) - 50;   // �ϴ� ������ ���� ���� Y ��ǥ (50�� �ٴ� ����)

            blocks.reserve(5 * BLOCK_COUNT);
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < BLOCK_COUNT; ++j) {
                    Block b;
                    b.x = startX + (j * BLOCK_WIDTH);
                    b.y = startY + (i * BLOCK_HEIGHT);
                    b.rect = { b.x, b.y, b.x + BLOCK_WIDTH, b.y + BLOCK_HEIGHT };
                    blocks.push_back(b);
                }
            }
        }
            break;
        default:
            break;
        }
        break;
    case WM_CREATE:
    {
        SetTimer(hWnd, 1, 50, NULL); // 50ms ���� WM_TIMER �޽��� �߻�

        int startX = (WIDTH - (BLOCK_WIDTH * BLOCK_COUNT)) / 2; // �߾� ������ ���� ���� X ��ǥ
        int startY = HEIGHT - (BLOCK_HEIGHT * ROW_COUNT) - 50;   // �ϴ� ������ ���� ���� Y ��ǥ (50�� �ٴ� ����)

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

        // ���� �ʱ� ��ġ�� �������� ��� �߾ӿ� �����մϴ�.
        ball.x = WIDTH / 2;
        ball.y = 30; // ��ܿ��� 30�ȼ� �Ʒ��� ��ġ
        ball.radius = 20; // �������� 20�ȼ��� ����

        break;
    }
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // Ű���� Ű�� ������ ��
        break;
    case WM_LBUTTONDOWN: {  // ���콺 ���� ��ư Ŭ��
        for (auto& block : blocks) {
            block.x -= 30;  // ��� ������ �������� 10 �ȼ� �̵�
            block.rect = { block.x, block.y, block.x + BLOCK_WIDTH, block.y + BLOCK_HEIGHT };
        }
        InvalidateRect(hWnd, NULL, TRUE);  // �����츦 �ٽ� �׸��ϴ�.
        break;
    }
    case WM_RBUTTONDOWN: {  // ���콺 ������ ��ư Ŭ��
        for (auto& block : blocks) {
            block.x += 30;  // ��� ������ ���������� 10 �ȼ� �̵�
            block.rect = { block.x, block.y, block.x + BLOCK_WIDTH, block.y + BLOCK_HEIGHT };
        }
        InvalidateRect(hWnd, NULL, TRUE);  // �����츦 �ٽ� �׸��ϴ�.
        break;
    }
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
            int startX = (WIDTH - (BLOCK_WIDTH * BLOCK_COUNT)) / 2; // �߾� ������ ���� ���� X ��ǥ
            int startY = HEIGHT - (BLOCK_HEIGHT * ROW_COUNT) - 50;   // �ϴ� ������ ���� ���� Y ��ǥ (50�� �ٴ� ����)

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

            // ���� �ʱ� ��ġ�� �������� ��� �߾ӿ� �����մϴ�.
            ball.x = WIDTH / 2;
            ball.y = 30; // ��ܿ��� 30�ȼ� �Ʒ��� ��ġ
            ball.radius = 20; // �������� 20�ȼ��� ����
            break;
        }
        case 'p':
        {
            // ������ �Ͻ� �����մϴ�.
            puzz = true;

            int cnt{};

            for (int i = 0; i < blocks.size(); ++i) {
                if (blocks[i].stack == 1) {
                    cnt++;
                }
            }

            // �޽��� ���ڿ� ����
            wstring message = L"������ �Ͻ� �����Ǿ����ϴ�. ����� ������ ��: " + to_wstring(cnt) + L". ����Ϸ��� Ȯ���� ��������.";


            MessageBox(
                hWnd,  // �޽��� �ڽ��� �θ� �����츦 �����մϴ�.
                message.c_str(),  // �޽��� �ڽ��� ǥ���� �ؽ�Ʈ�Դϴ�.
                L"���� �Ͻ� ����",  // �޽��� �ڽ��� �����Դϴ�.
                MB_ICONINFORMATION | MB_OK  // 'Ȯ��' ��ư�� �ִ� ���� ������ �޽��� �ڽ��Դϴ�.
            );

            // �޽��� �ڽ����� "Ȯ��"�� ���� ��, ������ �ٽ� �����մϴ�.
            puzz = false;

            break;
        }
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
                mBrush = CreateSolidBrush(block.color); // ������ �귯�� ����
                oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
                Rectangle(mDC, block.rect.left, block.rect.top, block.rect.right, block.rect.bottom);
                SelectObject(mDC, oldBrush); // ���� �귯�÷� ����
                DeleteObject(mBrush); // �귯�� ��ü ����
            }
        }


        // �� �׸���
        mBrush = CreateSolidBrush(ball.color); // �Ķ��� �귯�� ����
        oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
        Ellipse(mDC, ball.x - ball.radius, ball.y - ball.radius, ball.x + ball.radius, ball.y + ball.radius);
        SelectObject(mDC, oldBrush); // ���� �귯�÷� ����
        DeleteObject(mBrush); // �귯�� ��ü ����

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);
        DeleteObject(hBitmap); // ������ ��Ʈ�� ����
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        if (!puzz) {
            switch (wParam) {
            case 1:

                // �����̳� ������ ���� �����ߴ��� Ȯ��
                if (blocks.front().x <= 0 || blocks.back().x + BLOCK_WIDTH >= WIDTH) {
                    blockDirection *= -1; // ���� ����
                }

                // ��� ������ ��ġ ������Ʈ
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
                    case 0: //�������� �ö󰡱�
                        ball.y = ball.y - 2 - speed1;
                        break;
                    case 1: // 45���� ����~ ����
                        ball.x = ball.x - 3 - speed1;
                        ball.y = ball.y - 3 - speed1;
                        break;
                    case 2: //45���� ������~����
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

                // ���� ������ ���� ����� ��
                if ((ball.x + ball.radius) >= WIDTH) {
                    ball.x = WIDTH - ball.radius;  // ���� ȭ�� ������ ������ �ʵ��� ��ġ ����
                    if (speedYr == 2) {
                        speedYr = 1;
                    }
                    else if (speedYr == 3) {
                        speedYr = 4;
                    }
                }

                //���� õ�忡 �������
                if ((ball.y - ball.radius) <= 0) {
                    ball.y = ball.radius; // ���� ȭ�� ������ ������ �ʵ��� ����
                    speedYr = -1;
                    speedY = uid_ran_3(gen);
                }

                for (auto& block : blocks) {
                    if (CheckCollision(ball, block.rect) && block.visible) { //�浹!
                        speedY = -1;
                        speedYr = uid_ran_4(gen);
                        block.stack += 1;
                        block.color = RGB(0, 0, 0);
                        if (block.stack == 2) { // �ι� �ε�����
                            block.visible = false;
                        }
                        break; // �ٸ� ��ϰ��� �浹 �˻縦 �ߴ�
                    }
                }

                if (ball.y >= HEIGHT) {
                    blocks.clear();

                    int startX = (WIDTH - (BLOCK_WIDTH * BLOCK_COUNT)) / 2; // �߾� ������ ���� ���� X ��ǥ
                    int startY = HEIGHT - (BLOCK_HEIGHT * ROW_COUNT) - 50;   // �ϴ� ������ ���� ���� Y ��ǥ (50�� �ٴ� ����)

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

                    // ���� �ʱ� ��ġ�� �������� ��� �߾ӿ� �����մϴ�.
                    ball.x = WIDTH / 2;
                    ball.y = 30; // ��ܿ��� 30�ȼ� �Ʒ��� ��ġ
                    ball.radius = 20; // �������� 20�ȼ��� ����
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
        KillTimer(hWnd, 1); // Ÿ�̸� ����
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