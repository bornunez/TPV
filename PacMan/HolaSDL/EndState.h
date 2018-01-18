#pragma once
#include "GameState.h"
#include "GameStateMachine.h"

const int END_BUTTONS = 2;
class EndState : public GameState
{
private: 
	Font* gameFont; //Cambiar nombre?

	static void newGame(Game* game);
	static void exit(Game* game);

public:
	EndState(Game* game);
	~EndState();
	void handleEvent(SDL_Event event);
};

