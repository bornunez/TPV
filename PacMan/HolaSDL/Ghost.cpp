#include "Ghost.h"


Ghost::Ghost() {}

Ghost::Ghost(Texture* text)
{
	texture = text;
}

Ghost::Ghost(Texture* text, Game* game,uint iniCol, uint iniRow, uint w, uint h) : GameCharacter(text, game, iniCol, iniRow, w, h) 
{
	type = 0;
}

Ghost::~Ghost()
{
}

//PINTA GHOST EN PANTALLA
void Ghost::render()
{
	destRect.x = x*iniW; destRect.y = y*iniH;
	destRect.w = w; destRect.h = h;

	if (game->isPowered())
		texture->renderFrame(destRect, frame % 2, 12 + (frame % 2));
	else
		GameCharacter::render();
}

//ACTUALIZA EL MOVIMIENTO Y ANIMACIONES
void Ghost::update() {
	move();
	frame++;
}

//CARGA DE FICHERO LOS ARCHIVOS NECESARIOS PARA GHOST
void Ghost::loadFromFile(ifstream& file)
{
	GameCharacter::loadFromFile(file);
}

//GUARDA EN FICHERO LOS ARCHIVOS DE GHOST NECESARIOS
void Ghost::saveToFile(ofstream& file)
{
	file << type << " ";
	GameCharacter::saveToFile(file);
}

//CALCULA UNA DIRECCION Y MUEVE AL FANTASMA
void Ghost::move() {
	getNearDirs(dirs);
	int nx, ny;
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

void Ghost::getNearDirs(vector<Direction>& nearDirs)
{
	dirs.clear();
	//Vamos a guardar las direcciones posibles, de donde luego cogeremos una
	int nx, ny;
	for (int i = 0; i < 4; i++) {
		if (next(nx, ny, (Direction)i))
			nearDirs.push_back((Direction)i);
	}
	//Si hay mas de una posicion donde moverse
	if (nearDirs.size() > 1) {
		int i = 0;
		Direction back = Utilities::backDir(dir);
		//Buscamos a ver si esta la contraria a nuestro movimiento actual
		while (i < nearDirs.size() && dirs[i] != back)
			i++;
		if (i < nearDirs.size()) //En cuyo caso la eliminamos
			nearDirs.erase(nearDirs.begin() + i);
	}
}
