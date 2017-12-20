#pragma once
#include "GameCharacter.h"


const int MAX_ENERGY = 100;

class PacMan : public GameCharacter
{

private:
	Direction buffer = None;
	uint life = 3;
	uint energy = 0;
	uint dieLapse = 2000; //Tiempo de espera cuando muero hasta que vuelve a mover (En ms)
	bool dead = false;

public:
	PacMan();
	PacMan(Texture* text, Game* GAME, uint iniCol, uint iniRow, uint w, uint h);
	~PacMan();
	void update();
	void loadFromFile(ifstream& file);
	void saveToFile(ofstream& file);
	void die();
	bool isDead() { return dead; }
	void setDir(Direction ndir) { buffer = ndir; }
	void setLifes(int lifes) { life = lifes; }
	bool hasEnergy() { return (energy > 0); }
	int lifes() { return life; }
};

