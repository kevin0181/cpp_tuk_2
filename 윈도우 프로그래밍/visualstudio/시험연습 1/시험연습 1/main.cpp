#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"시험 연습1";
LPCTSTR lpszWindowName = L"시험 연습1";

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

struct Point {
    int x;
    int y;
    RECT rect;
    int block_status; // 0 1
    COLORREF color;
    int arrow;
};

int move_to_block(Point player, vector<Point> blocks) {
    for (int i = 0; i < blocks.size(); ++i) {
        if (player.x == blocks[i].x && player.y == blocks[i].y) {
            return 1;
        }
    }
    return 0;
}

void bullet_block(Point bullets, vector<Point>& blocks) {
    for (int i = 0; i < blocks.size(); ++i) {
        if (bullets.x == blocks[i].x && bullets.y == blocks[i].y) {
            blocks.erase(blocks.begin() + i);
        }
    }
}

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_block_position(0, 19);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitMap;

    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;

    const int WIDTH_GRID = 20;
    const int HEIGHT_GRID = 20;

    static RECT rect;
    static int cellSize_x{};
    static int cellSize_y{};
    static Point player;
    static vector<Point> blocks;
    static vector<Point> bullets;
    static bool reload_status = false;
    static bool Drag = false;
    switch (uMsg)
    {
    case WM_CREATE:

        player.x = 0;
        player.y = 0;
        player.color = RGB(255, 0, 0);

        SetTimer(hWnd, 1, 1000, NULL); // block
        SetTimer(hWnd, 2, 100, NULL); // bullet

        break;
    case WM_RBUTTONDOWN:
        break;
    case WM_LBUTTONDOWN:
    {
        Drag = true;

        POINT p;
        p.x = LOWORD(lParam);
        p.y = HIWORD(lParam);

        // 그리드 크기에 맞춰 플레이어 위치를 계산
        player.x = p.x / cellSize_x;
        player.y = p.y / cellSize_y;

        // 그리드 안에서만 플레이어가 움직이도록 제한
        player.x = max(0, min(player.x, WIDTH_GRID - 1));
        player.y = max(0, min(player.y, HEIGHT_GRID - 1));
        InvalidateRect(hWnd, NULL, false); // 화면을 갱신하여 변경된 위치에 플레이어를 그림

        SetCapture(hWnd);
    }
    break;
    case WM_MOUSEMOVE:
    {
        if (Drag && (wParam & MK_LBUTTON)) {
            POINT p;
            p.x = LOWORD(lParam);
            p.y = HIWORD(lParam);

            // 그리드 크기에 맞춰 플레이어 위치를 계산
            player.x = p.x / cellSize_x;
            player.y = p.y / cellSize_y;

            // 그리드 안에서만 플레이어가 움직이도록 제한
            player.x = max(0, min(player.x, WIDTH_GRID - 1));
            player.y = max(0, min(player.y, HEIGHT_GRID - 1));
            InvalidateRect(hWnd, NULL, FALSE); // 화면 갱신 요청
        }
    }
    break;
    case WM_LBUTTONUP:
        if (Drag) {
            Drag = false;
            ReleaseCapture();
        }
        break;
    case WM_KEYDOWN:
        switch (wParam) //move
        {
        case VK_LEFT:
            if (player.x == 0) {
                player.x = 19;
                if (1 == move_to_block(player, blocks))
                    player.x = 0;
                break;
            }

            player.x -= 1;
            player.x += move_to_block(player, blocks);
            player.arrow = 1;
            break;
        case VK_UP:
            if (player.y == 0) {
                player.y = 19;
                if (1 == move_to_block(player, blocks))
                    player.y = 0;
                break;
            }
            player.y -= 1;
            player.y += move_to_block(player, blocks);
            player.arrow = 2;
            break;
        case VK_RIGHT:
            if (player.x == 19) {
                player.x = 0;
                if (1 == move_to_block(player, blocks))
                    player.x = 19;
                break;
            }
            player.x += 1;
            player.x -= move_to_block(player, blocks);
            player.arrow = 3;
            break;
        case VK_DOWN:
            if (player.y == 19) {
                player.y = 0;
                if (1 == move_to_block(player, blocks))
                    player.y = 19;
                break;
            }
            player.y += 1;
            player.y -= move_to_block(player, blocks);
            player.arrow = 4;
            break;
        case VK_SPACE:
            if (bullets.size() >= 30) { // reload
                reload_status = true; //need reload
            }

            if (!reload_status) {
                Point bullet;
                bullet.x = player.x;
                bullet.y = player.y;
                bullet.arrow = player.arrow;
                bullet.color = RGB(0, 125, 0);
                bullets.push_back(bullet);
            }

            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_CHAR:
        switch (wParam)
        {
        case 'r':
            bullets.clear();
            reload_status = false;
            break;
        default:
            break;
        }
        break;
        InvalidateRect(hWnd, NULL, false);
    case WM_PAINT:
        GetClientRect(hWnd, &rect);
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitMap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        SelectObject(mDC, (HBITMAP)hBitMap);
        FillRect(mDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));


        cellSize_x = rect.right / WIDTH_GRID;
        cellSize_y = rect.bottom / HEIGHT_GRID; // 현재 창 크기 구하기

        player.rect = { player.x * cellSize_x, player.y * cellSize_y, (player.x + 1) * cellSize_x, (player.y + 1) * cellSize_y }; // player size

        for (auto& block : blocks) { // block size
            block.rect = { block.x * cellSize_x,block.y * cellSize_y,(block.x + 1) * cellSize_x,(block.y + 1) * cellSize_y };
        }

        for (auto& bullet : bullets) { // bullet size
            bullet.rect = { bullet.x * cellSize_x + 20, bullet.y * cellSize_y + 10, (bullet.x + 1) * cellSize_x - 20, (bullet.y + 1) * cellSize_y - 10 };
        }


        for (int x = 0; x <= cellSize_x; ++x) { // x line
            MoveToEx(mDC, 0, x * cellSize_y, NULL);
            LineTo(mDC, WIDTH_GRID * cellSize_x, x * cellSize_y);
        }

        for (int y = 0; y <= cellSize_y; ++y) { // y line
            MoveToEx(mDC, y * cellSize_x, 0, NULL);
            LineTo(mDC, y * cellSize_x, HEIGHT_GRID * cellSize_y);
        }

        //player paint
        mBrush = CreateSolidBrush(player.color);
        oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
        mPen = CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
        oldPen = (HPEN)SelectObject(mDC, mPen);
        Rectangle(mDC, player.rect.left, player.rect.top, player.rect.right, player.rect.bottom);
        SelectObject(mDC, oldBrush);
        DeleteObject(mBrush);
        SelectObject(mDC, oldPen);
        DeleteObject(mPen);


        //block paint
        for (int i = 0; i < blocks.size(); ++i) {
            mBrush = CreateSolidBrush(blocks[i].color);
            oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
            mPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
            oldPen = (HPEN)SelectObject(mDC, mPen);
            Rectangle(mDC, blocks[i].rect.left, blocks[i].rect.top, blocks[i].rect.right, blocks[i].rect.bottom);
            SelectObject(mDC, oldBrush);
            DeleteObject(mBrush);
            SelectObject(mDC, oldPen);
            DeleteObject(mPen);
        }


        //bullet paint
        for (int i = 0; i < bullets.size(); ++i) {
            mBrush = CreateSolidBrush(bullets[i].color);
            oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
            mPen = CreatePen(PS_SOLID, 0, RGB(0, 125, 0));
            oldPen = (HPEN)SelectObject(mDC, mPen);
            Ellipse(mDC, bullets[i].rect.left, bullets[i].rect.top, bullets[i].rect.right, bullets[i].rect.bottom);
            SelectObject(mDC, oldBrush);
            DeleteObject(mBrush);
            SelectObject(mDC, oldPen);
            DeleteObject(mPen);
        }

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);
        DeleteObject(hBitMap);
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    case WM_TIMER:
        switch (wParam)
        {
        case 1:
            Point block;
            block.color = RGB(0, 0, 0);
            block.x = uid_block_position(gen);
            block.y = uid_block_position(gen);
            blocks.push_back(block);
            InvalidateRect(hWnd, NULL, false);
            break;
        case 2:
            for (int i = 0; i < bullets.size(); ++i) {
                switch (bullets[i].arrow)
                {
                case 1:
                    bullets[i].x--;
                    break;
                case 2:
                    bullets[i].y--;
                    break;
                case 3:
                    bullets[i].x++;
                    break;
                case 4:
                    bullets[i].y++;
                    break;
                default:
                    break;
                }
                bullet_block(bullets[i], blocks);
            }
            InvalidateRect(hWnd, NULL, false);
            break;
        default:
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}