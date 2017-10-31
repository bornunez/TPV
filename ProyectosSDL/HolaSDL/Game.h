#pragma once
#include "SDL.h";
#include "PacMan.h";
#include "Ghost.h";
#include "GameMap.h";
#include "Texture.h"; 
#include <array>
#include <iostream>
#include <fstream>	

const uint FRAME_RATE = 50;
const uint WIN_WIDTH = 800;
const uint WIN_HEIGHT = 600;
const std::string IMAGES_PATH = "";
const uint NUM_TEXTURES = 3;


class	Game {
private:
	SDL_Window*	window = nullptr;
	SDL_Renderer* renderer = nullptr;

	// Tama�o celdas
	uint cellWitdth;
	uint cellHeight;

	//Booleanos de control
	bool exit = false;
	bool gameOver = false;
	bool win = false;

	// Numero de casillas con comida
	uint foodCells;

	GameMap *map;
	PacMan *pacMan;
	//Ghost* ghosts[4];
	array<Ghost*, 4> ghosts; //usar class array o hacerlo normal?
	array<Texture*, NUM_TEXTURES> textures;
	//Texture* textures[NUM_TEXTURES];

public:
	Game();
	~Game();
	void run();
	void render();
	void update();
	void handleEvents();
	void init();
	bool nextCell();
	void loadMap();
};
