#include "GameCharacter.h"



GameCharacter::GameCharacter()
{
}

GameCharacter::GameCharacter(Texture * text, Game * game, uint texCol, uint texRow, uint w, uint h)
{
	this->texture = text;
	this->game = game;
	this->texRow = texRow; this->texCol = texCol;
	this->w = w; this->h = h;
}


GameCharacter::~GameCharacter()
{
}

void GameCharacter::render() {
	SDL_Rect destRect;
	destRect.x = x*w; destRect.y = y*h;
	destRect.w = w; destRect.h = h;
	//texture->renderFrame(destRect, texRow, texCol);
	texture->renderFrame(destRect, texRow + ((int)dir % 4), texCol + (frame % 2));
}

void GameCharacter::loadFromFile(ifstream& file) {
	file >> x >> y >> iniX >> iniY >> dirX >> dirY;
}

void GameCharacter::saveToFile(ofstream& file) {
	file << x << " " << y << " " << iniX << " " << iniY << " " << dirX << " " << dirY;
}

bool GameCharacter::next(int & nx, int & ny, Direction ndir)
{
	//Asignamos las variables
	int ndx, ndy;
	Utilities::enumToDir(ndir, ndx, ndy);
	nx = x + ndx;
	ny = y + ndy;
	int rows = game->getRows();
	int cols = game->getCols();
	//Aplicamos la forma toroidal
	if (nx < 0)
		nx = cols - 1;
	else if (nx >= cols)
		nx = 0;
	if (ny < 0)
		ny = rows - 1;
	else if (ny >= rows)
		ny = 0;
	return game->getCell(ny, nx) != Wall;
}

void GameCharacter::init(int iniPosX, int iniPosY, uint W, uint H) {
	iniX = iniPosX;
	iniY = iniPosY;
	x = iniX;
	y = iniY;
	w = W;
	h = H;
}