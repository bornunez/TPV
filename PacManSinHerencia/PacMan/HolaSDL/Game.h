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
const uint NUM_TEXTURES = 4;


class	Game {
private:
	SDL_Window*	window = nullptr;
	SDL_Renderer* renderer = nullptr;

	

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
	// Tamaño celdas posiblemente estatico
	uint cellWitdth;
	uint cellHeight;
	Game();
	~Game();
	void run();
	void render();
	void update();
	void handleEvents();
	void init();
	bool nextCell();
	void loadTextures();
	void loadMap();
	void iniPositions(uint goNumber, uint iniX, uint iniY);
};
