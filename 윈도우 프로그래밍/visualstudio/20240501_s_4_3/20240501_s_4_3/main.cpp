#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include<set>
#include<stack>

using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"실습 4-3";
LPCTSTR lpszWindowName = L"실습 4-3";

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

    return Message.wParam;

}

#define WIDTH_LINE 40
#define HEGHIT_LINE 40

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
uniform_int_distribution<int> uid_shape(0, 5);
uniform_int_distribution<int> uid_color(0, 7);

struct Rec {
    int x;
    int y;
    RECT rect;
    COLORREF color;
    bool status;
    Rec() {
        switch (uid_color(gen))
        {
        case 0:
            color = RGB(0, 128, 128);
            break;
        case 1:
            color = RGB(240, 128, 128);
            break;
        case 2:
            color = RGB(204, 255, 0);
            break;
        case 3:
            color = RGB(139, 0, 139);
            break;
        case 4:
            color = RGB(255, 221, 0);
            break;
        case 5:
            color = (135, 206, 235);
            break;
        case 6:
            color = RGB(255, 182, 193);
            break;
        case 7:
            color = RGB(255, 253, 208);
            break;
        default:
            break;
        }
    }

};

struct Shape {
    RECT mapRect;
    bool status;
    vector<Rec> recs;
    int shape_status;
    int change_status = 0;

    Shape(int shape_s, RECT& mapRect) :shape_status(shape_s), mapRect(mapRect), status(true) {

        switch (uid_shape(gen))
        {
        case 0:
            for (int i = 0; i < 4; ++i) {
                Rec r;
                
                r.rect = { mapRect.left, 0, mapRect.left + 50, 50 };
                recs.push_back(r);
            }
            recs[0].x = 5; recs[0].y = 0;
            recs[1].x = 6; recs[1].y = 0;
            recs[2].x = 5; recs[2].y = 1;
            recs[3].x = 6; recs[3].y = 1;
            break;
        case 1:
            for (int i = 0; i < 3; ++i) {
                Rec r;

                r.rect = { mapRect.left, 0, mapRect.left + 50, 50 };
                recs.push_back(r);
            }
            recs[0].x = 4; recs[0].y = 0;
            recs[1].x = 5; recs[1].y = 0;
            recs[2].x = 6; recs[2].y = 0;
            break;
        case 2:
            for (int i = 0; i < 4; ++i) {
                Rec r;

                r.rect = { mapRect.left, 0, mapRect.left + 50, 50 };
                recs.push_back(r);
            }
            recs[0].x = 4; recs[0].y = 0;
            recs[1].x = 5; recs[1].y = 0;
            recs[2].x = 5; recs[2].y = 1;
            recs[3].x = 6; recs[3].y = 1;
            break;
        case 3:
            for (int i = 0; i < 4; ++i) {
                Rec r;

                r.rect = { mapRect.left, 0, mapRect.left + 50, 50 };
                recs.push_back(r);
            }
            recs[0].x = 4; recs[0].y = 0;
            recs[1].x = 5; recs[1].y = 0;
            recs[2].x = 6; recs[2].y = 0;
            recs[3].x = 4; recs[3].y = 1;
            break;
        case 4:
            for (int i = 0; i < 5; ++i) {
                Rec r;

                r.rect = { mapRect.left, 0, mapRect.left + 50, 50 };
                recs.push_back(r);
            }
            recs[0].x = 4; recs[0].y = 0;
            recs[1].x = 5; recs[1].y = 0;
            recs[2].x = 6; recs[2].y = 0;
            recs[3].x = 4; recs[3].y = 1;
            recs[4].x = 6; recs[4].y = 1;
            break;
        case 5:
            for (int i = 0; i < 4; ++i) {
                Rec r;

                r.rect = { mapRect.left, 0, mapRect.left + 50, 50 };
                recs.push_back(r);
            }
            recs[0].x = 4; recs[0].y = 0;
            recs[1].x = 5; recs[1].y = 0;
            recs[2].x = 6; recs[2].y = 0;
            recs[3].x = 5; recs[3].y = 1;
            break;
        default:
            break;
        }
    };

    void print_r(HDC& mDC) {
        for (auto& r : recs) {
            HBRUSH mBrush = CreateSolidBrush(r.color);
            HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
            HPEN mPen = CreatePen(PS_SOLID, 0.5, RGB(0, 0, 0));
            HPEN oldPen = (HPEN)SelectObject(mDC, mPen);

            //OffsetRect(&recs[i].rect, (x * 50), (y * 50));
            Rectangle(mDC, r.rect.left + (r.x * 50), r.rect.top + (r.y * 50), r.rect.right + (r.x * 50), r.rect.bottom + (r.y * 50));

            SelectObject(mDC, oldBrush);
            SelectObject(mDC, oldPen);
            DeleteObject(mBrush);
            DeleteObject(mPen);
        }
    }

    void change_v(vector<Shape>& listShape) {
        int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN;
        for (const auto& r : recs) {
            if (r.x < minX) minX = r.x;
            if (r.y < minY) minY = r.y;
            if (r.x > maxX) maxX = r.x;
        }

        vector<Rec> newRecs = recs;  // 복사하여 새로운 위치를 계산
        for (auto& r : newRecs) {
            int oldX = r.x - minX;
            int oldY = r.y - minY;
            r.x = oldY + minX;
            r.y = -oldX + minY + (recs[0].rect.bottom - recs[0].rect.top) / 50;  // 높이 보정
        }

        // 벽에 닿는지 검사하고 조정
        int adjustX = 0, adjustY = 0;
        for (const auto& r : newRecs) {
            if (r.x < 0 && r.x < adjustX) {
                adjustX = -r.x;
            }
            else if (r.x > 10 && r.x - 10 > adjustX) {
                adjustX = 9 - r.x;
            }
            if (r.y > 14) {
                int potentialAdjustY = 14 - r.y;
                if (potentialAdjustY < adjustY) {
                    adjustY = potentialAdjustY;
                }
            }
        }

        // 필요한 경우 도형의 x, y 좌표 조정
        if (adjustX != 0 || adjustY != 0) {
            for (auto& r : newRecs) {
                r.x += adjustX;
                r.y += adjustY;
            }
        }

        // 도형을 하나 내림
        if (change_status == 0 || change_status == 2) {
            for (auto& r : newRecs) {
                r.y++;
            }
        }

        // 다른 도형과의 충돌 검사
        bool collision = false;
        for (const auto& shape : listShape) {  // listShape는 다른 모든 도형을 포함하는 벡터
            for (const auto& otherRec : shape.recs) {
                for (const auto& r : newRecs) {
                    if (otherRec.x == r.x && otherRec.y == r.y) {
                        collision = true;  // 충돌 발견
                        break;
                    }
                }
                if (collision) break;
            }
            if (collision) break;
        }

        if (!collision) {
            recs = newRecs;  // 회전된 도형으로 업데이트
            change_status = (change_status + 1) % 4;  // 다음 회전 상태로 업데이트
        }
    }
  
};

void removeFullLines(vector<Shape>& listShape, int mapWidth) {
    const int MAX_HEIGHT = 15;  // 게임 필드의 최대 높이 가정
    vector<int> lineCount(MAX_HEIGHT + 1, 0);
    map<int, vector<Rec*>> recPositions;  // 각 y 레벨에 있는 Rec의 포인터를 저장

    // 각 줄에 있는 Rec의 수 계산
    for (auto& shape : listShape) {
        for (auto& rec : shape.recs) {
            lineCount[rec.y]++;
            recPositions[rec.y].push_back(&rec);
        }
    }

    // 완성된 줄 확인
    for (int y = 0; y <= MAX_HEIGHT; ++y) {
        if (lineCount[y] == mapWidth) {  // mapWidth는 한 줄을 채우는 데 필요한 Rec의 수
            // 줄 제거
            for (auto rec : recPositions[y]) {
                rec->status = false;  // 해당 Rec을 비활성화
            }
            // 위에 있는 도형을 아래로 이동
            for (auto& shape : listShape) {
                for (auto& rec : shape.recs) {
                    if (rec.y < y && rec.status) {  // 현재 줄 위에 있고 활성화된 Rec만
                        rec.y++;
                    }
                }
            }
        }
    }

    // 비활성화된 Rec 제거
    for (auto& shape : listShape) {
        auto& recs = shape.recs;
        recs.erase(remove_if(recs.begin(), recs.end(), [](const Rec& rec) {
            return !rec.status;  // 상태가 false인 Rec 제거
            }), recs.end());
    }
}
vector<pair<int, int>> findGroupDFS(const pair<int, int>& start, map<pair<int, int>, vector<Rec*>>& adjacencyList, set<pair<int, int>>& visited) {
    vector<pair<int, int>> group;
    stack<pair<int, int>> stack;

    stack.push(start);

    while (!stack.empty()) {
        auto current = stack.top();
        stack.pop();

        if (visited.find(current) != visited.end()) {
            continue;
        }

        visited.insert(current);
        group.push_back(current);

        vector<pair<int, int>> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} }; // 상, 하, 좌, 우

        for (const auto& dir : directions) {
            pair<int, int> neighbor = { current.first + dir.first, current.second + dir.second };

            if (adjacencyList.find(neighbor) != adjacencyList.end() && adjacencyList[neighbor][0]->color == adjacencyList[current][0]->color) {
                if (visited.find(neighbor) == visited.end()) {
                    stack.push(neighbor);
                }
            }
        }
    }

    return group;
}
void removeConnectedShapes(vector<Shape>& listShape) {
    map<pair<int, int>, vector<Rec*>> adjacencyList;
    for (auto& shape : listShape) {
        for (auto& rec : shape.recs) {
            // 각 Rec을 그 위치에서 맵에 추가합니다.
            adjacencyList[{rec.x, rec.y}].push_back(&rec);
        }
    }

    // 색상별로 그룹화된 도형을 찾고 제거합니다.
    vector<vector<pair<int, int>>> groups;
    set<pair<int, int>> visited;

    for (auto& entry : adjacencyList) {
        if (visited.find(entry.first) == visited.end()) {
            // DFS 또는 BFS를 사용하여 그룹을 찾습니다.
            vector<pair<int, int>> group = findGroupDFS(entry.first, adjacencyList, visited);
            if (group.size() >= 3) {
                groups.push_back(group);
            }
        }
    }

    // 찾아진 그룹에 속하는 모든 도형을 제거합니다.
    for (auto& group : groups) {
        for (auto& pos : group) {
            for (auto rec : adjacencyList[pos]) {
                rec->status = false;  // 도형의 상태를 비활성화로 변경
            }
        }
    }

    // 모든 비활성화된 도형을 제거합니다.
    for (auto& shape : listShape) {
        auto& recs = shape.recs;
        recs.erase(remove_if(recs.begin(), recs.end(), [](const Rec& rec) {
            return !rec.status;
            }), recs.end());
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;
    HPEN mPen, oldPen;
    HBRUSH mBrush, oldBrush;
    static RECT rect;
    static SIZE size;
    static int Timer1Count = 1000;
    static RECT mapRect;
    static bool game_status = false;
    static vector<Shape> listShape;
    static vector<Shape> makeShape;
    static bool make_status = true;
    static bool game_over = false;
    static vector<Rec> gameOverShape;
    static int g_int = 15;

    switch (uMsg)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, 1, Timer1Count, NULL);
        break;
    }
    case WM_COMMAND:
        switch (wParam)
        {
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때
    {
        switch (wParam)
        {
        case VK_RETURN:
            makeShape[0].change_v(listShape);
            break;
        case VK_LEFT:
        {
            bool status = true;
            for (auto& r : makeShape[0].recs) {
                if (r.x == 0) {
                    status = false;
                }
            }

            for (auto& r1 : listShape) {
                for (auto& r2 : r1.recs) {
                    for (auto& mr : makeShape[0].recs) {
                        if (r2.x == mr.x - 1 && r2.y == mr.y) {
                            status = false;
                        }
                    }
                }
            }

            if (status) {
                for (auto& r : makeShape[0].recs) {
                    r.x--;
                }
            }
            break;
        }
        case VK_UP:
            break;
        case VK_RIGHT:
        {
            bool status = true;
            for (auto& r : makeShape[0].recs) {
                if (r.x == 10) {
                    status = false;
                }
            }
            for (auto& r1 : listShape) {
                for (auto& r2 : r1.recs) {
                    for (auto& mr : makeShape[0].recs) {
                        if (r2.x == mr.x + 1 && r2.y == mr.y) {
                            status = false;
                        }
                    }
                }
            }
            if (status) {
                for (auto& r : makeShape[0].recs) {
                    r.x++;
                }
            }
            break;
        }
        case VK_DOWN:
        {
            bool status = true;
            for (auto& r : makeShape[0].recs) {
                if (r.y == 15) {
                    status = false;
                }
            }

            for (auto& r1 : listShape) {
                for (auto& r2 : r1.recs) {
                    for (auto& mr : makeShape[0].recs) {
                        if (r2.y - 1 == mr.y && r2.x == mr.x) {
                            status = false;
                        }
                    }
                }
            }

            if (status) {
                for (auto& r : makeShape[0].recs) {
                    r.y++;
                }
            }
            break;
        }
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);  // 윈도우를 다시 그립니다.
        break;
    }
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

        { //map
            mapRect = {
                rect.right / 2 - 425,
                -1,
                rect.right / 2 + 125,
                800
            };
            mBrush = (HBRUSH)GetStockObject(BLACK_BRUSH); // 투명한 브러시 사용
            oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

            FrameRect(mDC, &mapRect, mBrush); // 사각형의 테두리만 그림

            SelectObject(mDC, oldBrush);
            DeleteObject(mBrush);
        }


        for (auto& r1 : listShape) {
            for (auto& r2 : r1.recs) {
                if (r2.y == 1) {
                    game_over = true;
                    KillTimer(hWnd, 1);
                    SetTimer(hWnd, 2, 1, NULL);
                }
            }
        }

        if (game_over) {
            for (auto& r : gameOverShape) {
                HBRUSH mBrush = CreateSolidBrush(r.color);
                HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, mBrush);
                HPEN mPen = CreatePen(PS_SOLID, 0.5, RGB(0, 0, 0));
                HPEN oldPen = (HPEN)SelectObject(mDC, mPen);

                Rectangle(mDC, r.rect.left + (r.x * 50), r.rect.top + (r.y * 50), r.rect.right + (r.x * 50), r.rect.bottom + (r.y * 50));

                SelectObject(mDC, oldBrush);
                SelectObject(mDC, oldPen);
                DeleteObject(mBrush);
                DeleteObject(mPen);
            }
        }

        if (make_status) {

            while (true) {
                if (makeShape.size() == 2)
                    break;
                Shape ms(uid_shape(gen), mapRect);
                makeShape.push_back(ms);
            }

            for (auto& r : makeShape[1].recs) {
                r.x += 9;
                r.y += 2;
            }

            make_status = false;
        }


        //놓인 출력
        for (int i = 0; i < listShape.size(); ++i) {
            listShape[i].print_r(mDC);
        }

        //움직이는 도형
        for (int i = 0; i < makeShape.size(); ++i) {
            makeShape[i].print_r(mDC);
        }


        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);
        DeleteObject(hBitmap); // 생성한 비트맵 삭제
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        switch (wParam)
        {
        case 1:
        {

            bool status = true;
            for (auto& r : makeShape[0].recs) {
                if (r.y == 15) {
                    status = false;
                }
            }

            for (auto& r1:listShape) {
                for (auto& r2 : r1.recs) {
                    for (auto& mr : makeShape[0].recs) {
                        if (r2.y-1 == mr.y && r2.x == mr.x) {
                            status = false;
                        }
                    }
                }
            }

            if (status) {
                for (auto& r : makeShape[0].recs) {
                    r.y++;
                }
            }

            else {
                if (!game_over) {
                    listShape.push_back(makeShape[0]);
                    makeShape[0] = makeShape[1];
                    for (auto& r : makeShape[0].recs) {
                        r.x -= 9;
                        r.y -= 2;
                    }
                    makeShape.pop_back();

                    Shape ms(uid_shape(gen), mapRect);
                    makeShape.push_back(ms);

                    for (auto& r : makeShape[1].recs) {
                        r.x += 9;
                        r.y += 2;
                    }
                }
            }
            
            removeFullLines(listShape, 11);
            removeConnectedShapes(listShape);

            break;
        }
        case 2:
        {
            Rec r;
            if (g_int != -1) {
                for (int i = 0; i < 11; ++i) {
                    r.x = i;
                    r.y = g_int;
                    r.rect = { mapRect.left, 0, mapRect.left + 50, 50 };
                    gameOverShape.push_back(r);
                }
                g_int--;
            }
            if (g_int == -1) {
                KillTimer(hWnd, 2);
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