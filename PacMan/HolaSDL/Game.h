#pragma once
#include "SDL.h"
#include "GameMap.h"
#include "GameObject.h"
#include <vector>
#include <iostream>
#include "Pac_Man.h"
#include "Ghost.h"

class Game {
private:
	//CONSTANTES
	const int NUM_TXTS = 4;
	const string TEXT_PATH = "..\\images\\";
	const string LEVEL_PATH = "..\\levels\\";
	const int TICK_SPEED = 100;
	int MAP_ROWS;
	int MAP_COLS;
	int TILE_H, TILE_W;

	//PROPIEDADES Y VARIABLES DE LA VENTANA
	SDL_Window*	window = nullptr;
	SDL_Renderer* renderer = nullptr;
	uint winWidth = 800;
	uint winHeight = 600;
	int winX, winY; // Posición de la ventana

	//PROPIEDADES Y VARIABLES DEL JUEGO (Flags de control)
	bool exit = false;
	bool error = false;
	bool win = false;
	int numComida;
	GameMap* gameMap;
	Texture* textures[4]; // | PERSONAJES | MURO | COMIDA | BONUS
	Ghost* ghosts[4];
	Pac_Man* PacMan;
	Direction dir;

	//METODOS AUXILIARES
	void loadTextures();
	bool loadMap(string filename);

public:
	Game();
	~Game();
	void run();
	void render();
	void update();
	void handleEvents();
	MapCell getCell(int row, int col) { return gameMap->getCell(row,col); }
	int getRows() { return MAP_ROWS; }
	int getCols() { return MAP_COLS; }
	void eat() { numComida--; }
	void setCell(int row, int col, MapCell type) { gameMap->setCell(row, col, type); }
};
