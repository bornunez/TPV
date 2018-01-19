#pragma once
#include "GameState.h"
#include "GameStateMachine.h"

const int END_BUTTONS = 2;
class EndState : public GameState
{
private: 
	static void newGame(Game* game);
	static void exit(Game* game);

public:
	EndState(Game* game, bool gameOver);
	~EndState();
	void render();
	void handleEvent(SDL_Event event);
};

