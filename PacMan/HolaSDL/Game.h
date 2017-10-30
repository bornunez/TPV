#pragma once
#include "SDL.h"
#include "GameObject.h"
#include <vector>
#include <iostream>
class	Game {
private:
	SDL_Window*	window = nullptr;
	SDL_Renderer* renderer = nullptr;
	uint winWidth = 800;
	uint winHeight = 600;
	bool exit = false;
	int winX, winY; // Posición de la ventana
	//vector<GameObject*>	gameObjects;
public:
	Game();
	~Game();
	void run();
	void render();
	void update();
	void handleEvents();
	SDL_Renderer* GetRenderer() { return renderer; }
};
