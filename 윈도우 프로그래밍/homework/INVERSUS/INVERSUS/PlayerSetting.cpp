#include <Windows.h>
#include <string>
#include <gdiplus.h>
#include<atlimage.h>

#include"GameStateManager.h"
#include"PlayerSetting.h"
#include"sound.h"

using namespace std;

void PlaySecondMP3(const WCHAR* filename);

void PlayerSetting::game_setting(WPARAM wParam) {

    const std::wstring PATH = L"img/player/";

    switch (wParam)
    {
    case VK_DOWN:

        if (player == 3) {
            player = 1;
        }
        else {
            player++;
        }

        break;
    case VK_UP:
        if (player == 1) {
            player = 3;
        }
        else {
            player--;
        }
        break;
    default:
        break;
    }

    // -------- �̹��� ����
    switch (player)
    {
    case 1:
        PlaySecondMP3(L"sound/button sound.MP3"); // ��ư ����
        gameStateManager->setImage((PATH + L"player_1.png").c_str());
        break;
    case 2:
        PlaySecondMP3(L"sound/button sound.MP3"); // ��ư ����
        gameStateManager->setImage((PATH + L"player_2.png").c_str());
        break;
    case 3:
        PlaySecondMP3(L"sound/button sound.MP3"); // ��ư ����
        gameStateManager->setImage((PATH + L"player_back.png").c_str());
        break;
    default:
        break;

    }

    if (VK_RETURN == wParam) {
        switch (player) {
        case 1: // player1
            gameStateManager->setPlayer(player);
            gameStateManager->setImage(L"img/level/0.png");
            gameStateManager->setCurrentState(GameState::LEVEL);
            break;
        case 2: // player2
            gameStateManager->setPlayer(player);
            gameStateManager->setImage(L"img/level/0.png");
            gameStateManager->setCurrentState(GameState::LEVEL);
            break;
        case 3: // �ڷΰ���
            PostQuitMessage(0);
            break;
        default:
            break;
        }
    }
}