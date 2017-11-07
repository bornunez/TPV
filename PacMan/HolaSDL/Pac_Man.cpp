#include "Pac_Man.h"
#include "Game.h"

Pac_Man::Pac_Man() {}

Pac_Man::Pac_Man(Texture* text,Game* GAME)
{
	texture = text;
	game = GAME;
}
Pac_Man::Pac_Man(Texture* text, uint X, uint Y, uint W, uint H, Game* GAME, uint iniCol, uint iniRow) {
	texture = text;
	x = X; y = Y;
	w = W; h = H;
	texRow = iniRow; texCol = iniCol;
	game = GAME;
}


Pac_Man::~Pac_Man()
{
}

void Pac_Man::render() {
	SDL_Rect destRect;
	destRect.x = x*w; destRect.y = y*h;
	destRect.w = w; destRect.h = h;
	texture->renderFrame(destRect, texRow, texCol);
}

void Pac_Man::setDir(Direction ndir) {
	buffer = ndir;
}


void Pac_Man::update() {
	int nx, ny;
	if (buffer != None && next(nx, ny, buffer)) {
		dir = buffer;
		buffer = None;
	}
	if (next(nx,ny,dir)) {
		x = nx;
		y = ny;
		//Vemos nuestra siguiente casilla
		MapCell cell = game->nextCell(ny, nx);
		//Y nos movemos en consecuencia
		if (cell == Food) {
			game->eat();
			game->setCell(ny, nx, Empty);
		}
		else if (cell == PowerUp) {
			game->setCell(ny, nx, Empty);
		}
	}
}
bool Pac_Man::next(int& nx, int&ny,Direction ndir) {
	//Asignamos las variables
	int ndx, ndy;
	EnumToDir(ndir, ndx, ndy);
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
	return game->nextCell(ny, nx) != Wall;
}
void Pac_Man::EnumToDir(Direction dir, int&ndx, int& ndy) {
	if (dir == Left) {
		ndx = -1;
		ndy = 0;
	}
	else if (dir == Up) {
		ndx = 0;
		ndy = -1;
	}
	else if (dir == Down) {
		ndx = 0;
		ndy = 1;
	}
	else if (dir == Right) {
		ndx = 1;
		ndy = 0;
	}
}