#pragma once
#include "SDL.h"
#include "Texture.h"
#include "Utilities.h"

class Game;
//enum Direction { Up, Down, Left, Right, None };

class Pac_Man
{
private:
	Texture* texture = nullptr;	//	Puntero	a	su textura
	Game* game = nullptr;	//	Puntero	al	juego	al	que	pertenece

	//PROPIEDADES DEL RENDER
	uint texRow = 0; uint texCol = 0;	//	Posici�n del frame en la textura
	uint w = 100; uint h = 100;
	uint x = 0; uint y = 0;
	
	//PROPIEDADES DE PACMAN
	Direction dir = None;
	Direction buffer = None;
	int dirX = 1; int dirY = 0;
	bool powerUp = false;

	//Metodos auxiliares
	bool next(int& nx, int& ny,Direction dir); //Dada unadireccion, decimos si se puede mover a la siguiente casilla. nx y ny contienen la nueva casilla
	void EnumToDir(Direction dir, int& ndx, int& ndy); //Dada una direccion, devolvemos la direccion en forma numerica
public:	
	Pac_Man();
	Pac_Man(Texture* text, Game* GAME);
	Pac_Man(Texture* text, uint X, uint Y, uint W, uint H, Game* GAME, uint iniCol = 0, uint iniRow = 0);
	~Pac_Man();
	void render();
	void setDir(Direction dir);
	void update();
};

