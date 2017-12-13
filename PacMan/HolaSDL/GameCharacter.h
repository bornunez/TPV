#pragma once
#include "GameObject.h"

class GameCharacter : public GameObject
{

protected:
	Texture* texture = nullptr;	//	Puntero	a	su textura

	//PROPIEDADES DEL RENDER
	uint texRow = 0; uint texCol = 0;	//	Posición del frame en la textura
	uint frame = 0;
	uint w = 100; uint h = 100;
	uint x = 0; uint y = 0;
	uint iniX = 0; uint iniY = 0;

	//PROPIEDADES DEL PERSONAJE
	Direction dir = None;
	int dirX = 1; int dirY = 0;

	//Metodos auxiliares
	bool next(int& nx, int& ny, Direction dir); //Dada una direccion, decimos si se puede mover a la siguiente casilla. nx y ny contienen la nueva casilla

public:
	GameCharacter();
	GameCharacter(Texture* text, Game* game, uint iniCol, uint iniRow, uint w, uint h);
	~GameCharacter();

	void set(int X, int Y) { x = X; y = Y; }
	int getX() { return x; }
	int getY() { return y; }
	int getIniX() { return iniX; }
	int getiniY() { return iniY; }
	void init(int iniPosX, int iniPosY, uint W, uint H);

	void render();
	void loadFromFile(ifstream& file);
	void saveToFile(ofstream& file);
};

