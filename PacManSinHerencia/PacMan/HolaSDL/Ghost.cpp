#include "Ghost.h"

Ghost::Ghost(Texture* text, Game* mainGame) : GameObject(text, mainGame)
{
}

Ghost::Ghost(Texture* text, Game* mainGame, uint X, uint Y, uint W, uint H, uint iniCol, uint iniRow) : GameObject(text, mainGame, X, Y, W, H, iniCol, iniRow) {
}

Ghost::~Ghost()
{
}

void Ghost::update() {
	//Aqui va la logica del movimiento de los fantasmas
}

void Ghost::die() {
	x = iniX;
	y = iniY;
}
