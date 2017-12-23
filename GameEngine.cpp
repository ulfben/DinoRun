#include "GameEngine.h"
void GameEngine::changeState(const GameState next){
	maybeExitAndDeleteState();
	switch(next){
	case GameState::StartMenu:
		_currentState = new StartMenuState(*this);
		break;
	case GameState::GameOver:
		_currentState = new GameOverState(*this);
		break;
	default: //GameState::GamePlay
		_currentState = new GamePlayState(*this);
		break;
	}
	_currentState->enter();
};