#pragma once
#include "GameCharacter.h"

class Game;

class Ghost : public GameCharacter
{
protected:
	vector<Direction> dirs;
	uint type;
	void move();

public:
	Ghost();
	Ghost(Texture* text);
	Ghost(Texture* text, Game* GAME, uint iniCol, uint iniRow, uint w, uint h);
	~Ghost();
	void loadFromFile(ifstream& file);
	void saveToFile(ofstream& file);
	void update();
	void set(int X, int Y) { x = X; y = Y; }
	void die() { x = iniX; y = iniY; }
};