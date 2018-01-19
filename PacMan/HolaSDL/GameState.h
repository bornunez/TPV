#pragma once
#include "GameObject.h"
#include "MenuButton.h"
#include <list>



class GameState
{

protected:
	const int B_WIDTH = 200;const int B_HEIGHT = 80;
	int MIDDLESCREEN; int BUTTONSCALE = 2;
	int nButtons;

	list<GameObject*> stage;
	Game* game;
	Font* gameFont;

public:
	GameState(Game* game);
	virtual ~GameState();
	virtual void update();
	virtual void render();
	virtual void handleEvent(SDL_Event& event);
};

