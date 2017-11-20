#pragma once
#include "SDL.h"
#include "GameMap.h"
#include "GameObject.h"
#include <vector>
#include <iostream>
#include <sstream>
#include "Pac_Man.h"
#include "Ghost.h"
#include "Text.h"
#include "Score.h"

//Nombre de las texturas 
enum TextureName {Characters, Background, FoodTexture, PowerUpTexture,SpriteFont};
enum TextName{ScoreText,MaxScoreText, UserText, LevelText, TitleText};

//CONSTANTES DE ARRAYS 
const int NUM_TEXTURES = 5;
const int NUM_GHOST = 4;
const int NUM_TEXTS = 3;

class Game {
private:
	//CONSTANTES
	const string TEXT_PATH = "..\\images\\";
	const string LEVEL_PATH = "..\\levels\\";
	const string SCORETABLE_PATH = "..\\users\\scoretable.txt";
	const uint32_t POWERTIME = 5000; // Tiempo que Pac Man esta OP (En ms)
	const int GUI_Ratio = 20; //Porcentaje de la pantalla que ocupa el GUI a la derecha

	const int FRAME_RATE = 100;
	const int NUM_SCORES_TOP = 10;

	int MAP_ROWS;
	int MAP_COLS;
	int TILE_H, TILE_W;
	
	const TextureAtributes TEXTURE_ATRIBUTES[NUM_TEXTURES]
	{ {"characters1.png", 4, 14}, {"wall2.png", 1, 1} ,{"food2.png",1,1},{ "cereza.png",1,1 },{"spritefont.png",5,13} };
	

	//PROPIEDADES Y VARIABLES DE LA VENTANA
	SDL_Window*	window = nullptr;
	SDL_Renderer* renderer = nullptr;

	Region windowReg{ 1000, 750};

	Region canvas{ 0,0 };

	Region GUI{ 0,0 };

	int winX, winY; // Posición de la ventana

	//PROPIEDADES Y VARIABLES DEL JUEGO (Flags de control)
	bool exit = false;
	bool error = false;
	bool win = false;
	bool gameOver = false;
	int foodCount = 0;

	bool powered = false;
	uint32_t auxTime = 0;

	// Registro de puntuacion
	Score scoreTable;
	int score = 0;
	int level = 1;
	string userName;
	bool validateUser = false;		// Sera true si el jugador ya tiene un usuario registrado en el juego
	bool hasSaveFile = false; //Será true si existe un archivo de guardado
	ScoreReg actualUserReg;			// Registro que se asigna si un usuario inicia sesion y ya estaba registrado previamente

	//VARIABLES DEL JUEGO
	GameMap* gameMap;
	Texture* textures[NUM_TEXTURES]; // | PERSONAJES | MURO | COMIDA | BONUS
	Text* texts[NUM_TEXTS];
	Ghost* ghosts[NUM_GHOST];
	Pac_Man* pacMan;

	//METODOS AUXILIARES
	void loadTextures();
	void loadCharacters();
	void loadText();
	bool loadMap(string filename, bool saved);
	void screenRatioConfig();
	void login();
	void manageScoreTable();
	bool is_Ghost(int& rows, int& cols, int& ghost_num);
	void adjustTexts(); //Ajusta los textos en funcion del GUI
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
	void eat() { foodCount -= 1; score++; }
	bool save(string filename);


	//GETS Y SETS
	MapCell getCell(int row, int col) { return gameMap->getCell(row,col); }
	int getRows() { return MAP_ROWS; }
	int getCols() { return MAP_COLS; }
	void setCell(int row, int col, MapCell type) { gameMap->setCell(row, col, type); }
	bool isPowered() { return powered; }
};
