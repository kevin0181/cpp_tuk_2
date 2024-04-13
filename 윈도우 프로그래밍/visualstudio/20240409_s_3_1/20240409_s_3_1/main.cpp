#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 3-1";
LPCTSTR lpszWindowName = L"실습 3-1";

#define WIDTH 900
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


#define gridSize 40

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uid_ran_4(0, 3);
uniform_int_distribution<int> uid_ran_3(1, 3);
uniform_int_distribution<int> uid_position(0, 39);
struct shape {
    int positionX;
    int positionY;
    HBRUSH mBrush, oldBrush;
    RECT circleRect;
    int cellSizeX;
    int cellSizeY;
    int move_i;
    COLORREF color = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
    bool m1_status = false;
    bool hero_lineX_status = false;
    bool hero_lineY_status = false;
    bool shape_status; //삼각형 or 원
    int move_R_i{ 0 }; // 네모 계산
    int move_count = 0; //네모 움직이는 횟수
    bool view_status = true;
    bool ready_status;
    bool isWorm = false; // 이것이 먹이 지렁이인지 나타내는 플래그

    void print_circle_shape(HDC& mDC) {

        circleRect.left = 0;
        circleRect.top = 0;
        circleRect.right = cellSizeX;
        circleRect.bottom = cellSizeY;

        circleRect.left += (positionX * cellSizeX);
        circleRect.right += (positionX * cellSizeX);
        circleRect.top += (positionY * cellSizeY);
        circleRect.bottom += (positionY * cellSizeY);

        mBrush = CreateSolidBrush(color);
        oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
        Ellipse(mDC, circleRect.left, circleRect.top, circleRect.right, circleRect.bottom);
        DeleteObject(mBrush);
    }

    void print_s(HDC& mDC) {
        if (shape_status) {
            print_circle_shape(mDC);
        }
        else {
            print_food_shape(mDC);
        }
    }

    void print_food_shape(HDC& mDC) {
        circleRect.left = 0;
        circleRect.top = 0;
        circleRect.right = cellSizeX;
        circleRect.bottom = cellSizeY;

        circleRect.left += (positionX * cellSizeX) + 2;
        circleRect.top += (positionY * cellSizeY) + 2;

        circleRect.right += (positionX * cellSizeX) - 2;
        circleRect.bottom += (positionY * cellSizeY) - 2;

        mBrush = CreateSolidBrush(color);
        oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
        Rectangle(mDC, circleRect.left, circleRect.top, circleRect.right, circleRect.bottom);
        DeleteObject(mBrush);
    }

};


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    int cellSizeX;
    int cellSizeY;
    static bool start_status = false;
    static int Timer1Count = 0;
    static int Timer2Count = 0;
    static int Timer3Count = 0;
    static int timerInterval = 50; // 초기 타이머 간격을 100ms로 설정

    static int key_status;
    static vector<shape> food_shapes;
    static vector<shape> hero_shapes;
    static vector<shape> black_rock;
    static RECT crossRect;

    static int x_mouse;
    static int y_mouse;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        key_status = 3;
        SetTimer(hWnd, 2, 50, NULL);
        // SetTimer(hWnd, 3, timerInterval, NULL);
        shape shape1;

        shape1.positionX = 0;
        shape1.positionY = 0;
        shape1.color = RGB(255, 0, 0);
        shape1.shape_status = true;
        hero_shapes.push_back(shape1);

        for (int i = 0; i < 20; ++i) {

           /* shape1.move_i = uid_ran_4(gen);
            shape1.shape_status = true;*/

            shape1.positionX = uid_position(gen);
            shape1.positionY = uid_position(gen);
            shape1.shape_status = false;
            shape1.color = RGB(uid_RGB(gen), uid_RGB(gen), uid_RGB(gen));
            food_shapes.push_back(shape1);
        }

    }
    break;
    case WM_RBUTTONDOWN:
    {
        if (black_rock.size() < 20)
        {
            shape shape1;
            shape1.positionX = uid_position(gen);
            shape1.positionY = uid_position(gen);
            shape1.shape_status = false;
            shape1.color = RGB(0, 0, 0);
            black_rock.push_back(shape1);
        }
    }
        break;
    case WM_LBUTTONDOWN:
    {
        x_mouse = LOWORD(lParam);
        y_mouse = HIWORD(lParam);

        POINT pt;
        pt.x = x_mouse;
        pt.y = y_mouse;
        for (int i = 1; i < hero_shapes.size(); ++i) {
            if (PtInRect(&hero_shapes[i].circleRect, pt)) {
                // 마우스 클릭으로 선택된 세그먼트부터 끝까지 반복
                while (i < hero_shapes.size()) {
                    // 현재 세그먼트를 food_shapes로 이동
                    hero_shapes[i].view_status = true; // 보이도록 설정
                    hero_shapes[i].move_i = uid_ran_4(gen); // 무작위 방향 설정
                    hero_shapes[i].shape_status = true; // 형태 상태 설정
                    hero_shapes[i].ready_status = true; // 준비 상태 설정
                    food_shapes.push_back(hero_shapes[i]); // food_shapes에 추가

                    // hero_shapes에서 현재 세그먼트 제거
                    hero_shapes.erase(hero_shapes.begin() + i); // i번째 세그먼트를 제거
                    // 이 경우 i를 증가시키지 않음으로써 다음 세그먼트가 현재 인덱스에 올 것임
                }
                break; // 세그먼트 제거 후 반복문 종료
            }
        }

        int x = x_mouse - hero_shapes[0].circleRect.left;
        int y = y_mouse - hero_shapes[0].circleRect.top;

        if (y < 0) {
            key_status = 0;
        }
        else {
            key_status = 1;
        }

        if (x < 0) {
            key_status = 2;
        }
        else {
            key_status = 3;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    }
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때
        switch (wParam) {
        case VK_UP:    // 위쪽 화살표 키
            // 위쪽 키가 눌렸을 때 실행할 코드
            key_status = 0;
            break;
        case VK_DOWN:  // 아래쪽 화살표 키
            // 아래쪽 키가 눌렸을 때 실행할 코드
            key_status = 1;
            break;
        case VK_LEFT:  // 왼쪽 화살표 키
            // 왼쪽 키가 눌렸을 때 실행할 코드
            key_status = 2;
            break;
        case VK_RIGHT: // 오른쪽 화살표 키
            // 오른쪽 키가 눌렸을 때 실행할 코드
            key_status = 3;
            break;
        default:
            break;
        }
        break;
    case WM_CHAR:

        switch (wParam)
        {
        case  's': //start
            start_status = !start_status;
            SetTimer(hWnd, 1, timerInterval, NULL);
            break;
        case '=':
            // 속도 증가: 간격 감소
            timerInterval = max(10, timerInterval - 10); // 최소 간격은 10ms로 제한
            SetTimer(hWnd, 1, timerInterval, NULL); // 타이머 재설정
            break;
        case '-':
            // 속도 감소: 간격 증가
            timerInterval = min(1000, timerInterval + 10); // 최대 간격은 1000ms로 제한
            SetTimer(hWnd, 1, timerInterval, NULL); // 타이머 재설정
            break;
        case 'j':
            switch (key_status)
            {
            case 0:
                hero_shapes[0].positionX += 1;
                break;
            case 1:
                hero_shapes[0].positionX += 1;
                break;
            case 2:
                hero_shapes[0].positionY -= 1;
                break;
            case 3:
                hero_shapes[0].positionY -= 1;
                break;
            default:
                break;
            }
            break;
        case 't':
        {
            if (!hero_shapes.empty()) {
                // 첫 번째 세그먼트 저장
                shape first_segment = hero_shapes.front();

                // 첫 번째 세그먼트를 제외하고 나머지 세그먼트를 한 칸씩 앞으로 이동
                for (size_t i = 1; i < hero_shapes.size(); ++i) {
                    hero_shapes[i - 1] = hero_shapes[i];
                }

                // 저장해둔 첫 번째 세그먼트를 맨 끝에 삽입
                hero_shapes.back() = first_segment;
            }
            break;
        }
        case 'a':
            if (!start_status) {
                start_status = true;
                SetTimer(hWnd, 1, 1, NULL);

                for (int i = 0; i < food_shapes.size(); ++i) {
                    hero_shapes.push_back(food_shapes[i]);
                    food_shapes[i].view_status = false;
                }

                for (int i = 0; i < hero_shapes.size(); ++i) {
                    hero_shapes[i].shape_status = true;
                }
            }
            else {
                start_status = false;
                SetTimer(hWnd, 1, timerInterval, NULL);
                for (int i = 0; i < food_shapes.size(); ++i) {
                    food_shapes[i].view_status = true;
                }

                shape h = hero_shapes[0];

                hero_shapes.clear();
                hero_shapes.push_back(h);
            }
            break;
        case 'q':
            KillTimer(hWnd, 1);
            KillTimer(hWnd, 2);
            PostQuitMessage(0);
            break;
        default:
            break;
        }

        InvalidateRect(hWnd, NULL, true);

        break;
    case WM_SIZE:
        break;
    case WM_PAINT:
    {
        GetClientRect(hWnd, &rect);
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        SelectObject(mDC, (HBITMAP)hBitmap);

        Rectangle(mDC, 0, 0, rect.right, rect.bottom);
        cellSizeX = rect.right / gridSize;
        cellSizeY = rect.bottom / gridSize;

        for (int y = 0; y <= gridSize; ++y) {
            MoveToEx(mDC, y * cellSizeX, 0, NULL);
            LineTo(mDC, y * cellSizeX, gridSize * cellSizeY);
        }

        for (int x = 0; x <= gridSize; ++x) {
            MoveToEx(mDC, 0, x * cellSizeY, NULL);
            LineTo(mDC, gridSize * cellSizeX, x * cellSizeY);
        }

        for (auto& shape : food_shapes) {
            if (shape.view_status) {
                shape.cellSizeX = cellSizeX;
                shape.cellSizeY = cellSizeY;
                shape.print_s(mDC);  // 먹이 도형
            }
        }

        for (auto& shape : hero_shapes) {
            shape.cellSizeX = cellSizeX;
            shape.cellSizeY = cellSizeY;
            shape.print_circle_shape(mDC);  // 주인공 도형
        }

        for (auto& black_r : black_rock) {
            black_r.cellSizeX = cellSizeX;
            black_r.cellSizeY = cellSizeY;
            black_r.print_circle_shape(mDC);  // 바리게이트 도형
        }

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        switch (wParam) {
        case 1:
            if (start_status) {
                shape hero_s1;

                for (int j = 0; j < food_shapes.size(); ++j) {
                    if (hero_shapes[0].positionX == food_shapes[j].positionX && hero_shapes[0].positionY == food_shapes[j].positionY
                        && food_shapes[j].shape_status && food_shapes[j].ready_status) {
                        food_shapes[j].ready_status = false;
                        food_shapes[j].view_status = false;
                        hero_s1 = food_shapes[j];
                        hero_s1.positionX = food_shapes[j].positionX;
                        hero_s1.positionY = food_shapes[j].positionY;
                        hero_shapes.push_back(hero_s1);
                        break;
                    }
                }

                // 먹이로 만듬
                for (int j = 0; j < food_shapes.size(); ++j) {
                    if (hero_shapes[0].positionX == food_shapes[j].positionX && hero_shapes[0].positionY == food_shapes[j].positionY
                        && !food_shapes[j].shape_status && food_shapes[j].view_status) {
                        food_shapes[j].move_i = uid_ran_4(gen);
                        food_shapes[j].shape_status = true;
                        food_shapes[j].ready_status = true;
                    }
                }

                // 위치 저장을 위한 임시 벡터
                vector<pair<int, int>> new_positions;

                // 현재 각 부분의 위치 저장
                for (auto& hero : hero_shapes) {
                    new_positions.push_back({ hero.positionX, hero.positionY });
                }

                // 키 상태에 따라 머리 위치 업데이트
                switch (key_status) {
                case 0: // 위
                    hero_shapes[0].positionY--;
                    break;
                case 1: // 아래
                    hero_shapes[0].positionY++;
                    break;
                case 2: // 왼쪽
                    hero_shapes[0].positionX--;
                    break;
                case 3: // 오른쪽
                    hero_shapes[0].positionX++;
                    break;
                }

                // 경계 조건 검사 및 조정

                for (int i = 0; i < black_rock.size(); ++i) {
                    if (black_rock[i].positionX == hero_shapes[0].positionX && black_rock[i].positionY == hero_shapes[0].positionY) {
                        switch (key_status)
                        {
                        case 0:
                            key_status = 1;
                            break;
                        case 1:
                            key_status = 0;
                            break;
                        case 2:
                            key_status = 3;
                            break;
                        case 3:
                            key_status = 2;
                            break;
                        default:
                            break;
                        }
                    }
                }
                
                if (hero_shapes[0].positionX > 39) {
                    hero_shapes[0].positionX = 39;
                    hero_shapes[0].positionY++;
                    key_status = 2;
                }
                else if (hero_shapes[0].positionX < 0) {
                    hero_shapes[0].positionX = 0;
                    hero_shapes[0].positionY++;
                    key_status = 3;
                }

                if (hero_shapes[0].positionY > 39) {
                    hero_shapes[0].positionY = 39;
                    hero_shapes[0].positionX++;
                    key_status = 0;
                }
                else if (hero_shapes[0].positionY < 0) {
                    hero_shapes[0].positionY = 0;
                    hero_shapes[0].positionX++;
                    key_status = 1;
                }

                // 몸통 위치 업데이트
                for (size_t i = 1; i < hero_shapes.size(); i++) {
                    hero_shapes[i].positionX = new_positions[i - 1].first;
                    hero_shapes[i].positionY = new_positions[i - 1].second;
                }


            }
            Timer1Count++;
            break;
        case 2:
        {


            if (Timer2Count % 2 == 0) {
                for (int i = 1; i < food_shapes.size(); ++i) {

                    //부딪히면
                    for (int i = 1; i < food_shapes.size(); ++i) {
                        if (!food_shapes[i].isWorm) {
                            for (int j = 1; j < food_shapes.size(); ++j) {

                                if (food_shapes[i].shape_status && food_shapes[j].shape_status &&
                                    food_shapes[i].positionX == food_shapes[j].positionX &&
                                    food_shapes[i].positionY == food_shapes[j].positionY) {
                                    if (i != j) {
                                        int r_m{ 3 };
                                        r_m = uid_ran_4(gen);
                                        food_shapes[i].move_i = r_m;
                                        food_shapes[j].move_i = r_m;
                                        switch (r_m)
                                        {
                                        case 0:
                                            food_shapes[j].positionX = food_shapes[i].positionX;
                                            food_shapes[j].positionY = food_shapes[i].positionY;
                                            food_shapes[j].positionX--;
                                            food_shapes[j].m1_status = food_shapes[i].m1_status;
                                            break;
                                        case 1:
                                            food_shapes[j].positionX = food_shapes[i].positionX;
                                            food_shapes[j].positionY = food_shapes[i].positionY;
                                            food_shapes[j].positionY++;
                                            food_shapes[j].m1_status = food_shapes[i].m1_status;
                                            break;
                                        case 2:
                                            food_shapes[j].positionX = food_shapes[i].positionX;
                                            food_shapes[j].positionY = food_shapes[i].positionY;
                                            food_shapes[j].positionX--;
                                            food_shapes[j].move_count = 0;
                                            break;
                                        case 3:

                                            break;
                                        default:
                                            break;
                                        }
                                        food_shapes[i].isWorm = true;
                                        food_shapes[j].isWorm = true;
                                    }
                                }
                            }
                        }
                    }

                    switch (food_shapes[i].move_i)
                    {
                    case 0: // 먹이 이동 방법 1

                        if (food_shapes[i].positionX == 39)
                            food_shapes[i].m1_status = true;
                        if (food_shapes[i].positionX == 0)
                            food_shapes[i].m1_status = false;

                        food_shapes[i].positionX += (food_shapes[i].m1_status == true ? -1 : 1); // 방향에 따라 오른쪽 또는 왼쪽으로 이동
                        break;
                    case 1: // 먹이 이동 방법 2
                        if (food_shapes[i].positionY == 39)
                            food_shapes[i].m1_status = true; // 위로 이동하는 방향으로 변경
                        if (food_shapes[i].positionY == 0)
                            food_shapes[i].m1_status = false; // 아래로 이동하는 방향으로 변경

                        food_shapes[i].positionY += (food_shapes[i].m1_status ? -1 : 1); // 현재 방향에 따라 위 또는 아래로 이동
                        break;
                    case 2: // 먹이 이동 방법 3
                        if (food_shapes[i].move_count >= 5) { // 5칸을 이동했다면 방향 전환
                            food_shapes[i].move_count = 0; // 카운트 리셋
                            food_shapes[i].move_R_i = (food_shapes[i].move_R_i + 1) % 4; // 다음 방향 설정
                        }

                        switch (food_shapes[i].move_R_i) {
                        case 0: // 오른쪽으로 이동
                            food_shapes[i].positionX++;
                            break;
                        case 1: // 아래로 이동
                            food_shapes[i].positionY++;
                            break;
                        case 2: // 왼쪽으로 이동
                            food_shapes[i].positionX--;
                            break;
                        case 3: // 위로 이동
                            food_shapes[i].positionY--;
                            break;
                        default:
                            break;
                        }
                        food_shapes[i].move_count++; // 이동 카운트 증가
                        break;
                    case 3:
                        break;
                    default:
                        break;
                    }
                }
            }
            Timer2Count++;
            break;
        }
        case 3:
            Timer3Count++;
            break;
        default:
            break;
        }

        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        KillTimer(hWnd, 2);
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}