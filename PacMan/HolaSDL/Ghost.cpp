#include "Ghost.h"
#include "Game.h"
Ghost::Ghost() {}

Ghost::Ghost(Texture* text)
{
	texture = text;
	//game = GAME;
}
Ghost::Ghost(Texture* text, uint X, uint Y, uint W, uint H, Game* GAME,uint iniCol, uint iniRow) {
	texture = text;
	x = X; y = Y;
	w = W; h = H;
	texRow = iniRow; texCol = iniCol;
	game = GAME;
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
	mueve();
}
bool Ghost::next(int& nx, int&ny, Direction ndir) {
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
void Ghost::EnumToDir(Direction dir, int&ndx, int& ndy) {
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
	else if (dir == None) {
		ndx = 0;
		ndy = 0;
	}
}
void Ghost::mueve() {
	vector<Direction> Dirs;
	//Vamos a guardar las direcciones posibles, de donde luego cogeremos una
	int nx, ny;
	for (int i = 0; i < 4; i++) {
		if (next(nx, ny, (Direction)i))
			Dirs.push_back((Direction)i);
	}
	if (Dirs.size() > 1) {
		int i = 0;
		Direction back = backDir(dir);
		while (i < Dirs.size() && Dirs[i] != back )
			i++;
		if (i < Dirs.size())
			Dirs.erase(Dirs.begin()+ i);
	}
	if (Dirs.size() >= 1)
		dir = Dirs[rand() % Dirs.size()];
	else
		dir = None;
	next(nx, ny, dir);
	x = nx;
	y = ny;
}
Direction Ghost::backDir(Direction currDir) {
	if (currDir == Up)
		return Down;
	else if (currDir == Down)
		return Up;
	else if (currDir == Left)
		return Right;
	else if (currDir == Right)
		return Left;
	else
		return None;
}
