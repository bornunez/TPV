#pragma once
#include "Texture.h";
class Game;

class	Ghost {
private:
	Texture* texture = nullptr;				//	Puntero	a	su textura
	Game* game = nullptr;					//	Puntero	al	juego	al	que	pertenece
	uint texRow = 0; uint texCol = 0;		//	Posición del frame en la textura
	uint w = 100;uint h = 100;				//Tamaño en pantalla del GO

public:
	uint x = 0; uint y = 0;					//Posicion actual 
	uint iniX = 0; uint iniY = 0;			//Posicion inicial 
	//actualDir ------------ Ver que tipo es (direccion actual del fantasma

	Ghost(Texture* text, Game* game);
	Ghost(Texture* text, Game* game, uint X, uint Y, uint W, uint H, uint iniCol = 0, uint iniRow = 0);
	~Ghost();
	void setIniPosition(uint iniX, uint iniY);
	void setActualPosition(uint x, uint y);
	void render();
	void update();
	void die();
};