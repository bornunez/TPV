#pragma once

#include "GameCharacter.h"

class PacMan : public GameCharacter
{

private:
	Direction buffer = None;
	uint life = 3;
	uint energy;
	uint dieLapse = 2000; //Tiempo de espera cuando muero hasta que vuelve a mover (En ms)
	bool dead = false;

public:
	PacMan();
	PacMan(Texture* text, Game* GAME, uint iniCol, uint iniRow, uint w, uint h);
	~PacMan();
	void update();
	void loadFromFile(ifstream& file);
	void saveToFile(ofstream& file);
	bool die();
	void isDead() { if (dead) { SDL_Delay(dieLapse); dead = false; }}
	void setDir(Direction ndir) { buffer = ndir; }
	void setLifes(int lifes) { life = lifes; }
	int lifes() { return life; }
};

