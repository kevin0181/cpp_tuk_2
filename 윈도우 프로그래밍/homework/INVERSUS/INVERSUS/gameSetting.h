#pragma once
#include "GameStateManager.h"

class GameSetting {
private:
	int player;

	GameStateManager* gameStateManager;
public:

	GameSetting(GameStateManager* instance) : gameStateManager(instance) {};

	void game_setting(WPARAM wParam);
};