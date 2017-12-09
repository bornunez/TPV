#pragma once
#include "Game.h"

class GameObject
{
protected:
	Game* game = nullptr;	//	Puntero	al	juego	al	que	pertenece

public:
	GameObject();
	~GameObject();
	virtual void render() = 0;
	virtual void update() = 0;
	virtual void loadFromFile() = 0;
	virtual void saveToFile() = 0;
};

