#include "GameCharacter.h"
#include "GameStateMachine.h"


GameCharacter::GameCharacter()
{
}

GameCharacter::GameCharacter(Texture * text, PlayState* playState, Game * game, uint texCol, uint texRow, uint w, uint h)
{
	this->texture = text;
	this->game = game;
	this->texRow = texRow; this->texCol = texCol;
	this->w = w; this->h = h; iniW = w; iniH = h;
	this->playState = playState;
}

GameCharacter::~GameCharacter()
{
}

//PINTA EN PANTALLA
void GameCharacter::render() {
	destRect.x = x*iniW; destRect.y = y*iniH;
	destRect.w = w; destRect.h = h;
	texture->renderFrame(destRect, texRow + ((int)dir % 4), texCol + (frame % 2));
}


//CARGA DE FICHERO
void GameCharacter::loadFromFile(ifstream& file) {
		file >> y >> x >> iniY >> iniX >> dirX >> dirY;
}

//GUARDA EN EL FICHERO
void GameCharacter::saveToFile(ofstream& file) {
	file << y << " " << x << " " << iniY << " " << iniX << " " << dirX << " " << dirY;
}

bool GameCharacter::handleEvent(SDL_Event & e)
{
	return false;
}

//DADA UNA DIRECCION, DEVOLVERA TRUE SI SE PUEDE MOVER, EN CUYO CASO ESTARÁN SE DEVUELVE EN NX NY
bool GameCharacter::next(int & nx, int & ny, Direction ndir)
{
	//Asignamos las variables
	int ndx, ndy;
	Utilities::enumToDir(ndir, ndx, ndy);
	nx = x + ndx;
	ny = y + ndy;
	int rows = playState->getRows();
	int cols = playState->getCols();
	//Aplicamos la forma toroidal
	if (nx < 0)
		nx = cols - 1;
	else if (nx >= cols)
		nx = 0;
	if (ny < 0)
		ny = rows - 1;
	else if (ny >= rows)
		ny = 0;
	return playState->getCell(ny, nx) != Wall;
}

void GameCharacter::init(uint W, uint H) {
	w = W;
	h = H;
	iniW = w; iniH = h;
}

void GameCharacter::init(int X, int Y, Direction DIR)
{
	x = X;
	y = Y;
	iniX = X;
	iniY = Y;
	dir = None;
}