#include <Windows.h>
#include <tchar.h>
#include <random>
#include <atlimage.h>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"INVERSUS";
LPCTSTR lpszWindowName = L"INVERSUS";

#define WIDTH 900
#define HEIGHT 800
#define SCROLL_SPEED 1 // Speed of the scrolling
#define SCROLL_SPEED2 3 // Speed of the scrolling


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
    HWND hWnd;
    MSG Message;
    WNDCLASSEX WndClass;
    g_hInst = hInstance;

    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
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
        20, 20, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, NULL, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return static_cast<int>(Message.wParam);
}


class Explosion {
public:
    RECT rect;
    COLORREF color;
    int frame;

    Explosion(RECT r, COLORREF c) : rect(r), color(c), frame(0) {}

    bool update() {
        ++frame;
        return frame < 9; 
    }

    void draw(HDC& mDC, const Explosion& ex) const {
        HBRUSH brush = CreateSolidBrush(ex.color);
        HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, brush);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distX(ex.rect.left, ex.rect.right);
        std::uniform_int_distribution<int> distY(ex.rect.top, ex.rect.bottom);
        std::uniform_int_distribution<int> distSize(20, 50);

        
        for (int i = 0; i < 10; ++i) { 
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
    bool status = false;
    RECT rect;
    COLORREF color = RGB(255, 0, 0);
    void draw(HDC& mDC) {
        HBRUSH hBrush = CreateSolidBrush(color);
        HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

        Ellipse(mDC, rect.left, rect.top, rect.right, rect.bottom);

        SelectObject(mDC, oldBrush);
        DeleteObject(hBrush);
    }
};

struct Circle {
    bool left;
    bool right;
    RECT rect;
    COLORREF color;
    int shadow = 10;
    bool jump = false;
    int speed = 3;

    void draw_sub(HDC& mDC) {
        HBRUSH hBrush = CreateSolidBrush(color);
        HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

        Ellipse(mDC, rect.left, rect.top, rect.right, rect.bottom);

        SelectObject(mDC, oldBrush);
        DeleteObject(hBrush);
    }
    void draw(HDC& mDC) {
        HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 200));
        HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

        Ellipse(mDC, rect.left, rect.top, rect.right, rect.bottom);

        SelectObject(mDC, oldBrush);
        DeleteObject(hBrush);
    }

    void draw_shadow(HDC& mDC) {
        // Creating a semi-transparent brush for the shadow
        HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
        HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

        // To remove the outline, we need to set the pen to NULL
        HPEN hPen = (HPEN)SelectObject(mDC, GetStockObject(NULL_PEN));

        Ellipse(mDC, rect.left + shadow, rect.top + shadow, rect.right + shadow, rect.bottom + shadow);

        SelectObject(mDC, hPen);
        SelectObject(mDC, oldBrush);
        DeleteObject(hBrush);
    }
};



random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_sub(1, 5);
uniform_int_distribution<int> uid_speed(3, 7);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static int scrollPos = 0;
    static int scrollPos2 = 0;
    static RECT rect;
    static RECT mapRECT;
    static CImage backImage;

    static CImage backImage2;

    static Circle main;
    static int jumpCnt = 0;

    static vector<Circle> subs;

    static vector<Bullet> bullets;

    static vector<Explosion> explodes;

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HBITMAP hOldBitmap;

    switch (uMsg) {
    case WM_CREATE:

        GetClientRect(hWnd, &rect);

        if (backImage.Load(L"1.png") != S_OK) {
            MessageBox(hWnd, L"Failed to load image", L"Error", MB_OK | MB_ICONERROR);
            PostQuitMessage(1);
        }
        if (backImage2.Load(L"2.png") != S_OK) {
            MessageBox(hWnd, L"Failed to load image", L"Error", MB_OK | MB_ICONERROR);
            PostQuitMessage(1);
        }

        mapRECT = rect;
        mapRECT.left += 100;
        mapRECT.right -= 100;

        SetTimer(hWnd, 1, 1, NULL);

        main.rect = { 0,0,50,50 };
        OffsetRect(&main.rect, mapRECT.right / 2 + 25, mapRECT.bottom - 100);

        break;
    case WM_KEYUP:
        switch (wParam)
        {
        case VK_LEFT:
            main.left = false;
            break;
        case VK_RIGHT:
            main.right = false;
            break;
        case VK_UP:
            main.jump = false;
            break;
        default:
            break;
        }
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
            main.left = true;
            break;
        case VK_RIGHT:
            main.right = true;
            break;
        case VK_SPACE:
        {
            Bullet bullet;

            bullet.rect = main.rect;
            InflateRect(&bullet.rect, -15, -15);
            bullet.status = true;
            bullets.push_back(bullet);
            break;
        }
        case VK_UP:
            main.jump = true;
            break;
        default:
            break;
        }
        break;
    case WM_PAINT: {
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        hOldBitmap = static_cast<HBITMAP>(SelectObject(mDC, hBitmap));

        FillRect(mDC, &rect, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));

        int imageHeight = backImage.GetHeight();
        int yOffset = scrollPos % imageHeight;

        int imageHeight2 = backImage2.GetHeight();
        int yOffset2 = scrollPos2 % imageHeight;

        // Draw the image twice to create a continuous scrolling effect
        backImage.Draw(mDC, 0, yOffset - imageHeight, rect.right, imageHeight);
        backImage.Draw(mDC, 0, yOffset, rect.right, imageHeight);

        // Draw the image twice to create a continuous scrolling effect
        backImage2.Draw(mDC, 50, yOffset2 - imageHeight2, rect.right - 100, imageHeight2);
        backImage2.Draw(mDC, 50, yOffset2, rect.right - 100, imageHeight2);

        for (auto& bullet : bullets) {
            bullet.draw(mDC);
        }

        main.draw_shadow(mDC);

        for (auto& sub : subs) {
            sub.draw_sub(mDC);
        }

        main.draw(mDC);

        for (const auto& explosion : explodes) {
            explosion.draw(mDC, explosion);
        }

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);

        SelectObject(mDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        switch (wParam)
        {
        case 1:
            scrollPos += SCROLL_SPEED; // Update the scroll position
            scrollPos2 += SCROLL_SPEED2; // Update the scroll position

            if (main.left) {
                OffsetRect(&main.rect, -3, 0);
                RECT r;
                if (!IntersectRect(&r, &main.rect, &mapRECT)) {
                    OffsetRect(&main.rect, 3, 0);
                }
            }

            if (main.right) {
                OffsetRect(&main.rect, 3, 0);
                RECT r;
                if (!IntersectRect(&r, &main.rect, &mapRECT)) {
                    OffsetRect(&main.rect, -3, 0);
                }
            }

            for (auto& bullet : bullets) {
                OffsetRect(&bullet.rect, 0, -3);
            }

            if (main.jump) {
                if (!(jumpCnt >= 5)) {
                    jumpCnt++;
                    if (jumpCnt <= 5) {
                        InflateRect(&main.rect, jumpCnt, jumpCnt);
                    }
                }
            }

            if (!main.jump) {
                if (jumpCnt != 0) {
                    if (jumpCnt >= 0) {
                        InflateRect(&main.rect, -jumpCnt, -jumpCnt);
                    }
                    jumpCnt--;
                }
            }

            if (subs.size() == 0) {
                Circle c;
                int n = uid_sub(gen);
                for (int i = 0; i < n; ++i) {
                    c.speed = uid_speed(gen);
                    c.color = RGB(0, 0, 255);
                    c.rect = { 0,0,50,50 };
                    OffsetRect(&c.rect, mapRECT.left + (i * 150), 30);
                    subs.push_back(c);
                }
            }
            else {
                for (int i = subs.size()-1; i >= 0; --i) {
                    OffsetRect(&subs[i].rect, 0, subs[i].speed);
                    if (subs[i].rect.top > main.rect.bottom) {
                        subs.erase(subs.begin() + i);
                    }
                }

                for (int i = subs.size() - 1; i >= 0; --i) {
                    RECT r;
                    if (IntersectRect(&r, &subs[i].rect, &main.rect) && !main.jump) {
                        Explosion e(main.rect, main.color);
                        explodes.push_back(e);
                        subs.erase(subs.begin() + i);
                        main.rect = { 0,0,50,50 };
                        OffsetRect(&main.rect, mapRECT.right / 2 + 25, mapRECT.bottom - 100);
                    }
                }

                for (auto i = subs.rbegin(); i != subs.rend(); ++i) {
                    bool sub_erased = false;
                    for (auto j = bullets.rbegin(); j != bullets.rend(); ++j) {
                        RECT r;
                        if (IntersectRect(&r, &i->rect, &j->rect)) {
                            Explosion e(i->rect, i->color);
                            explodes.push_back(e);
                            bullets.erase(next(j).base());
                            subs.erase(next(i).base());
                            sub_erased = true;
                            break;
                        }
                    }
                    if (sub_erased) break; // break the outer loop to avoid iterator invalidation
                }
            }
            

            for (auto it = explodes.begin(); it != explodes.end(); ) {
                if (!it->update()) {
                    if (explodes.size() >= 1) {
                        it = explodes.erase(it);
                    }
                }
                else {
                    ++it;
                }
            }

            break;
        default:
            break;
        }

        InvalidateRect(hWnd, NULL, FALSE); // Force a repaint
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1); // Destroy the timer
        backImage.Destroy(); // Clean up the image object
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}
