#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <objidl.h>
#include <gdiplus.h>
#include <dshow.h>
#include <thread>

#pragma comment(lib, "strmiids.lib")
#pragma comment (lib,"Gdiplus.lib")

using namespace std;
using namespace Gdiplus;

ULONG_PTR gdiplusToken;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"INVERSUS";
LPCTSTR lpszWindowName = L"INVERSUS";

#define WIDTH 1200
#define HEIGHT 800

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);



void MonitorMediaEvent();
void MonitorSecondMediaEvent();
void game_setting(bool game_status, WPARAM wParam, Image*& pImage, int& player_num, bool start);


// GDI+ �ʱ�ȭ
void InitGDIPlus() {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

// GDI+ ����
void ShutdownGDIPlus() {
    GdiplusShutdown(gdiplusToken);
}
// DirectShow �������̽�
IGraphBuilder* pGraph = NULL;
IMediaControl* pControl = NULL;
IMediaEvent* pEvent = NULL;
IBasicAudio* pBasicAudio = NULL;

// �� ��° ����� ������ ���� DirectShow �������̽�
IGraphBuilder* pGraph2 = NULL;
IMediaControl* pControl2 = NULL;
IMediaEvent* pEvent2 = NULL;
IBasicAudio* pBasicAudio2 = NULL;

void PlayMP3(const WCHAR* filename) {
    CoInitialize(NULL);
    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);
    pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
    pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);
    pGraph->QueryInterface(IID_IBasicAudio, (void**)&pBasicAudio);

    BSTR bstrFilename = SysAllocString(filename);
    pControl->RenderFile(bstrFilename);
    SysFreeString(bstrFilename);
    pControl->Run();

    if (pBasicAudio != NULL) {
        pBasicAudio->put_Volume(-2500); // ���� ����
    }

    // ������ �����峪 Ÿ�̸Ӹ� ����Ͽ� �̺�Ʈ ����͸�
    std::thread eventMonitor(MonitorMediaEvent);
    eventMonitor.detach(); // ��׶��忡�� ����
}

void PlayMP3Close() {
    if (pControl) pControl->Release();
    if (pGraph) pGraph->Release();
    if (pEvent) pEvent->Release();
    if (pBasicAudio) pBasicAudio->Release();
    CoUninitialize();
}

void MonitorMediaEvent() {
    long evCode;
    LONG_PTR param1, param2;
    while (pEvent->GetEvent(&evCode, &param1, &param2, 0) == S_OK) {
        pEvent->FreeEventParams(evCode, param1, param2);
        if (evCode == EC_COMPLETE) {
            // �̵�� ��� �Ϸ� �̺�Ʈ ó��
            PlayMP3Close(); // ���ҽ� ���� �Լ� ȣ��
            break;
        }
    }
}

void PlaySecondMP3(const WCHAR* filename) {
    CoInitialize(NULL);
    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph2);
    pGraph2->QueryInterface(IID_IMediaControl, (void**)&pControl2);
    pGraph2->QueryInterface(IID_IMediaEvent, (void**)&pEvent2);
    pGraph2->QueryInterface(IID_IBasicAudio, (void**)&pBasicAudio2);

    BSTR bstrFilename = SysAllocString(filename);
    pControl2->RenderFile(bstrFilename);
    SysFreeString(bstrFilename);
    pControl2->Run();

    if (pBasicAudio2 != NULL) {
        pBasicAudio2->put_Volume(0); // ���� ����
    }

    // ������ �����忡�� �̺�Ʈ ����͸�
    std::thread eventMonitor(MonitorSecondMediaEvent);
    eventMonitor.detach(); // ��׶��忡�� ����
}

void CloseSecondMP3() {
    if (pControl2) pControl2->Release();
    if (pGraph2) pGraph2->Release();
    if (pEvent2) pEvent2->Release();
    if (pBasicAudio2) pBasicAudio2->Release();
    CoUninitialize();
}

void MonitorSecondMediaEvent() {
    long evCode;
    LONG_PTR param1, param2;
    while (pEvent2->GetEvent(&evCode, &param1, &param2, 0) == S_OK) {
        pEvent2->FreeEventParams(evCode, param1, param2);
        if (evCode == EC_COMPLETE) {
            // �̵�� ��� �Ϸ� �̺�Ʈ ó��
            CloseSecondMP3(); // ���ҽ� ���� �Լ� ȣ��
            break;
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {

    InitGDIPlus();

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

    // ��ü ȭ�� ũ�⸦ �����ɴϴ�.
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // ��üȭ�� ��Ÿ�Ϸ� â�� �����մϴ�.
    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_POPUP | WS_VISIBLE,
        0, 0, screenWidth, screenHeight, NULL, (HMENU)NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    //ShutdownGDIPlus();
    return Message.wParam;

}

#define WIDTH_LINE 40
#define HEGHIT_LINE 40


random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap, introImgBitMap;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static int Timer1Count = 0;
    static int cellSizeX;
    static int cellSizeY;
    static Image* pImage = nullptr;  // ���� �̹��� ������
    static bool start = true;
    static int player_num = -1;
    static bool game_status = false;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        pImage = new Image(L"Inversus Intro.png");
        PlayMP3(L"main intro.mp3"); // ��ο� �ִ� MP3 ���� ���

        break;
    }
    case WM_COMMAND:
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // Ű���� Ű�� ������ ��

        if (start && wParam == VK_RETURN) {
            PlaySecondMP3(L"button sound.MP3"); // ��ư ����
            delete pImage;
            player_num = 0;
            pImage = new Image(L"img/player_0.png"); // �� �̹��� �ε�
            start = false;
        }

        game_setting(game_status, wParam, pImage, player_num, start);
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_LBUTTONDOWN:
        break;
    case WM_LBUTTONUP:
        break;
    case WM_RBUTTONDOWN:
        break;
    case WM_RBUTTONUP:
        break;
    case WM_MOUSEMOVE:
        break;
    case WM_CHAR:
        switch (wParam)
        {
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
        FillRect(mDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

        cellSizeX = rect.right / WIDTH_LINE;
        cellSizeY = rect.bottom / HEGHIT_LINE;

        
        if (start) { // game start
            //intro img
            Graphics graphics(hDC);
            graphics.DrawImage(pImage, 0, 0, pImage->GetWidth(), pImage->GetHeight());
        }
        else {
            Graphics graphics(hDC);
            graphics.DrawImage(pImage, 0, 0, pImage->GetWidth(), pImage->GetHeight());
        }


        /*for (int y = 0; y <= WIDTH_LINE; ++y) {
            MoveToEx(mDC, y * cellSizeX, 0, NULL);
            LineTo(mDC, y * cellSizeX, WIDTH_LINE * cellSizeY);
        }

        for (int x = 0; x <= HEGHIT_LINE; ++x) {
            MoveToEx(mDC, 0, x * cellSizeY, NULL);
            LineTo(mDC, HEGHIT_LINE * cellSizeX, x * cellSizeY);
        }*/

        //BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);

        DeleteObject(hBitmap); // ������ ��Ʈ�� ����
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_DESTROY:
        delete pImage;  // �̹��� �ڿ� ����
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}