#include "PacMan.h"


PacMan::PacMan()
{
}

PacMan::PacMan(Texture * text, PlayState* playState, Game* game, uint iniCol, uint iniRow, uint w, uint h) : GameCharacter(text, playState, game, iniCol, iniRow, w, h)
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
		MapCell cell = playState->getCell(ny, nx);
		//Y nos movemos en consecuencia
		if (cell == Food) {
			playState->eat();
			playState->setCell(ny, nx, Empty);
		}
		else if (cell == PowerUp) {
			playState->setCell(ny, nx, Empty);
			playState->powerUp();
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
		try
		{
			energy = aux;
			if (aux < 0)
				throw FileFormatError("Energia menor que 0, se recuperará el juego...");
		}
		catch (exception& e)
		{
			cerr << "Caught: " << e.what() << endl;
			cerr << "Type: " << typeid(e).name() << endl;
			energy = 0;
		}
		file >> life;
	}
}

//GUARDA EN FICHERO LOS ARCHIVOS DE PACMAN NECESARIOS
void PacMan::saveToFile(ofstream& file)
{
	GameCharacter::saveToFile(file);
	file << " " << energy << " " << life;
}

bool PacMan::handleEvent(SDL_Event & event)
{
	if (event.key.keysym.sym == SDLK_LEFT)
		setDir(Left);
	else if (event.key.keysym.sym == SDLK_RIGHT)
		setDir(Right);
	else if (event.key.keysym.sym == SDLK_UP)
		setDir(Up);
	else if (event.key.keysym.sym == SDLK_DOWN)
		setDir(Down);
	return true;
}
