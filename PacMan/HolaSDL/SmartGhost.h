#pragma once
#include "Ghost.h"

class SmartGhost : public Ghost
{
private:
	int age = 0; //Entero que indica la edad del fant (0 es joven, 1 es adulto, 2 es viejo)
	int auxAge = 0;
	int maxAge = 30000; //Tiempo de vida (en ms)
	bool old = false;
	bool child = false;

	void smartMove();
	void closestDir();
public:
	SmartGhost(Texture* text, Game* game, uint iniCol, uint iniRow, uint w, uint h);
	~SmartGhost();

	bool hasChild() { return child; }
	void isFather() { child = true; }
	bool isOld() { return old; }
	bool isAdult() { return !isOld() && age > maxAge / 2; }
	void update();
	void loadFromFile(ifstream& file);
	void saveToFile(ofstream& file);
};

