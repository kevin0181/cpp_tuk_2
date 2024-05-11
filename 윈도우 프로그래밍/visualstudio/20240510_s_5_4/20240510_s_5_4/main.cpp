#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <numeric>
#include<atlimage.h>
#include <gdiplus.h>

using namespace Gdiplus;

using namespace std;

ULONG_PTR gdiplusToken;

HINSTANCE g_hInst;

LPCTSTR lpszClass = L"실습 5-4";
LPCTSTR lpszWindowName = L"실습 5-4";

#define WIDTH 1000
#define HEIGHT 900

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {

    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

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

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_VSCROLL, 200, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    GdiplusShutdown(gdiplusToken);

    return Message.wParam;

}

#define IMG_CELL_SIZE 165

struct Move {
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    int jump = -1;
    bool jump_status = false;
};

struct Character {
    int x = 0;
    int y = 0;
    int width = 100;
    int height = 100;
    int targetX = 0; // Target x-coordinate
    int targetY = 0; // Target y-coordinate
    int move_status = 0; // 초기 이미지 인덱스
    Move move;
    int select_img = 0;
    bool x_move = true;

    void print_v(HDC& mDC1, int i,CImage imgs[10]) {
        if (move_status != -1)
            imgs[move_status].Draw(mDC1, x + (i * 100), y, width, height, IMG_CELL_SIZE * select_img, 0, IMG_CELL_SIZE, IMG_CELL_SIZE);
    }

    Character() {
       
    }
};

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC1, mDC2;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    HBITMAP oldBit1, oldBit2;

    static HBITMAP hBit1, hBit2;
    static RECT rect;
    static SIZE size;
    static vector<Character> character;
    static bool e_status = true;
    static bool s_status = true;
    static CImage backgroundImage;
    static CImage imgs[10];
    static vector<Character> saveCharacter;

    static int selectCharacter = 0;
    static bool a_status = false;
    static bool m_status = false;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        
        SetTimer(hWnd, 1, 160, NULL);
        Character cha;
        character.push_back(cha);

        imgs[0].Load(L"among us move right1.png"); // 우로 걷기
        imgs[1].Load(L"among us move left1.png"); // 좌로 걷기
        imgs[2].Load(L"among us move back1.png"); // 위로 걷기
        imgs[3].Load(L"among us move front1.png"); // 아래로 걷기
        imgs[4].Load(L"among us jump right1.png"); // 우 점프
        imgs[5].Load(L"among us jump left1.png"); // 좌 점프
        imgs[6].Load(L"among us move right up1.png"); // 우 상단
        imgs[7].Load(L"among us move right down1.png"); // 우 하단
        imgs[8].Load(L"among us move left up1.png"); // 좌 상단
        imgs[9].Load(L"among us move left down1.png"); // 좌 하단

        for (int i = 0; i < 10; ++i) {
            if (imgs[i].IsNull()) {
                wchar_t errorMsg[256];
                swprintf_s(errorMsg, L"이미지 로드에 실패했습니다: %d번째 이미지", i);
                MessageBox(hWnd, errorMsg, L"이미지 로드 실패", MB_OK | MB_ICONERROR);
            }
        }

        backgroundImage.Load(L"b.png");

        break;
    }
    case WM_COMMAND:
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYUP:
        switch (wParam)
        {
        case VK_LEFT:
            for (int i = 0; i < character.size(); ++i) {
                character[i].move.left = false;
                character[i].select_img = 0;
            }
            break;
        case VK_RIGHT:
            for (int i = 0; i < character.size(); ++i) {
                character[i].move.right = false;
                character[i].select_img = 0;
            }
            break;
        case VK_UP:
            for (int i = 0; i < character.size(); ++i) {
                character[i].move.up = false;
                character[i].select_img = 0;
            }
            break;
        case VK_DOWN:
            for (int i = 0; i < character.size(); ++i) {
                character[i].move.down = false;
                character[i].select_img = 0;
            }
            break;
        default:
            break;
        }

        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때
        switch (wParam)
        {
        case VK_LEFT:
            for (int i = 0; i < character.size(); ++i) {
                character[i].move.left = true;
                character[i].move_status = 1;
            }
            break;
        case VK_RIGHT:
            for (int i = 0; i < character.size(); ++i) {
                character[i].move.right = true;
                character[i].move_status = 0;
            }
            break;
        case VK_UP:
            for (int i = 0; i < character.size(); ++i) {
                character[i].move.up = true;
                character[i].move_status = 2;
            }
            break;
        case VK_DOWN:
            for (int i = 0; i < character.size(); ++i) {
                character[i].move.down = true;
                character[i].move_status = 3;
            }
            break;
        default:
            break;
        }

        if (character[selectCharacter].move.right && character[selectCharacter].move.down) { //우 하단
            character[selectCharacter].move_status = 7;
        }

        if (character[selectCharacter].move.right && character[selectCharacter].move.up) { // 우 상단
            character[selectCharacter].move_status = 6;
        }

        if (character[selectCharacter].move.left && character[selectCharacter].move.down) { // 좌 하단
            character[selectCharacter].move_status = 9;
        }
        
        if (character[selectCharacter].move.left && character[selectCharacter].move.up) { // 좌 상단
            character[selectCharacter].move_status = 8;
        }

        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_CHAR:
        switch (wParam) 
        {
        case 'a':
            saveCharacter.clear();
            saveCharacter = character;
            for (int i = 0; i < character.size(); ++i) {
                character[i].x = 0;
                character[i].y = 0;
                character[i].move.right = true;
                character[i].move.left = false;
                character[i].move_status = 0;
            }
            a_status = true;
            SetTimer(hWnd, 4, 160, NULL);

            break;
        case '0':
            selectCharacter = 0;
            break;
        case '1':
            selectCharacter = 1;
            break;
        case '2':
            selectCharacter = 2;
            break;
        case '3':
            selectCharacter = 3;
            break;
        case '4':
            selectCharacter = 4;
            break;
        case '5':
            selectCharacter = 5;
            break;
        case 't':
        {
            if (character.size() >= 6) {
                break;
            }

            Character cha;
            cha = character[0];
            character.push_back(cha);
            
            break;
        }
        case 'e':
            SetTimer(hWnd, 2, 160, NULL);
            break;
        case 's':
            SetTimer(hWnd, 3, 160, NULL);
            break;
        case 'j':
            character[selectCharacter].move.jump_status = true;
            character[selectCharacter].move.jump = 0;
            if (character[selectCharacter].move_status == 1) {
                character[selectCharacter].move_status = 5;
            }
            else if (character[selectCharacter].move_status == 0) {
                character[selectCharacter].move_status = 4;
            }
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_LBUTTONDOWN:
    {
        m_status = true;
        int mouseX = LOWORD(lParam); // Get the horizontal position of the cursor
        int mouseY = HIWORD(lParam); // Get the vertical position of the cursor

        for (auto& chara : character) {
            chara.targetX = mouseX;
            chara.targetY = mouseY;
        }

        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
    case WM_MOUSEMOVE:
        break;
    case WM_LBUTTONUP:
        break;
    case WM_PAINT:
    {
        GetClientRect(hWnd, &rect);
        hDC = BeginPaint(hWnd, &ps);
        mDC1 = CreateCompatibleDC(hDC);
        hBit1 = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        oldBit1 = (HBITMAP)SelectObject(mDC1, hBit1);

        FillRect(mDC1, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

        backgroundImage.Draw(mDC1, 0, 0, rect.right, rect.bottom);
        
        for (int i = 0; i < character.size(); ++i) {
            character[i].print_v(mDC1, i, imgs);
        }

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC1, 0, 0, SRCCOPY);

        // 로드된 비트맵을 선택
        SelectObject(mDC1, oldBit1);
        DeleteObject(hBit1);  // 해제
        DeleteDC(mDC1);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        switch (wParam)
        {
        case 4:
            for (int i = 0; i < character.size(); ++i) {
                if (character[i].x_move) {
                    character[i].move.left = false;
                    character[i].move.right = true;
                    character[i].move_status = 0;
                    if (character[i].x + (i * 165) >= rect.right) {
                        character[i].y += 165;
                        character[i].x_move = false;
                        character[i].move.right = false;
                        character[i].move.left = true;
                        character[i].move_status = 1;
                    }
                }
                else {
                    character[i].move.right = false;
                    character[i].move.left = true;
                    character[i].move_status = 1;
                    if (character[i].x + (i * 165) <= 0) {
                        character[i].y += 165;
                        character[i].x_move = true;
                        character[i].move.right = true;
                        character[i].move.left = false;
                        character[i].move_status = 0;
                    }
                }
            }
            break;

        case 2:

            if (e_status) {
                character[selectCharacter].width += 10;
                character[selectCharacter].height +=10;
                if (character[selectCharacter].width >= 200 && character[selectCharacter].height >= 200) {
                    e_status = false;
                }
            }
            else {
                character[selectCharacter].width -= 10;
                character[selectCharacter].height -= 10;
                if (character[selectCharacter].width <= 100 && character[selectCharacter].height <= 100) {
                    e_status = true;
                    character[selectCharacter].width = 100;
                    character[selectCharacter].height = 100;
                    KillTimer(hWnd, 2);
                }
            }

            break;
        case 3:
            if (e_status) {
                character[selectCharacter].width -= 10;
                character[selectCharacter].height -= 10;
                if (character[selectCharacter].width <= 50 && character[selectCharacter].height <= 50) {
                    e_status = false;
                }
            }
            else {
                character[selectCharacter].width += 10;
                character[selectCharacter].height += 10;
                if (character[selectCharacter].width >= 100 && character[selectCharacter].height >= 100) {
                    e_status = true;
                    character[selectCharacter].width = 100;
                    character[selectCharacter].height = 100;
                    KillTimer(hWnd, 3);
                }
            }
            break;
        case 1:
        {
            if (m_status) {
                for (auto& chara : character) {
                    if (chara.y < chara.targetY) {
                        chara.move_status = 3;
                        chara.move.down = true;
                        chara.move.right = false;
                        chara.move.left = false;
                        chara.move.up = false;
                        chara.y += 7;
                    }
                    if (chara.y > chara.targetY) {
                        chara.move_status = 2;
                        chara.move.up = true;
                        chara.move.right = false;
                        chara.move.down = false;
                        chara.move.left = false;
                        chara.y -= 7;
                    }
                    if (chara.x < chara.targetX) {
                        chara.move_status = 0;
                        chara.move.right = true;
                        chara.move.down = false;
                        chara.move.up = false;
                        chara.move.left = false;
                        chara.x += 7;
                    }
                    if (chara.x > chara.targetX) {
                        chara.move_status = 1;
                        chara.move.left = true;
                        chara.move.right = false;
                        chara.move.down = false;
                        chara.move.up = false;
                        chara.x -= 7;
                    }
                    
                }
            }

            if (!a_status) {
                if (selectCharacter != 0) {
                    Move& move = character[selectCharacter].move;
                    if (move.down || move.left || move.up || move.right) {
                        character[selectCharacter].select_img++;
                        if (character[selectCharacter].select_img == 4) {
                            character[selectCharacter].select_img = 0;
                        }

                        if (move.down) {
                            character[selectCharacter].y += 7;
                        }

                        if (move.left) {
                            character[selectCharacter].x -= 7;
                        }

                        if (move.up) {
                            character[selectCharacter].y -= 7;
                        }

                        if (move.right) {
                            character[selectCharacter].x += 7;
                        }

                    }

                    if (move.jump != -1 && move.jump_status) {
                        move.jump += 10;
                        character[selectCharacter].y -= move.jump;
                        if (move.jump >= 20) {
                            move.jump_status = false;
                        }
                    }
                    else if (move.jump != -1 && !move.jump_status) {
                        move.jump += 10;
                        character[selectCharacter].y += move.jump;
                        if (move.jump >= 30) {
                            move.jump = -1;
                            move.jump_status = false;

                            if (character[selectCharacter].move_status == 5) {
                                character[selectCharacter].move_status = 1;
                                move.left = true;
                            }
                            else if (character[selectCharacter].move_status == 4) {
                                character[selectCharacter].move_status = 0;
                                move.right = true;
                            }

                        }
                    }
                }
                else if (selectCharacter == 0) {
                    Move& move = character[0].move;
                    for (int i = 0; i < character.size(); ++i) {
                        if (move.down || move.left || move.up || move.right) {
                            character[i].select_img++;
                            if (character[i].select_img == 4) {
                                character[i].select_img = 0;
                            }

                            if (move.down) {
                                character[i].y += 7;
                            }

                            if (move.left) {
                                character[i].x -= 7;
                            }

                            if (move.up) {
                                character[i].y -= 7;
                            }

                            if (move.right) {
                                character[i].x += 7;
                            }

                        }

                        if (move.jump != -1 && move.jump_status) {
                            move.jump += 10;
                            character[i].y -= move.jump;
                            if (move.jump >= 20) {
                                move.jump_status = false;
                            }
                        }
                        else if (move.jump != -1 && !move.jump_status) {
                            move.jump += 10;
                            character[i].y += move.jump;
                            if (move.jump >= 30) {
                                move.jump = -1;
                                move.jump_status = false;

                                if (character[i].move_status == 5) {
                                    character[i].move_status = 1;
                                    move.left = true;
                                }
                                else if (character[i].move_status == 4) {
                                    character[i].move_status = 0;
                                    move.right = true;
                                }

                            }
                        }
                    }
                }
            }
else {
  
    for (int i = 0; i < character.size(); ++i) {
        Move& move = character[i].move;
        if (move.down || move.left || move.up || move.right) {
            character[i].select_img++;
            if (character[i].select_img == 4) {
                character[i].select_img = 0;
            }

            if (move.down) {
                character[i].y += 7;
            }

            if (move.left) {
                character[i].x -= 7;
            }

            if (move.up) {
                character[i].y -= 7;
            }

            if (move.right) {
                character[i].x += 7;
            }

        }

        if (move.jump != -1 && move.jump_status) {
            move.jump += 10;
            character[i].y -= move.jump;
            if (move.jump >= 20) {
                move.jump_status = false;
            }
        }
        else if (move.jump != -1 && !move.jump_status) {
            move.jump += 10;
            character[i].y += move.jump;
            if (move.jump >= 30) {
                move.jump = -1;
                move.jump_status = false;

                if (character[i].move_status == 5) {
                    character[i].move_status = 1;
                    move.left = true;
                }
                else if (character[i].move_status == 4) {
                    character[i].move_status = 0;
                    move.right = true;
                }

            }
        }
    }
}

            
            

            break;
        }
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