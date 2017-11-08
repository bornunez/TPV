#pragma once
#include "Texture.h"
#include "SDL.h"
//#include "Game.h";
class Game;
class	GameObject {
private:
	Texture* texture = nullptr;	//	Puntero	a	su textura
	Game* game = nullptr;	//	Puntero	al	juego	al	que	pertenece

	uint texRow = 0; uint texCol = 0;	//	Posición del frame en la textura
public:
	uint w = 100;uint h = 100;
	uint x = 0; uint y = 0;
	GameObject();	
	GameObject(Texture* text);
	GameObject(Texture* text, uint X, uint Y, uint W, uint H,uint iniCol = 0, uint iniRow = 0);
	~GameObject();
	void render();
	void update();
};

