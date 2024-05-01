#include <windows.h>		//--- ¿©µµøÏ «Ï¥ı ∆ƒ¿œ
#include <tchar.h>
#include <random>

#define marginX 16
#define marginY 39

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"2024 Window Programming Exam";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
int  WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_  LPSTR lpszCmdParam, _In_  int nCmdShow)
{
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
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	//--- ≈©±‚ ∫Ø∞Ê ∞°¥… (±‚¡∏ (1024, 768))
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 1024 + marginX, 768 + marginY, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC	hDC, mDC;
	HBRUSH hBrush, oldBrush;
	RECT rect;

	std::mt19937 gen(static_cast<unsigned int> (time(0)));
	std::uniform_int_distribution<int> color(0, 255);
	std::uniform_int_distribution<int> randomX(0, 19);
	std::uniform_int_distribution<int> randomY(0, 19);


	static int count = 0;
	static int count2 = 0;
	static int num = 0;
	static int playerX = 0;
	static int playerY = 0;
	static int enemyX = randomX(gen);
	static int enemyY = randomY(gen);
	static int itemX[50];
	static int itemY[50];
	static int obstacleX[100];
	static int obstacleY[100];
	static int mX;
	static int mY;
	static int bulletX = -100;
	static int bulletY = -100;
	static COLORREF playerColor = RGB(0, 255, 0);
	static COLORREF randomColor[50];
	static COLORREF enemyColor = RGB(255, 0, 0);

	static bool itemGet = false;
	static bool drawObstacle = false;
	static bool drawItem = false;
	static bool draw = false;

	static bool up = false;
	static bool down = false;
	static bool left = false;
	static bool right = false;

	switch (iMessage) {
	case WM_CREATE:
		for (int i = 0; i < 50; ++i) {
			itemX[i] = randomX(gen);
			itemY[i] = randomY(gen);
			randomColor[i] = RGB(color(gen), color(gen), color(gen));
		}

		SetTimer(hWnd, 1, 60, NULL);
		SetTimer(hWnd, 2, 100, NULL);

		break;
	case WM_TIMER:

		switch (wParam) {
		case 1:
			if (up) {
				bulletY -= 1;
			}
			else if (down) {
				bulletY += 1;
			}
			else if (left) {
				bulletX -= 1;
			}
			else if (right) {
				bulletX += 1;
			}
			break;
		case 2:
			if (enemyX < 0) {
				enemyX++;
				enemyY++;
			}
			else if (enemyX > 19) {
				enemyX--;
				enemyY--;
			}
			else if (enemyY < 0) {
				enemyX--;
				enemyY++;
			}
			else if (enemyY > 19) {
				enemyY--;
				enemyX++;
			}

		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_MOUSEMOVE:

		if (drawObstacle && wParam == MK_LBUTTON) {
			mX = LOWORD(lParam);
			mY = HIWORD(lParam);

			obstacleX[num] = mX / 20;
			obstacleY[num] = mY / 20;

			if (obstacleX[num] == obstacleX[num + 1] && obstacleY[num] == obstacleY[num + 1] || obstacleX[num] == obstacleX[num - 1] && obstacleY[num] == obstacleY[num - 1]) {
				obstacleX[num + 1] == -1;
				obstacleY[num + 1] == -1;

				--num;
			}

			++num;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_UP:
			playerY--;


			for (int i = 0; i < 100; ++i) {
				if (playerY < 0) {
					playerY++;
				}
				else if (playerX == obstacleX[i] && playerY == obstacleY[i] && itemGet == false)
				{
					playerY++;
				}
				else if (playerX == obstacleX[i] && playerY == obstacleY[i] && itemGet)
				{
					playerY--;
					obstacleY[i] = -1;
					obstacleX[i] = -1;
					itemGet = false;
				}
			}

			up = true;
			down = false;
			left = false;
			right = false;


			break;
		case VK_DOWN:
			playerY++;

			for (int i = 0; i < 100; ++i) {
				if (playerY > 19) {
					playerY--;
				}
				else if (playerX == obstacleX[i] && playerY == obstacleY[i] && itemGet == false)
				{
					playerY--;
				}
				else if (playerX == obstacleX[i] && playerY == obstacleY[i] && itemGet)
				{
					playerY++;
					obstacleY[i] = -1;
					obstacleX[i] = -1;
					itemGet = false;

				}
			}

			up = false;
			down = true;
			left = false;
			right = false;


			break;
		case VK_RIGHT:
			playerX++;

			for (int i = 0; i < 100; ++i) {
				if (playerX > 19) {
					playerX--;
				}
				else if (playerX == obstacleX[i] && playerY == obstacleY[i] && itemGet == false)
				{
					playerX--;

				}
				else if (playerX == obstacleX[i] && playerY == obstacleY[i] && itemGet)
				{
					playerX++;

					obstacleY[i] = -1;
					obstacleX[i] = -1;
					itemGet = false;

				}
			}

			up = false;
			down = false;
			left = false;
			right = true;


			break;
		case VK_LEFT:
			playerX--;

			for (int i = 0; i < 100; ++i) {
				if (playerX < 0) {
					playerX++;
				}
				else if (playerX == obstacleX[i] && playerY == obstacleY[i] && itemGet == false)
				{
					playerX++;
				}
				else if (playerX == obstacleX[i] && playerY == obstacleY[i] && itemGet)
				{
					playerX--;
					obstacleY[i] = -1;
					obstacleX[i] = -1;
					itemGet = false;

				}
			}

			up = false;
			down = false;
			left = true;
			right = false;


			break;
		case 'S':

			if (count != 4) {
				if (drawObstacle) {
					drawObstacle = false;
					++count;
				}
				else {
					drawObstacle = true;
				}
			}
			break;
		case 'P':
			if (drawItem) {
				drawItem = false;
			}
			else {
				drawItem = true;
				draw = true;
			}
			break;
		case 'Q':
			exit(0);
			break;
		case VK_RETURN:

			if (up) {
				bulletX = playerX;
				bulletY = playerY - 1;
			}
			else if (down) {
				bulletX = playerX;
				bulletY = playerY + 1;
			}
			else if (left) {
				bulletX = playerX - 1;
				bulletY = playerY;
			}
			else if (right) {
				bulletX = playerX + 1;
				bulletY = playerY;
			}

			break;
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		for (int i = 0; i < 20; ++i) {
			for (int j = 0; j < 20; ++j) {
				Rectangle(hDC, i * 20, j * 20, (i + 1) * 20, (j + 1) * 20);
			}
		}

		// «√∑π¿ÃæÓ ±◊∏Æ±‚
		if (itemGet == false) {
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Rectangle(hDC, playerX * 20, playerY * 20, (playerX + 1) * 20, (playerY + 1) * 20);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
		}
		else if (itemGet == true) {
			hBrush = CreateSolidBrush(playerColor);
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Ellipse(hDC, playerX * 20, playerY * 20, (playerX + 1) * 20, (playerY + 1) * 20);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
		}
		// ¿Âæ÷π∞
		for (int i = 0; i < num; ++i) {
			hBrush = CreateSolidBrush(RGB(180, 180, 180));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Rectangle(hDC, obstacleX[i] * 20, obstacleY[i] * 20, (obstacleX[i] + 1) * 20, (obstacleY[i] + 1) * 20);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
			++count2;

			if (bulletX == obstacleX[i] && obstacleY[i] == bulletY) {
				bulletX = -100;
				bulletY = -100;
				obstacleX[i] = -1;
				obstacleY[i] = -1;
			}
		}

		// æ∆¿Ã≈€
		for (int i = 0; i < num; ++i) {
			if (draw) {

				hBrush = CreateSolidBrush(randomColor[i]);
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, itemX[i] * 20, itemY[i] * 20, (itemX[i] + 1) * 20, (itemY[i] + 1) * 20);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
				if (playerX == itemX[i] && playerY == itemY[i]) {
					itemY[i] == -1;
					itemX[i] == -1;
					playerColor = randomColor[i];
					itemGet = true;
				}

			}
		}

		// ¿˚
		hBrush = CreateSolidBrush(enemyColor);
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, enemyX * 20, enemyY * 20, (enemyX + 1) * 20, (enemyY + 1) * 20);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		// √—æÀ

		hBrush = CreateSolidBrush(RGB(0, 0, 255));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, bulletX * 20, bulletY * 20, (bulletX + 1) * 20, (bulletY + 1) * 20);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		if (bulletX < 0 && bulletY < 0 || bulletX >= 20 || bulletY >= 20) {
			bulletX = -100;
			bulletY = -100;
		}

		if (enemyX == bulletX && enemyY == bulletY) {
			enemyColor = RGB(0, 0, 0);
			bulletX = -100;
			bulletY = -100;
		}


		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}


