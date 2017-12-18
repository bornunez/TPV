#pragma once
#include "SDL.h"
#include "Texture.h"
#include "Utilities.h"
#include <vector>

class Game;


class Ghost
{
private:
	Texture* texture = nullptr;	//	Puntero	a	su textura
	Game* game = nullptr;		//	Puntero	al	juego	al	que	pertenece

	//PROPIEDADES DEL RENDER
	uint texRow = 0; uint texCol = 0;	//	Posici�n del frame en la textura
	uint w = 100; uint h = 100;
	uint x = 0; uint y = 0;
	uint frame = 0;

	//PROPIEDADES DE GHOST
	Direction dir = None;
	vector<Direction> dirs;
	int dirX = 0; int dirY = 0;
	int iniX = 0; int iniY = 0;
	//Direction dir

	bool next(int& nx, int& ny, Direction dir); //Dada una direccion, decimos si se puede mover a la siguiente casilla. nx y ny contienen la nueva casilla
	void mueve();

public:

	Ghost();
	Ghost(Texture* text);
	Ghost(Texture* text,Game* GAME, uint iniCol = 0, uint iniRow = 0);
	~Ghost();
	void init(int iniPosX, int iniPosY, uint W, uint H);
	void set(int X, int Y) { x = X; y = Y; }
	void render();
	void update();
	void Die() { x = iniX; y = iniY; }
	int getX() { return x; }
	int getY() { return y; }
	int getIniX() { return iniX; }
	int getiniY() { return iniY; }
};