#pragma once
#include "Game.h"

class GameObject
{
protected:
	Game* game = nullptr;	//	Puntero	al	juego	al	que	pertenece

public:
	GameObject();
	virtual ~GameObject() = 0;
	virtual void render() = 0;
	virtual void update() = 0;
	virtual void loadFromFile(ifstream& file) = 0;
	virtual void saveToFile(ofstream& file) = 0;
};

