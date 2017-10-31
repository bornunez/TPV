#include "GameObject.h"
#include "Game.h";

GameObject::GameObject(Texture* text, Game* mainGame)
{
	game = mainGame;
	texture = text;
}
GameObject:: GameObject(Texture* text, Game* mainGame, uint X, uint Y, uint W, uint H, uint iniCol, uint iniRow) {
	game = mainGame;
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
	texture->renderFrame(destRect, texRow, texCol);
	
	update();
}

//Cada go tendra su propio update sobreescrito ya que todos heredaran de GO
void GameObject::update() {
	
}
