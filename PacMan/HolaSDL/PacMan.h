#pragma once
#include "GameObject.h"

class PacMan : public GameObject
{
private:;
	uint life, energy;
	//nextDir ------------ Ver que tipo es

public:
	PacMan(Texture* text, Game* game);
	PacMan(Texture* text, Game* game, uint X, uint Y, uint W, uint H, uint iniCol = 0, uint iniRow = 0);
	~PacMan();
	void update();
	void die();
	void nextDir();
	uint getLife() { return life; }
};
