	#pragma once
#include "Game.h"
#include "Rect.h"

class GameObject
{
protected:
	Game* game = nullptr;	//	Puntero	al	juego	al	que	pertenece
	Rect* rect = nullptr;
public:
	GameObject();
	GameObject(Rect* rect) : rect(rect) {};
	virtual ~GameObject() = 0;
	virtual void render() = 0;
	virtual void update() = 0;
	virtual bool handleEvent(SDL_Event& event) = 0;
};

