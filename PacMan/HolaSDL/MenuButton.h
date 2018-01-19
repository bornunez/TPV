#pragma once
#include "GameObject.h"


enum button_state
{
	MOUSE_OUT = 0,
	MOUSE_OVER = 1,
	CLICKED = 2
};

class MenuButton : public GameObject
{
private:
	Texture* texture = nullptr;	//	Puntero	a	su textura
	int x, y, width, height;
	SDL_Rect destRect;
	typedef void CallBackOnClick(Game* game);
	
protected:
	CallBackOnClick* cbOnClick;
	bool click;

public:
	MenuButton(int x, int y, int width, int height, Texture* texture, CallBackOnClick* cbOnClick, Game* game);
	MenuButton();
	~MenuButton();
	void render();
	void update();
	bool handleEvent(SDL_Event& e);
};

