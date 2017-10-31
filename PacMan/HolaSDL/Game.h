#pragma once
#include "SDL.h"
#include "GameObject.h"
#include <vector>
#include <iostream>
class	Game {
private:
	//CONSTANTES
	const int NUM_GO = 9;
	const int NUM_TXTS = 4;
	const string TEXT_PATH = "..\\images\\";
	const int TICK_SPEED = 50;
	//PROPIEDADES Y VARIABLES DE LA VENTANA
	SDL_Window*	window = nullptr;
	SDL_Renderer* renderer = nullptr;
	uint winWidth = 800;
	uint winHeight = 600;
	int winX, winY; // Posición de la ventana
	//PROPIEDADES Y VARIABLES DEL JUEGO (Flags de control)
	bool exit = false;
	Texture* Textures[4];
	GameObject* gameObjects[9];

	//METODOS AUXILIARES
	void loadGO();
	void loadTextures();

public:
	Game();
	~Game();
	void run();
	void render();
	void update();
	void handleEvents();
};
