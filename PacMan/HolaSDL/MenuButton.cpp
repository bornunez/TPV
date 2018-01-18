#include "MenuButton.h"


MenuButton::MenuButton(int x, int y, int width, int height, Texture* texture, CallBackOnClick* cbOnClick, Game* game)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->texture = texture;
	this->destRect.x = x; destRect.y = y;
	this->destRect.w = width; destRect.h = height;
	this->cbOnClick = cbOnClick;
	this->game = game;
}

MenuButton::MenuButton()
{
}

MenuButton::~MenuButton()
{
}

void MenuButton::render() {
	texture->render(destRect);
	//texture->renderFrame(destRect, 1, 1);

}

void MenuButton::update() { 
	/*
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	if (mouseX < (x + width)
		&& mouseX > x
		&& mouseY < (y + height)
		&& mouseY > y)
	{
		cbOnClick(game);
		}*/
}

bool MenuButton::handleEvent(SDL_Event & event)
{	
	bool handled = false;
	if (event.type = SDL_MOUSEBUTTONDOWN) //Es izquierdo del raton
	{
		int mouseX, mouseY;
		SDL_Point p;
		SDL_Rect r;
		SDL_GetMouseState(&p.x, &p.y);
		r.x = x; r.y = y; r.w = width; r.h = height;
		if (SDL_PointInRect(&p, &r)) {
			handled = true;
			cbOnClick(game);
		}
		return handled;
	}
	return false;
}
