#pragma once
#include "SDL.h"
#include "GameMap.h"
#include "GameObject.h"
#include <vector>
#include <iostream>
#include "Pac_Man.h"
#include "Ghost.h"

//Nombre de las texturas 
enum TextureName {Characters, Background, FoodText, PowerUpText};

class Game {
private:
	//CONSTANTES
	const int NUM_TXTS = 4;
	const int NUM_GHOST = 4;
	const string TEXT_PATH = "..\\images\\";
	const string LEVEL_PATH = "..\\levels\\";
	const uint32_t POWERTIME = 5000; // Tiempo que Pac Man esta OP (En ms)
	const int GUI_Ratio = 20; //Porcentaje de la pantalla que ocupa el GUI a la derecha

	const int FRAME_RATE = 100;
	int MAP_ROWS;
	int MAP_COLS;
	int TILE_H, TILE_W;
	
	// Estructura para las texturas
	typedef struct {
		string filename;
		int numRows;
		int numCols;
	} TextureAtributes; 
	
	const TextureAtributes TEXTURE_ATRIBUTES[4]
	{ {"characters1.png", 4, 14}, {"wall2.png", 1, 1} ,{"food2.png",1,1},{ "cereza.png",1,1 } };

	//PROPIEDADES Y VARIABLES DE LA VENTANA
	SDL_Window*	window = nullptr;
	SDL_Renderer* renderer = nullptr;
	uint winWidth = 900;
	uint winHeight = 750;

	uint canvasWidth = 0;
	uint canvasHeight = 0;

	uint GUIWidth = 0;
	uint GUIHeight = 0;

	int winX, winY; // Posición de la ventana

	//PROPIEDADES Y VARIABLES DEL JUEGO (Flags de control)
	bool exit = false;
	bool error = false;
	bool win = false;
	bool gameOver = false;
	int foodCount = 0;

	bool powered = false;
	uint32_t auxTime = 0;

	GameMap* gameMap;
	Texture* textures[4]; // | PERSONAJES | MURO | COMIDA | BONUS
	Ghost* ghosts[4];
	Pac_Man* pacMan;
	//Direction dir;

	//GAMEMAP 

	//METODOS AUXILIARES
	void loadTextures();
	void loadCharacters();
	bool loadMap(string filename);
	void screenRatioConfig();

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
	void eat() { foodCount -= 1; cout << foodCount << endl; }

	//GETS Y SETS
	MapCell getCell(int row, int col) { return gameMap->getCell(row,col); }
	int getRows() { return MAP_ROWS; }
	int getCols() { return MAP_COLS; }
	void setCell(int row, int col, MapCell type) { gameMap->setCell(row, col, type); }
	bool isPowered() { return powered; }
};
