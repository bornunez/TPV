#pragma once
#include "Ghost.h"

class SmartGhost : public Ghost
{
private:
	int age = 0;

public:
	SmartGhost();
	~SmartGhost();

	void update();
	void loadFromFile();
	void saveToFile();
};

