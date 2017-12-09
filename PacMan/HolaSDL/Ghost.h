#pragma once
#include "GameCharacter.h"

class Game;

class Ghost : public GameCharacter
{
private:
	vector<Direction> dirs;

	void move();

public:
	Ghost();
	Ghost(Texture* text);
	Ghost(Texture* text,Game* GAME, uint iniCol = 0, uint iniRow = 0);
	~Ghost();
	void loadFromFile();
	void saveToFile();
	void update();
	void set(int X, int Y) { x = X; y = Y; }
	void die() { x = iniX; y = iniY; }
};