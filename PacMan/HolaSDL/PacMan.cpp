#include "PacMan.h"


PacMan::PacMan(Texture* text, Game* mainGame) 
{
	game = mainGame;
	texture = text;
	life = 3;
	energy = 0;
}
PacMan::PacMan(Texture* text, Game* mainGame, uint X, uint Y, uint W, uint H, uint iniCol, uint iniRow) 
{
	game = mainGame;
	texture = text;
	x = X; y = Y;
	w = W; h = H;
	texRow = iniRow; texCol = iniCol;
	life = 3;
	energy = 0;
}

PacMan::~PacMan()
{
}

void PacMan::setIniPosition(uint iniX, uint iniY) {
	this->iniX = iniX;
	this->iniY = iniY;
}

void PacMan::setActualPosition(uint x, uint y)
{
	this->x = x;
	this->y = y;
}

void PacMan::render() {
	SDL_Rect destRect;

	destRect.x = x; destRect.y = y;

	destRect.w = w; destRect.h = h;
	texture->renderFrame(destRect, texRow, texCol);
}

void PacMan::update() {

}


void PacMan::die() {
	x = iniX;
	y = iniY;
	this->life--;
}

void PacMan::nextDir() {


}