#include "PacMan.h"


PacMan::PacMan(Texture* text, Game* mainGame) : GameObject (text, mainGame)
{
	life = 3;
	energy = 0;
}
PacMan::PacMan(Texture* text, Game* mainGame, uint X, uint Y, uint W, uint H, uint iniCol, uint iniRow) : GameObject (text, mainGame, X, Y, W, H, iniCol, iniRow) {
	life = 3;
	energy = 0;
}

PacMan::~PacMan()
{
}

void PacMan::update() {
	//Logica propia de pacman
}


void PacMan::die() {
	x = iniX;
	y = iniY;
	this->life--;
}

void PacMan::nextDir() {


}