#include "SmartGhost.h"


SmartGhost::SmartGhost(Texture* text, Game* game, uint iniCol, uint iniRow, uint w, uint h) : Ghost(text, game, iniCol, iniRow, w, h)
{
	Ghost::type = 1;
	//auxAge = SDL_GetTicks();
}


SmartGhost::~SmartGhost()
{
}

void SmartGhost::update()
{
	//Establecemos la edad del fant
	//int tickTime = SDL_GetTicks();
	//age = tickTime - auxAge;

	//Y dependiendo de la edad, haremos cosas distintas
	if (age < (maxAge / 4)) {
		Ghost::update();
		h = iniH / 3;
		w = iniW / 3;
		age++;
	}
	else if (age < (maxAge / 2)) {
		Ghost::update();
		h = iniH / 2;
		w = iniW / 2;
		age++;
	}
	else if (age < maxAge) {
		h = iniH;
		w = iniW;
		cout << "ADULTO" << endl;
		smartMove();
		age++;
	}
	else {
		old = true;
		//Ghost::die();
	}
	frame++;
}

void SmartGhost::smartMove() {
	int nx, ny;
	dirs.clear();
	getNearDirs(dirs);
	//Si hay mas de 2 direcciones es una interseccion -> """"PATHFINDING""""
	if (dirs.size() > 1) {
		closestDir();
	}
	else if (dirs.size()>0)
		dir = dirs[0]; //Si no es trivial
					   //Y si no tiene donde moverse se queda quieto
	else
		dir = None;
	//Y nos movemos
	next(nx, ny, dir);
	x = nx;
	y = ny;
}

void SmartGhost::closestDir()
{
	//Pos de pacman
	int pacX = game->getPacPosX();
	int pacY = game->getPacPosY();
	//Distancia minima
	int minDist = 10000000;
	int minDir = 0;
	//Para cada casilla vacia adyacente, vamos a ver su ""coste"". Si es menor que el minimo actual, guardamos esa pos como prometedora.
	for (int i = 0; i < dirs.size(); i++) {
		//Primero hace falta saber la pos de esa direccion
		int dx, dy;
		next(dx, dy, dirs[i]);
		//Y ahora la distancia hasta el target
		int distX = pacX - dx;
		int distY = pacY - dy;
		int actDist = abs(distX) + abs(distY); //Asi calculamos la distancia absoluta
											   //Y nos quedamos con la menor
		if (actDist < minDist) {
			minDist = actDist;
			minDir = i;
		}
	}
	dir = dirs[minDir]; //Cogemos la direccion "optima"
}

void SmartGhost::loadFromFile(ifstream& file)
{
	Ghost::loadFromFile(file);
	file >> age;
}

void SmartGhost::saveToFile(ofstream& file)
{
	Ghost::saveToFile(file);
	file << " " << age;
}
