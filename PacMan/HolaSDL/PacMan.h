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
	PacMan(Texture* text, PlayState* playState, Game* game, uint iniCol, uint iniRow, uint w, uint h);
	~PacMan();
	void update();
	void loadFromFile(ifstream& file);
	void saveToFile(ofstream& file);
	bool handleEvent(SDL_Event& event);
	void die();
	bool isDead() { return dead; }
	void setDir(Direction ndir) { buffer = ndir; }
	void setLifes(int lifes) { life = lifes; }
	void resetEnergy() { energy = 0; }
	bool hasEnergy() { return (energy > 0); }
	int lifes() { return life; }
};

