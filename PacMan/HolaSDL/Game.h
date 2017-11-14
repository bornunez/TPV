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
	const int NUM_GHOST = 4;
	const string TEXT_PATH = "..\\images\\";
	const string LEVEL_PATH = "..\\levels\\";
	const int TICK_SPEED = 125;
	int MAP_ROWS;
	int MAP_COLS;
	int TILE_H, TILE_W;

	//PROPIEDADES Y VARIABLES DE LA VENTANA
	SDL_Window*	window = nullptr;
	SDL_Renderer* renderer = nullptr;
	uint winWidth = 750;
	uint winHeight = 750;
	int winX, winY; // Posición de la ventana

	//PROPIEDADES Y VARIABLES DEL JUEGO (Flags de control)
	bool exit = false;
	bool error = false;
	bool win = false;
	bool gameOver = false;
	int numComida = 0;
	GameMap* gameMap;
	Texture* textures[4]; // | PERSONAJES | MURO | COMIDA | BONUS
	Ghost* ghosts[4];
	Pac_Man* PacMan;
	Direction dir;

	//GAMEMAP 

	//METODOS AUXILIARES
	void loadTextures();
	void loadCharacters();
	bool loadMap(string filename);


public:
	Game();
	~Game();
	void run();
	void render();
	void update();
	void handleEvents();

	//METODOS DEL CICLO DE JUEGO PARA LOS COMPONENTES
	void collision();
	void powerUp();
	void eat() { numComida -= 1; cout << numComida << endl; }

	//GETS Y SETS
	MapCell getCell(int row, int col) { return gameMap->getCell(row,col); }
	int getRows() { return MAP_ROWS; }
	int getCols() { return MAP_COLS; }
	void setCell(int row, int col, MapCell type) { gameMap->setCell(row, col, type); }
	void renderMap();
};
