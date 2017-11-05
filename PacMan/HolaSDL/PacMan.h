#pragma once
#include "Texture.h";
class Game;

class PacMan {
private:;
	uint life, energy;
	Texture* texture = nullptr;				//	Puntero	a	su textura
	Game* game = nullptr;					//	Puntero	al	juego	al	que	pertenece
	uint texRow = 0; uint texCol = 0;		//	Posición del frame en la textura
	uint w = 100;uint h = 100;				//Tamaño en pantalla del GO
	//nextDir ------------ Ver que tipo es

public:
	uint x = 0; uint y = 0;					//Posicion actual 
	uint iniX = 0; uint iniY = 0;			//Posicion inicial 

	PacMan(Texture* text, Game* game);
	PacMan(Texture* text, Game* game, uint X, uint Y, uint W, uint H, uint iniCol = 0, uint iniRow = 0);
	~PacMan();
	void setIniPosition(uint iniX, uint iniY);
	void setActualPosition(uint x, uint y);
	void render();
	void update();
	void die();
	void nextDir();
	uint getLife() { return life; }
};
