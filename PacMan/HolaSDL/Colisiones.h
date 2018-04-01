#pragma once
#include "GameState.h"
#include "Personaje.h"
#include "Rect.h"
#include "Physics.h"
class Colisiones :
	public GameState
{
private:
	Personaje* left;
	Personaje* right;
	Personaje* pared;
public:
	Colisiones(Game* game);
	virtual ~Colisiones();
	void update();
	
};

