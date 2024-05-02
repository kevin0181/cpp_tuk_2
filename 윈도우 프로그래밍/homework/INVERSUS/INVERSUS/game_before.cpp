#include <Windows.h>
#include <string>
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;

void PlaySecondMP3(const WCHAR* filename);

void game_setting(bool game_status, WPARAM wParam, Image*& pImage, int& player_num, bool start) {
    if (!game_status && !start) { // ���� ���� ����
        PlaySecondMP3(L"button sound.MP3"); // ��ư ����
        switch (wParam)
        {
        case VK_DOWN:

            if (player_num == 3) {
                player_num = 1;
            }
            else {
                player_num++;
            }

            break;
        case VK_UP:
            if (player_num == 1) {
                player_num = 3;
            }
            else {
                player_num--;
            }
            break;
        default:
            break;
        }
        // -------- �̹��� ����
        switch (player_num)
        {
        case 1:
            delete pImage;
            pImage = new Image(L"img/player_1.png"); // �� �̹��� �ε�
            break;
        case 2:

            delete pImage;
            pImage = new Image(L"img/player_2.png"); // �� �̹��� �ε�
            break;
        case 3:
            delete pImage;
            pImage = new Image(L"img/player_back.png"); // �� �̹��� �ε�
            break;
        default:
            break;
        }
    }
}