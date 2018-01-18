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
	virtual bool handleEvent(SDL_Event& event) = 0;
};

