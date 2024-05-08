#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <numeric>
#include "resource.h"
#include<atlimage.h>

using namespace std;

HINSTANCE g_hInst;

LPCTSTR lpszClass = L"실습 5-2";
LPCTSTR lpszWindowName = L"실습 5-2";

#define WIDTH 1000
#define HEIGHT 900

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

    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_VSCROLL, 200, 0, WIDTH, HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return Message.wParam;

}
struct img_s {
    RECT rect;
    RECT imgSize;
};

void imgSet(int img_count, CImage img[2], int img_status, vector<img_s>& imgs, RECT rect) {
    int cell_width = rect.right / img_count;
    int cell_height = rect.bottom / img_count;

    int imgW = img[img_status].GetWidth();
    int imgH = img[img_status].GetHeight();
    int img_width = imgW / img_count;
    int img_height = imgH / img_count;

    // 조각의 인덱스를 무작위로 섞기 위한 벡터
    vector<int> indices(img_count * img_count);
    iota(indices.begin(), indices.end(), 0);
    shuffle(indices.begin(), indices.end(), mt19937{ random_device{}() });

    // 섞인 인덱스를 사용하여 각 조각의 화면 위치와 원본 영역을 설정
    for (int i = 0; i < img_count; ++i) {
        for (int j = 0; j < img_count; ++j) {
            int index = indices[i * img_count + j];
            int originalRow = index / img_count;
            int originalCol = index % img_count;

            img_s i_s;
            // 원본 이미지에서 가져올 영역 설정
            i_s.imgSize.left = img_width * originalCol;
            i_s.imgSize.top = img_height * originalRow;
            i_s.imgSize.right = img_width * (originalCol + 1);
            i_s.imgSize.bottom = img_height * (originalRow + 1);

            // 화면에 조각을 배치할 위치 설정
            i_s.rect.left = cell_width * j;
            i_s.rect.top = cell_height * i;
            i_s.rect.right = cell_width * (j + 1);
            i_s.rect.bottom = cell_height * (i + 1);

            imgs.push_back(i_s);
        }
    }
}

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
    static CImage img[2];
    static int img_status = 0;
    static int img_count = 3;
    static vector<img_s> imgs;
    static bool game_status = false;
    static bool img_set_status = true;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        img[0].Load(L"dog1.jpeg");
        img[1].Load(L"dog2.jpeg");

        break;
    }
    case WM_COMMAND:
        switch (wParam)
        {
        case ID_40001:
            img_status = 0;
            break;
        case ID_40002:
            img_status = 1;
        case ID_40006:
            game_status = true;
            break;
        case ID_40009:
            PostQuitMessage(0);
            break;
        case ID_40007: //전체 그림 보기
        {
            img_set_status = false;
            imgs.clear();
            int cell_width = rect.right / img_count;
            int cell_height = rect.bottom / img_count;

            int imgW = img[img_status].GetWidth();
            int imgH = img[img_status].GetHeight();

            int img_width = imgW / img_count;
            int img_height = imgH / img_count;

            // 섞인 인덱스를 사용하여 각 조각의 화면 위치와 원본 영역을 설정
            for (int i = 0; i < img_count; ++i) {
                for (int j = 0; j < img_count; ++j) {

                    img_s i_s;
                    // 원본 이미지에서 가져올 영역 설정
                    i_s.imgSize.left = img_width * j;
                    i_s.imgSize.top = img_height * i;
                    i_s.imgSize.right = img_width * (j + 1);
                    i_s.imgSize.bottom = img_height * (i + 1);

                    // 화면에 조각을 배치할 위치 설정
                    i_s.rect.left = cell_width * j;
                    i_s.rect.top = cell_height * i;
                    i_s.rect.right = cell_width * (j + 1);
                    i_s.rect.bottom = cell_height * (i + 1);

                    imgs.push_back(i_s);
                }
            }
        }
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_LBUTTONDOWN:
    {
        
        InvalidateRect(hWnd, NULL, false);
        break;
    }
    case WM_CHAR:
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_PAINT:
    {
        GetClientRect(hWnd, &rect);
        hDC = BeginPaint(hWnd, &ps);
        mDC1 = CreateCompatibleDC(hDC);
        hBit1 = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        oldBit1 = (HBITMAP)SelectObject(mDC1, hBit1);

        // 배경을 흰색으로 채움
        FillRect(mDC1, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

        if (game_status) {
            if (img_set_status)
                imgSet(img_count, img, img_status, imgs, rect);
            img_set_status = true;

            for (const auto& imgPart : imgs) {
                img[img_status].StretchBlt(mDC1, imgPart.rect.left, imgPart.rect.top,
                    imgPart.rect.right - imgPart.rect.left, imgPart.rect.bottom - imgPart.rect.top,
                    imgPart.imgSize.left, imgPart.imgSize.top,
                    imgPart.imgSize.right - imgPart.imgSize.left, imgPart.imgSize.bottom - imgPart.imgSize.top,
                    SRCCOPY);
            }
        }
        
        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC1, 0, 0, SRCCOPY);

        // 로드된 비트맵을 선택
        SelectObject(mDC1, oldBit1);
        DeleteDC(mDC1);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_DESTROY:
        DeleteObject(hBit1);
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
