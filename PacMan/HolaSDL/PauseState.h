#pragma once
#include "GameState.h"

const int PAUSE_BUTTONS = 3;

class PauseState :
	public GameState
{
private:
	static void resume(Game* game);
	static void exitFromMenu(Game* game);
	static void saveGame(Game* game);
public:
	PauseState(Game* game);
	~PauseState();
};

