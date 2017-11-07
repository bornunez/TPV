#pragma once
#include "SDL.h"
#include "Texture.h"


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

	//PROPIEDADES DE PACMAN
	int dirX = 0; int dirY = 0;
	bool PowerUp = false;

public:

	Ghost();
	Ghost(Texture* text);
	Ghost(Texture* text, uint X, uint Y, uint W, uint H, uint iniCol = 0, uint iniRow = 0);
	~Ghost();
	void render();
	void update();
};

