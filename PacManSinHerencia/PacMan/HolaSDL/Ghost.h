#pragma once
#include "GameObject.h"; //O .cpp
#include "SDL.h";

class	Ghost : public GameObject {
private:

public:
	//actualDir ------------ Ver que tipo es (direccion actual del fantasma

	Ghost(Texture* text, Game* game);
	Ghost(Texture* text, Game* game, uint X, uint Y, uint W, uint H, uint iniCol = 0, uint iniRow = 0);
	~Ghost();
	void update();
	void die();
};