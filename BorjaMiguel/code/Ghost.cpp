#include "Ghost.h"
#include "Game.h"
Ghost::Ghost() {}

Ghost::Ghost(Texture* text)
{
	texture = text;
	//game = GAME;
}
Ghost::Ghost(Texture* text, Game* GAME,uint iniCol, uint iniRow) {
	texture = text;
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

	if (game->isPowered())
		texture->renderFrame(destRect, frame % 2, 12 + (frame % 2));
	else
		texture->renderFrame(destRect, texRow + ((int)dir % 4), texCol + (frame % 2));

}

void Ghost::update() {
	mueve();
	frame++;
}
bool Ghost::next(int& nx, int&ny, Direction ndir) { //DADA UNA DIRECCION DIR, DECIMOS SI SE PUEDE MOVER, Y DEVOLVEMOS EL SIGUIENTE PUNTO
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

void Ghost::mueve() {
	dirs.clear();
	//Vamos a guardar las direcciones posibles, de donde luego cogeremos una
	int nx, ny;
	for (int i = 0; i < 4; i++) {
		if (next(nx, ny, (Direction)i))
			dirs.push_back((Direction)i);
	}
	//Si hay mas de una posicion donde moverse
	if (dirs.size() > 1) {
		int i = 0;
		Direction back = Utilities::backDir(dir);
		//Buscamos a ver si esta la contraria a nuestro movimiento actual
		while (i < dirs.size() && dirs[i] != back )
			i++;
		if (i < dirs.size()) //En cuyo caso la eliminamos
			dirs.erase(dirs.begin()+ i);
	}
	//Y cogemos una direccion aleatoria
	if (dirs.size() >= 1)
		dir = dirs[rand() % dirs.size()];
	else
		dir = None;
	//Y nos movemos
	next(nx, ny, dir);
	x = nx;
	y = ny;
}


void Ghost::init(int iniPosX, int iniPosY, uint W, uint H) {
	iniX = iniPosX;
	iniY = iniPosY;
	x = iniX;
	y = iniY;
	w = W;
	h = H;
}