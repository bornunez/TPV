#include "Ghost.h"
Ghost::Ghost() {}

Ghost::Ghost(Texture* text)
{
	texture = text;
	//game = GAME;
}
Ghost::Ghost(Texture* text, uint X, uint Y, uint W, uint H, uint iniCol, uint iniRow) {
	texture = text;
	x = X; y = Y;
	w = W; h = H;
	texRow = iniRow; texCol = iniCol;
}

Ghost::~Ghost()
{
}

void Ghost::render() {
	SDL_Rect destRect;
	destRect.x = x*w; destRect.y = y*h;
	destRect.w = w; destRect.h = h;
	texture->renderFrame(destRect, texRow, texCol);
}

void Ghost::update() {

}
