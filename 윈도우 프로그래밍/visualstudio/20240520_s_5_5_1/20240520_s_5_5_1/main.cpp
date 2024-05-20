#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <atlimage.h>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"INVERSUS";
LPCTSTR lpszWindowName = L"INVERSUS";

#define WIDTH 1200
#define HEIGHT 800
#define BLOCK_SIZE 50
#define MAX_BLOCKS 10

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

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
        20, 20, 1000, 800, NULL, (HMENU)NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return Message.wParam;
}

#define IMG_CELL_SIZE 165

#pragma once
#include <Windows.h>

class Explosion {
public:
    RECT rect;
    COLORREF color;
    int frame;

    Explosion(RECT r, COLORREF c) : rect(r), color(c), frame(0) {}

    bool update() {
        ++frame;
        return frame < 9; // 프레임
    }

    void draw(HDC& mDC, const Explosion& ex) const {
        HBRUSH brush = CreateSolidBrush(ex.color);
        HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, brush);

        // 난수 생성기 초기화
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distX(ex.rect.left, ex.rect.right);
        std::uniform_int_distribution<int> distY(ex.rect.top, ex.rect.bottom);
        std::uniform_int_distribution<int> distSize(20, 50); // 원의 크기 범위

        // 여러 개의 원을 그려 폭발 효과 생성
        for (int i = 0; i < 10; ++i) { // 폭발 조각의 개수
            int x = distX(gen);
            int y = distY(gen);
            int size = distSize(gen);
            Ellipse(mDC, x - size, y - size, x + size, y + size);
        }
        SelectObject(mDC, oldBrush);
        DeleteObject(brush);
    }
};

struct Bullet {
    bool left;
    bool right;
    bool up;
    bool down;
    RECT rect = { 0,0,10,10 };
    COLORREF color = RGB(255, 0, 0);
};

struct Move {
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    int jump = -1;
    bool jump_status = false;
};

struct Block {
    RECT rect;
    COLORREF color;
    int cnt = 3;
    vector<Block> inBlocks;
};

struct Character {
    int x = 0;
    int y = 0;
    int width = 100;
    int height = 100;
    int move_status = 0;
    Move move;
    CImage imgs[10];
    int select_img = 0;

    void print_v(HDC& mDC1) {
        if (move_status != -1)
            imgs[move_status].Draw(mDC1, x, y, width, height, IMG_CELL_SIZE * select_img, 0, IMG_CELL_SIZE, IMG_CELL_SIZE);
    }

    void print_v2(HDC& mDC1, int i, CImage imgs[10]) {
        if (move_status != -1)
            imgs[move_status].Draw(mDC1, x + (i * 100), y, width, height, IMG_CELL_SIZE * select_img, 0, IMG_CELL_SIZE, IMG_CELL_SIZE);
    }

    Character() {}
};

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_block_cnt(3, 6);
uniform_int_distribution<int> uid_block_shape(2, 6); // 랜덤한 블록 크기를 위해 추가
uniform_int_distribution<int> uid_x(0, WIDTH - BLOCK_SIZE * 6);
uniform_int_distribution<int> uid_y(0, HEIGHT - BLOCK_SIZE * 6);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;

    static RECT rect;
    static Character character;
    static vector<Block> blocks;
    static vector<Bullet> bullets;
    static vector<Character> cats;
    static CImage imgs[10];
    static vector<Explosion> explodes;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, 2, 1, NULL);
        SetTimer(hWnd, 1, 100, NULL);
        GetClientRect(hWnd, &rect);

        imgs[0].Load(L"among us move right1_v2.png"); // 우로 걷기
        imgs[1].Load(L"among us move left1_v2.png"); // 좌로 걷기
        imgs[2].Load(L"among us move back1_v2.png"); // 위로 걷기
        imgs[3].Load(L"among us move front1_v2.png"); // 아래로 걷기
        //imgs[4].Load(L"among us jump right1.png"); // 우 점프
        //imgs[5].Load(L"among us jump left1.png"); // 좌 점프
        //imgs[6].Load(L"among us move right up1.png"); // 우 상단
        //imgs[7].Load(L"among us move right down1.png"); // 우 하단
        //imgs[8].Load(L"among us move left up1.png"); // 좌 상단
        //imgs[9].Load(L"among us move left down1.png"); // 좌 하단

        character.imgs[0].Load(L"among us move right1.png");
        character.imgs[1].Load(L"among us move left1.png");
        character.imgs[2].Load(L"among us move back1.png");
        character.imgs[3].Load(L"among us move front1.png");
        character.imgs[4].Load(L"among us jump right1.png");
        character.imgs[5].Load(L"among us jump left1.png");
        character.imgs[6].Load(L"among us move right up1.png");
        character.imgs[7].Load(L"among us move right down1.png");
        character.imgs[8].Load(L"among us move left up1.png");
        character.imgs[9].Load(L"among us move left down1.png");

        // 블록 생성
        for (int i = 0; i < MAX_BLOCKS; ++i) {
            int block_width = uid_block_shape(gen);
            int block_height = uid_block_shape(gen);
            int x = uid_x(gen);
            int y = uid_y(gen);

            Block b;
            for (int row = 0; row < block_height; ++row) {
                for (int col = 0; col < block_width; ++col) {
                    int block_x = x + col * BLOCK_SIZE;
                    int block_y = y + row * BLOCK_SIZE;
                    b.inBlocks.push_back({ {block_x, block_y, block_x + BLOCK_SIZE, block_y + BLOCK_SIZE}, RGB(0, 0, 0), uid_block_cnt(gen) });
                }
            }
            blocks.push_back(b);
        }
       
        for (int i = 0; i < 3; ++i) {
            Character cha;
            cha.x = i * 3;
            cha.move.right = true;
            cha.x += 100;
            cha.y += 100;
            cha.move_status = 0;
            cats.push_back(cha);
        }

        break;
    }
    case WM_CHAR:
        switch (wParam)
        {
        case 'j':
            character.move.jump_status = true;
            character.move.jump = 0;
            if (character.move_status == 1) {
                character.move_status = 5;
            }
            else if (character.move_status == 0) {
                character.move_status = 4;
            }
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYUP:
        switch (wParam)
        {
        case VK_LEFT:
            character.move.left = false;
            character.select_img = 0;
            break;
        case VK_RIGHT:
            character.move.right = false;
            character.select_img = 0;
            break;
        case VK_UP:
            character.move.up = false;
            character.select_img = 0;
            break;
        case VK_DOWN:
            character.move.down = false;
            character.select_img = 0;
            break;
        default:
            break;
        }
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
            character.move.left = true;
            character.move_status = 1;
            break;
        case VK_RIGHT:
            character.move.right = true;
            character.move_status = 0;
            break;
        case VK_UP:
            character.move.up = true;
            character.move_status = 2;
            break;
        case VK_DOWN:
            character.move.down = true;
            character.move_status = 3;
            break;
        case VK_SPACE:
        {
            Bullet b;
            if (character.move.left || character.move.right || character.move.up || character.move.down) {
                b.left = character.move.left;
                b.right = character.move.right;
                b.up = character.move.up;
                b.down = character.move.down;
                OffsetRect(&b.rect, character.x + 50, character.y + 50);
                bullets.push_back(b);
            }
            break;
        }
        default:
            break;
        }

        if (character.move.right && character.move.down) {
            character.move_status = 7;
        }

        if (character.move.right && character.move.up) {
            character.move_status = 6;
        }

        if (character.move.left && character.move.down) {
            character.move_status = 9;
        }

        if (character.move.left && character.move.up) {
            character.move_status = 8;
        }
        break;
    case WM_PAINT:
    {
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        SelectObject(mDC, (HBITMAP)hBitmap);

        FillRect(mDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

        for (const auto& bullet : bullets) {
            HBRUSH hBrush = CreateSolidBrush(bullet.color);
            HPEN hPen = CreatePen(PS_SOLID, 1, bullet.color);
            HGDIOBJ oldBrush = SelectObject(mDC, hBrush);
            HGDIOBJ oldPen = SelectObject(mDC, hPen);
            Ellipse(mDC, bullet.rect.left, bullet.rect.top, bullet.rect.right, bullet.rect.bottom);
            SelectObject(mDC, oldBrush);
            SelectObject(mDC, oldPen);
            DeleteObject(hBrush);
            DeleteObject(hPen);
        }

        for (int i = 0; i < cats.size(); ++i) {
            cats[i].print_v2(mDC, i, imgs);
        }

        for (const auto& block : blocks) {
            for (const auto& inBlock : block.inBlocks) {
                HBRUSH hBrush = CreateSolidBrush(inBlock.color);
                HPEN hPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
                HGDIOBJ oldBrush = SelectObject(mDC, hBrush);
                HGDIOBJ oldPen = SelectObject(mDC, hPen);
                Rectangle(mDC, inBlock.rect.left, inBlock.rect.top, inBlock.rect.right, inBlock.rect.bottom);
                SelectObject(mDC, oldBrush);
                SelectObject(mDC, oldPen);
                DeleteObject(hBrush);
                DeleteObject(hPen);
            }
        }

        // 폭발 그리기
        for (const auto& explosion : explodes) {
            explosion.draw(mDC, explosion);
        }

        character.print_v(mDC);

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);

        DeleteObject(hBitmap);
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        switch (wParam)
        {
        case 1:
        {

            for (int i = 0; i < cats.size(); ++i) {
                Move& move = cats[i].move;
                if (move.down || move.left || move.up || move.right) {
                    cats[i].select_img++;
                    if (cats[i].select_img == 4) {
                        cats[i].select_img = 0;
                    }

                    if (move.down) {
                        cats[i].y += 7;
                    }

                    if (move.left) {
                        cats[i].x -= 7;
                    }

                    if (move.up) {
                        cats[i].y -= 7;
                    }

                    if (move.right) {
                        cats[i].x += 7;
                    }
                }

                if (cats[i].x + 100 >= rect.right) {
                    cats[i].y += 200;
                    cats[i].move.right = false;
                    cats[i].move.left = true;
                    cats[i].move_status = 1;
                }

            }



            Move& move = character.move;
            if (move.down || move.left || move.up || move.right) {
                character.select_img++;
                if (character.select_img == 4) {
                    character.select_img = 0;
                }

                if (move.down) {
                    character.y += 7;
                }

                if (move.left) {
                    character.x -= 7;
                }

                if (move.up) {
                    character.y -= 7;
                }

                if (move.right) {
                    character.x += 7;
                }

            }
            if (move.jump != -1 && move.jump_status) {
                move.jump += 10;
                character.y -= move.jump;
                if (move.jump >= 20) {
                    move.jump_status = false;
                }
            }
            else if (move.jump != -1 && !move.jump_status) {
                move.jump += 10;
                character.y += move.jump;
                if (move.jump >= 30) {
                    move.jump = -1;
                    move.jump_status = false;

                    if (character.move_status == 5) {
                        character.move_status = 1;
                        move.left = true;
                    }
                    else if (character.move_status == 4) {
                        character.move_status = 0;
                        move.right = true;
                    }
                }
            }


            for (int i = 0; i < bullets.size(); ++i) {
                if (bullets[i].left) {
                    OffsetRect(&bullets[i].rect, -10, 0);
                }

                if (bullets[i].right) {
                    OffsetRect(&bullets[i].rect, 10, 0);
                }

                if (bullets[i].up) {
                    OffsetRect(&bullets[i].rect, 0, -10);
                }

                if (bullets[i].down) {
                    OffsetRect(&bullets[i].rect, 0, 10);
                }
            }
            break;
        }
        case 2:

            for (auto it = explodes.begin(); it != explodes.end(); ) { // 폭발 애니메이션 프레임
                if (!it->update()) {
                    if (explodes.size() >= 1) {
                        it = explodes.erase(it);
                    }
                }
                else {
                    ++it;
                }
            }

            RECT r;
            for (auto& block : blocks) {
                for (int i = 0; i < block.inBlocks.size(); ++i) {
                    for (int j = 0; j < bullets.size(); ++j) {
                        if (IntersectRect(&r, &bullets[j].rect, &block.inBlocks[i].rect)) {
                            block.inBlocks[i].cnt--;
                            bullets.erase(bullets.begin() + j); // 총알 제거
                            --j; // 총알 인덱스 조정
                            
                            if (block.inBlocks[i].cnt == 0) {

                                Explosion e(block.inBlocks[i].rect, RGB(0, 0, 0));

                                explodes.push_back(e);

                                block.inBlocks.erase(block.inBlocks.begin() + i);
                                --i; // 블록 인덱스 조정
                                break; // 이미 이 블록은 삭제되었으므로 내부 루프 종료
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < bullets.size(); ++i) {
                for (int j = 0; j < cats.size(); ++j) {
                    RECT catRect;
                    catRect = { cats[j].x, cats[j].y, cats[j].x + 100, cats[j].y + 150 };
                    if (IntersectRect(&r, &bullets[i].rect, &catRect)) {
                        Explosion e(catRect, RGB(255, 0, 0));

                        explodes.push_back(e);
                        cats.erase(cats.begin() + j); 
                    }
                }
            }
            break;
        default:
            break;
        }

        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}