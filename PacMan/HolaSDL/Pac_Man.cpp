#include "Pac_Man.h"
#include "Game.h"

Pac_Man::Pac_Man() {}

Pac_Man::Pac_Man(Texture* text,Game* GAME)
{
	texture = text;
	game = GAME;
}
Pac_Man::Pac_Man(Texture* text, Game* GAME, uint iniCol, uint iniRow) {
	texture = text;
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
	texture->renderFrame(destRect, texRow + ((int) dir%4), texCol + (frame % 2));
}

void Pac_Man::setDir(Direction ndir) {
	buffer = ndir;
}


void Pac_Man::update() {
	//setTexture();
	int nx, ny;
	if (buffer != None && next(nx, ny, buffer)) {
		dir = buffer;
		buffer = None;
	}
	if (next(nx,ny,dir)) {
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
			powerUp = true;
		}
	}
	frame++;
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
	return game->getCell(ny, nx) != Wall;
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

void Pac_Man::init(int iniPosX, int iniPosY, uint W, uint H) {
	iniX = iniPosX; iniY = iniPosY;
	x = iniX; y = iniY;
	w = W; h = H;
}

bool Pac_Man::die() {
	life--;
	if (life == 0)
		return true;
	else
		return false;
}

void Pac_Man::setTexture(int i, int j) {

}