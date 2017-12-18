#pragma once
#include "Ghost.h"

class SmartGhost : public Ghost
{
private:
	int age = 0; //Entero que indica la edad del fant (0 es joven, 1 es adulto, 2 es viejo)
	int auxAge = 0;
	int maxAge = 30000; //Tiempo de vida (en ms)
	void smartMove();
public:
	SmartGhost(Texture* text, Game* game, uint iniCol, uint iniRow, uint w, uint h);
	~SmartGhost();

	void update();
	void loadFromFile(ifstream& file);
	void saveToFile(ofstream& file);
};


