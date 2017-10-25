#include "GameObject.h"
#include "Game.h";


GameObject::GameObject(Texture* text)
{
	texture = text;
}
GameObject:: GameObject(Texture* text, uint X, uint Y, uint W, uint H, uint iniCol, uint iniRow) {
	texture = text;
	x = X; y = Y;
	w = W; h = H;
	texRow = iniRow; texCol = iniCol;
}


GameObject::~GameObject()
{
}

void GameObject::render() {

	SDL_Rect destRect;
	destRect.x = x; destRect.y = y;
	destRect.w = w; destRect.h = h;
	texture->renderFrame(game->GetRenderer(), destRect, texRow, texCol);
}

void GameObject::update() {

}
