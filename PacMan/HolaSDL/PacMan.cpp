#include "PacMan.h"


PacMan::PacMan()
{
}

PacMan::PacMan(Texture * text, Game * GAME, uint iniCol, uint iniRow, uint w, uint h) : GameCharacter(text,GAME, iniCol, iniRow, w, h)
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
			energy = MAX_ENERGY;
		}
	}
	frame++;

	if (hasEnergy())
		energy--;
}

//RESTA UNA VIDA A PACMAN Y SI SU VIDA LLEGA A CERO SE CONSIDERA DEAD
void PacMan::die() {
	life--;
	if (life == 0) {
		SDL_Delay(dieLapse);
		dead = true;
	}
	else {
		x = iniX;
		y = iniY;
		dead = false;
	}
}

//CARGA DE FICHERO LOS ARCHIVOS NECESARIOS PARA PACMAN
void PacMan::loadFromFile(ifstream& file)
{	
	GameCharacter::loadFromFile(file);
	int aux;
	energy = 0;
	file >> aux;
	if (file.fail())
		file.clear();
	else {
		energy = aux;
		file >> life;
	}
}

//GUARDA EN FICHERO LOS ARCHIVOS DE PACMAN NECESARIOS
void PacMan::saveToFile(ofstream& file)
{
	GameCharacter::saveToFile(file);
	file << " " << energy << " " << life;
}