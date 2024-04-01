#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 2-7번";
LPCTSTR lpszWindowName = L"실습 2-7번";

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
uniform_int_distribution<int> uidX(0, 600);

//void keyArrow(HDC hDC, PAINTSTRUCT &ps, WPARAM wParam, HWND hWnd, static TCHAR str[11][32], int &cnt_x, int &cnt_y, int &caret_x, int &caret_y, SIZE size);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hDC;
    static TCHAR lpOut[100];
    static RECT rect;
    static SIZE size;
    static TCHAR str[11][32];
    static int cnt_x;
    static int cnt_y;
    int size_tap = 5;
    TCHAR ch_n; // 필요할때쓰는거.


    //caret좌표
    static int caret_x;
    static int caret_y;

    //LPCWSTR 문자 하나 보관
    static TCHAR i_ch[32];

    // 대소문자 f1
    static bool f1_status = false;

    // insert
    static bool in_status = false;

    static bool f2_status = false;
    
    switch (uMsg)
    {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        cnt_x = 0;
        cnt_y = 0;
        caret_x = 0;
        caret_y = 0;
        break;
    case WM_CHAR:

        //----------------------------
        switch (wParam)
        {
        case VK_BACK:
            if (cnt_x == 0 && cnt_y == 0 || caret_x == 0 && caret_y == 0) //처음 부터 뒤로가는거 막기
                break;

            if (lstrlen(str[caret_y]) == 0) {
                caret_y--;
                cnt_y--;
                caret_x = lstrlen(str[caret_y]);
                cnt_x = lstrlen(str[caret_y]);
                for (int i = 0; i < lstrlen(str[caret_y + 1]); ++i) { // 한줄만 이제 뒤에 붙이고
                    str[caret_y][caret_x + i] = str[caret_y + 1][i];
                    cnt_x++;
                }

                for (int i = caret_y + 1; i < sizeof(str) / sizeof(str[0]); ++i) { // 그 뒤부터는 위로 붙이고
                    str[i][0] = '\0';
                    memcpy(str[i], str[i + 1], sizeof(str[i + 1]));
                }

                break;

            }

            if (lstrlen(str[caret_y]) != 0 && caret_x == 0) { // 맨앞에서 지웠는데 뒤에 데이터가 남아있으면 윗줄로 가게
                caret_y--;
                cnt_y--;
                caret_x = lstrlen(str[caret_y]);
                for (int i = 0; i < lstrlen(str[caret_y + 1]); ++i) { // 한줄만 이제 뒤에 붙이고
                    str[caret_y][caret_x + i] = str[caret_y + 1][i];
                    cnt_x++;
                }

                for (int i = caret_y + 1; i < sizeof(str) / sizeof(str[0]); ++i) { // 그 뒤부터는 위로 붙이고
                    str[i][0] = '\0';
                    /*for (int j = 0; j < lstrlen(str[i + 1]); ++j) {
                        str[i][j] = str[i + 1][j];
                    }*/
                    memcpy(str[i], str[i + 1], sizeof(str[i + 1]));
                }

                break;
            }

            for (int i = caret_x; i < lstrlen(str[caret_y]); ++i) {
                str[caret_y][i - 1] = str[caret_y][i];
            }

            caret_x--;

            str[caret_y][lstrlen(str[caret_y]) - 1] = '\0';

            InvalidateRect(hWnd, NULL, TRUE);

            break;
        case VK_RETURN:

            if (caret_x != cnt_x && caret_y != cnt_y)
                break;

            if (cnt_y < 9) {
                cnt_y++;
                cnt_x = 0;

                caret_y++;
                caret_x = 0;
            }
            break;
        case VK_ESCAPE:
            for (size_t i = 0; i < 10; ++i) {
                for (size_t j = 0; j < lstrlen(str[i]); ++j) {
                    str[i][j] = '\0';
                }
            }
            cnt_x = 0;
            cnt_y = 0;

            caret_x = 0;
            caret_y = 0;
            break;
        case VK_TAB:
            if (30 - cnt_x < 5) { // tap이 끝에 닿았을때 더 이상 못가게 막기
                size_tap = 30 - cnt_x;
                for (size_t i = 0; i < size_tap; ++i) {
                    str[cnt_y][cnt_x++] = ' ';
                    caret_x++;
                }
            }
            else {
                for (size_t i = 0; i < size_tap; ++i) {
                    str[cnt_y][cnt_x++] = ' ';
                    caret_x++;
                }
            }
            break;
        default:


            if (f1_status)
                wParam = (WPARAM)CharUpper((LPTSTR)(DWORD_PTR)wParam); //대문자로 변형
            else
                wParam = (WPARAM)CharLower((LPTSTR)(DWORD_PTR)wParam); //소문자로

            if (in_status) {  //insert 상태에따라 넣는거 다르게

                if (cnt_x > 29 && cnt_y < 9) {
                    cnt_y++;
                    cnt_x = 0;
                }

                if (lstrlen(str[caret_y]) > 29 && cnt_y < 9) {
                    //str[caret_y + 1][cnt_x] = str[caret_y][lstrlen(str[caret_y])-1];
                     //str[caret_y][lstrlen(str[caret_y]) - 1] = '\0';

                     //여기서 값 바꿀꺼 해야댐 아니면 그냥 막던가.
                    break;
                }

                if (caret_x == cnt_x && caret_y == cnt_y) {
                    str[cnt_y][cnt_x++] = wParam;
                    caret_x++;
                }
                else {
                    for (int i = lstrlen(str[caret_y]); i > caret_x - 1; --i) {
                        str[caret_y][i + 1] = str[caret_y][i];
                    }
                    str[caret_y][caret_x++] = wParam;
                    cnt_x++;
                }
            }
            else {

                if (cnt_x > 29 && cnt_y < 9) {
                    cnt_y++;
                    cnt_x = 0;
                }

                if (caret_x > 29 && caret_y < 9) {
                    caret_y++;
                    caret_x = 0;
                }

                if (caret_x == 30 && caret_y == 9) {
                    break;
                }

                if (caret_x == cnt_x && caret_y == cnt_y) {
                    str[cnt_y][cnt_x++] = wParam; // 실질적으로 입력되는곳.
                    caret_x++;
                }
                else {
                    str[caret_y][caret_x++] = wParam;
                }
            }

            break;
        }

        str[cnt_y][cnt_x] = '\0';
        InvalidateRect(hWnd, NULL, TRUE);
        break;
        //----------------------------
    case WM_PAINT:

        hDC = BeginPaint(hWnd, &ps);

        for (size_t i = 0; i < cnt_y + 1; ++i) {
            TextOut(hDC, 0, DEFAULT_COL * i, str[i], lstrlen(str[i]));// 입력 받은거 출력
        }


        for (int i = 0; i < caret_x; ++i) {
            i_ch[i] = str[caret_y][i];
        }

        GetTextExtentPoint32(hDC, i_ch, lstrlen(i_ch), &size);

        memset(i_ch, 0, sizeof(i_ch));
        SetCaretPos((size.cx), caret_y * DEFAULT_COL); //caret 위치 표시
        ShowCaret(hWnd);
        EndPaint(hWnd, &ps);
        ReleaseDC(hWnd, hDC);
        break;
    case WM_DESTROY:
        HideCaret(hWnd);
        DestroyCaret();
        PostQuitMessage(0);
        break;
    default:
        break;
    }


    if (uMsg == WM_KEYDOWN) {
        //key arrow
        switch (wParam)
        {
        case VK_LEFT:
            if (caret_x == 0)
                break;
            caret_x -= 1;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_RIGHT:
            if (lstrlen(str[caret_y]) == caret_x)
                break;
            caret_x += 1;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_UP:
            if (caret_y == 0)
                break;

            caret_y -= 1;

            if (lstrlen(str[caret_y]) < caret_x) // 올라갈때 값 크기 비교
                caret_x = lstrlen(str[caret_y]);

            if (caret_x == 0) // 아무것도 입력안된줄은 올라가면 마지막 쪽으로 가게함
                caret_x = lstrlen(str[caret_y]);


            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_DOWN:
            if (caret_y == cnt_y)
                break;
            caret_y += 1;

            if (lstrlen(str[caret_y]) < caret_x) // 올라갈때 값 크기 비교
                caret_x = lstrlen(str[caret_y]);

            if (caret_x == 0) // 아무것도 입력안된줄은 올라가면 마지막 쪽으로 가게함
                caret_x = lstrlen(str[caret_y]);

            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_F1: // f1 대소문자 변경
            f1_status = !f1_status;
            break;
        case VK_DELETE:

            if (caret_x == 30 && caret_y == 9)
                break;

            if (caret_x == 0 && lstrlen(str[caret_y]) == 0) {

                for (int i = 0; i < lstrlen(str[caret_y + 1]); ++i) { // 한줄만 이제 뒤에 붙이고
                    str[caret_y][caret_x + i] = str[caret_y + 1][i];
                    cnt_x++;
                }

                for (int i = caret_y + 1; i < cnt_y + 1; ++i) { // 그 뒤부터는 위로 붙이고
                    str[i][0] = '\0';
                    memcpy(str[i], str[i + 1], sizeof(str[i + 1]));
                }
                if (cnt_y != caret_y)
                    cnt_y--;
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            }


            for (int i = caret_x; i < lstrlen(str[caret_y]); ++i) {
                str[caret_y][i] = str[caret_y][i + 1];
            }

            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_HOME:
            caret_x = 0;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_END:
            caret_x = lstrlen(str[caret_y]);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_INSERT:
            in_status = !in_status;
            if (in_status) {
                HideCaret(hWnd);
                CreateCaret(hWnd, NULL, 2, 15);
                ShowCaret(hWnd);
            }
            else {
                HideCaret(hWnd);
                CreateCaret(hWnd, NULL, 5, 15);
                ShowCaret(hWnd);
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_F2:
            f2_status = !f2_status;

            if (f2_status) {

                for (int i = 0; i < sizeof(str) / sizeof(str[0]); ++i) {
                    for (int j = lstrlen(str[i]); j > 0; --j) {
                        str[i][j + 1] = str[i][j];
                    }
                }

            }
            else {

            }
            break;
        default:
            break;
        }

    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}