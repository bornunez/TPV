#pragma once
#include "GameObject.h"
#include "Texture.h"
class Personaje :
	public GameObject
{
private:
	Texture * texture = nullptr;
public:
	Personaje(Rect* rect) : GameObject(rect) {};
	Personaje(Rect* rect, Texture* tex) : GameObject(rect), texture(tex) {};
	void render();
	void update() {}
	bool handleEvent(SDL_Event& event) { return false; }
	Rect* getRect() { return rect; }
	~Personaje();
};

