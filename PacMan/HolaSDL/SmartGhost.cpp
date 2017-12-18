#include "SmartGhost.h"



SmartGhost::SmartGhost(Texture* text, Game* game, uint iniCol, uint iniRow, uint w, uint h) : Ghost(text, game, iniCol, iniRow, w ,h)
{
	Ghost::type = 1;
}


SmartGhost::~SmartGhost()
{
}

void SmartGhost::update()
{
}

void SmartGhost::loadFromFile(ifstream& file)
{
	Ghost::loadFromFile(file);
	file >> age;
}

void SmartGhost::saveToFile(ofstream& file)
{
	Ghost:saveToFile(file);
	file << " " << age;
}
