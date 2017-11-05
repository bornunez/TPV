#include "Ghost.h"

Ghost::Ghost(Texture* text, Game* mainGame) 
{
	game = mainGame;
	texture = text;
}

Ghost::Ghost(Texture* text, Game* mainGame, uint X, uint Y, uint W, uint H, uint iniCol, uint iniRow) 
{
	game = mainGame;
	texture = text;
	x = X; y = Y;
	w = W; h = H;
	texRow = iniRow; texCol = iniCol;
}

Ghost::~Ghost()
{
}

void Ghost::setIniPosition(uint iniX, uint iniY) {
	this->iniX = iniX;
	this->iniY = iniY;
}

void Ghost::setActualPosition(uint x, uint y)
{
	this->x = x;
	this->y = y;
}

void Ghost::render() {
	SDL_Rect destRect;

	destRect.x = x; destRect.y = y;

	destRect.w = w; destRect.h = h;
	texture->renderFrame(destRect, texRow, texCol);
}

void Ghost::update() {
	//Aqui va la logica del movimiento de los fantasmas
}

void Ghost::die() {
	x = iniX;
	y = iniY;
}
