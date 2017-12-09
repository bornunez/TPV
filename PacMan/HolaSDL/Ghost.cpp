#include "Ghost.h"


Ghost::Ghost() {}

Ghost::Ghost(Texture* text)
{
	texture = text;
	//game = GAME;
}
Ghost::Ghost(Texture* text, Game* GAME,uint iniCol, uint iniRow) : GameCharacter(text, GAME, iniCol, iniRow) 
{
}

Ghost::~Ghost()
{
}

void Ghost::loadFromFile()
{
}

void Ghost::saveToFile()
{
}


void Ghost::update() {
	move();
	frame++;
}


void Ghost::move() {
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