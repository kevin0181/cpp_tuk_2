#include <Windows.h>
#include <string>
#include <gdiplus.h>
#include<atlimage.h>
#include"game_befor.h"
#include"sound.h"

using namespace std;

void PlaySecondMP3(const WCHAR* filename);

void game_setting(WPARAM wParam, CImage& pImage, int& player_num, bool& start, bool& game_setting_status) {
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
        PlaySecondMP3(L"sound/button sound.MP3"); // ��ư ����
        pImage.Destroy();
        pImage.Load(L"img/player_1.png"); // �� �̹��� �ε�
        break;
    case 2:
        PlaySecondMP3(L"sound/button sound.MP3"); // ��ư ����
        pImage.Destroy();
        pImage.Load(L"img/player_2.png"); // �� �̹��� �ε�
        break;
    case 3:
        PlaySecondMP3(L"sound/button sound.MP3"); // ��ư ����
        pImage.Destroy();
        pImage.Load(L"img/player_back.png"); // �� �̹��� �ε�
        break;
    default:
        break;

    }

    if (VK_RETURN == wParam) {
        switch (player_num) {
        case 1: // player1
            break;
        case 2: // player2
            break;
        case 3: // �ڷΰ���
            //pImage.Destroy();
            //pImage.Load(L"img/Inversus Intro.png");
            //PlayMP3Close();
            //PlayMP3(L"sound/main intro.mp3"); // ��ο� �ִ� MP3 ���� ���
            //start = true;
            //game_setting_status = false;
            PostQuitMessage(0);
            break;
        default:
            break;
        }
    }
}