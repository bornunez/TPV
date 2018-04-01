#pragma once
#include "PlayState.h"
#include "GameStateMachine.h"
#include "Colisiones.h"

const int MENU_BUTTONS = 3;

class MainMenuState : public GameState
{
private:
	static void menuToPlay(Game* game);
	static void exitFromMenu(Game* game);
	static void loadGame(Game* game);


public:
	MainMenuState(Game* game);
	void handleEvent(SDL_Event& event);
};

