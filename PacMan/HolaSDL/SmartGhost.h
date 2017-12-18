#pragma once
#include "Ghost.h"

class SmartGhost : public Ghost
{
private:
	int age = 0;

public:
	SmartGhost(Texture* text, Game* game, uint iniCol, uint iniRow, uint w, uint h);
	~SmartGhost();

	void update();
	void loadFromFile(ifstream& file);
	void saveToFile(ofstream& file);
};

