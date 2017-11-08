#pragma once
#include "SDL.h"
#include "Texture.h"
#include "Utilities.h"

class Game;


class Ghost
{
private:
	Texture* texture = nullptr;	//	Puntero	a	su textura
	Game* game = nullptr;	//	Puntero	al	juego	al	que	pertenece

							//PROPIEDADES DEL RENDER
	uint texRow = 0; uint texCol = 0;	//	Posición del frame en la textura
	uint w = 100; uint h = 100;
	uint x = 0; uint y = 0;

	//PROPIEDADES DE GHOST
	Direction dir = None;
	int dirX = 0; int dirY = 0;
	//Direction dir;
	bool PowerUp = false;

	bool next(int& nx, int& ny, Direction dir); //Dada unadireccion, decimos si se puede mover a la siguiente casilla. nx y ny contienen la nueva casilla
	void EnumToDir(Direction dir, int& ndx, int& ndy); //Dada una direccion, devolvemos la direccion en forma numerica
	void mueve();
	Direction backDir(Direction currDir);
public:

	Ghost();
	Ghost(Texture* text);
	Ghost(Texture* text, uint X, uint Y, uint W, uint H,Game* GAME, uint iniCol = 0, uint iniRow = 0);
	~Ghost();
	void render();
	void update();
};

