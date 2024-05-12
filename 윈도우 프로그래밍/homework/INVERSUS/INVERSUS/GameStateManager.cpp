#include <string>
#include "GameStateManager.h"
#include "GameState.h"

void GameStateMachine::setCurrentState(GameState gameState) {
	currentState = gameState;
}

void GameStateMachine::setImage(LPCTSTR path) {
	cImage.Destroy();
	cImage.Load(path);
}

void GameStateMachine::DrawImage(HDC& mDC, RECT rect) {
	cImage.Draw(mDC, 0, 0, rect.right, rect.bottom, 0, 0, cImage.GetWidth(), cImage.GetHeight()); //이미지 전체 화면
}

GameState GameStateMachine::getState() {
	return currentState;
}