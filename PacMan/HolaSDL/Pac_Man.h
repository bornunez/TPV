#pragma once
#include "SDL.h"
#include "Texture.h"
#include "Utilities.h"

class Game;

class Pac_Man
{
private:
	Texture* texture = nullptr;	//	Puntero	a	su textura
	Game* game = nullptr;	//	Puntero	al	juego	al	que	pertenece

	//PROPIEDADES DEL RENDER
	uint texRow = 0; uint texCol = 0;	//	Posición del frame en la textura
	uint frame = 0;
	uint w = 100; uint h = 100;
	uint x = 0; uint y = 0;
	uint iniX = 0; uint iniY = 0;
	
	//PROPIEDADES DE PACMAN
	Direction dir = None;
	Direction buffer = None;
	int dirX = 1; int dirY = 0;
	int life = 3;
	uint dieLapse = 2000 ; //Tiempo de espera cuando muero hasta que vuelve a mover (En ms)
	bool dead = false;

	//Metodos auxiliares
	bool next(int& nx, int& ny,Direction dir); //Dada unadireccion, decimos si se puede mover a la siguiente casilla. nx y ny contienen la nueva casilla

public:	
	Pac_Man();
	Pac_Man(Texture* text, Game* GAME);
	Pac_Man(Texture* text, Game* GAME, uint iniCol = 0, uint iniRow = 0);
	~Pac_Man();
	void render();
	void setDir(Direction dir);
	void update();
	//GETS y SETS
	void init(int iniPosX, int iniPosY, uint W, uint H);
	void set(int X, int Y) { x = X; y = Y; }
	int getX() { return x; }
	int getY() { return y; }
	int getIniX() { return iniX; }
	int getiniY() { return iniY; }
	bool die();
	int lifes() { return life; }
	void setLifes(int lifes) { life = lifes; }
	void isDead() { if (dead) { SDL_Delay(dieLapse); dead = false; } }
};

