#include "PacMan.h"


PacMan::PacMan()
{
}

PacMan::PacMan(Texture * text, Game * GAME, uint iniCol, uint iniRow) : GameCharacter(text,GAME, iniCol, iniRow)
{
}


PacMan::~PacMan()
{
}

void PacMan::update()
{
	int nx, ny;
	if (buffer != None && next(nx, ny, buffer)) {
		dir = buffer;
		buffer = None;
	}
	if (next(nx, ny, dir)) {
		x = nx;
		y = ny;
		//Vemos nuestra siguiente casilla
		MapCell cell = game->getCell(ny, nx);
		//Y nos movemos en consecuencia
		if (cell == Food) {
			game->eat();
			game->setCell(ny, nx, Empty);
		}
		else if (cell == PowerUp) {
			game->setCell(ny, nx, Empty);
			game->powerUp();
		}
	}
	frame++;
}


bool PacMan::die() {
	life--;
	if (life == 0)
		return true;
	else {
		x = iniX;
		y = iniY;
		dead = true;
		return false;
	}
}


void PacMan::loadFromFile()
{
}

void PacMan::saveToFile()
{
}
