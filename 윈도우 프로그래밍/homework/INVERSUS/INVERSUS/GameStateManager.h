#pragma once

#include<atlimage.h>
#include <string>

#include "GameState.h"

class GameStateMachine {
private:
	GameState currentState;
	CImage cImage;
public:
	void setCurrentState(GameState state);
	void setImage(LPCTSTR path);
	void DrawImage(HDC& mDC, RECT rect);

	GameState getState();
};