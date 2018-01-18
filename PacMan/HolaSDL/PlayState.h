#pragma once
#include <vector>
#include "Text.h"
#include "Score.h"
#include "GameState.h"
#include "FileNotFoundError.h"
#include "FileFormatError.h"

class PacMan;
class GameMap;


class PlayState : public GameState
{
private:


	const int SMARTGHOST_SPRITE = 8;	//Columna de la imagen que contiene el sprite de SmartGhost
	const int GUI_Ratio = 20; //Porcentaje de la pantalla que ocupa el GUI a la derecha
	const int MAX_LEVEL = 4;

	const int FRAME_RATE = 100;
	const int NUM_SCORES_TOP = 10;

	int MAP_ROWS;
	int MAP_COLS;
	int TILE_H, TILE_W;


	//PROPIEDADES Y VARIABLES DE LA VENTANA
	//SDL_Window*	window = nullptr;
	//SDL_Renderer* renderer = nullptr;

	Region canvas{ 0,0 };
	Region GUI{ 0,0 };
	int winX, winY; // Posición de la ventana

	//PROPIEDADES Y VARIABLES DEL JUEGO (Flags de control)
	bool menu = true;
	bool exit = false;
	bool error = false;
	bool gameOver = false;
	bool saveSt = false;
	int foodCount = 0;

	string userCode;

	bool powered = false;
	uint32_t auxTime = 0;

	// Registro de puntuacion
	Score scoreTable;
	int score = 0;
	int level = 0;
	
	bool hasSaveFile = false; //Será true si existe un archivo de guardado
	ScoreReg actualUserReg;			// Registro que se asigna si un usuario inicia sesion y ya estaba registrado previamente
	Font* gameFont;


	//VARIABLES DEL JUEGO
	GameMap* gameMap;
	
	//Text* texts[NUM_TEXTS];
	uint numGhost;
	PacMan* pacMan;
	//list<GameCharacter*> characters;

	//METODOS AUXILIARES
	//void loadTextures();
	bool loadLevel(string filename, bool saved);
	void screenRatioConfig();
	void login();
	//void manageScoreTable();
	void adjustTexts(); //Ajusta los textos en funcion del GUI
	void destroyGhosts();
	void checkLevel();
	//uint writeCode();
	//void endGame();
	bool freeSpace(int x, int y, int& nx, int& ny);
	bool isGhost(int x, int y);
	void pacCollision();
	void fantCollision();
	void updateGhosts();

public:
	PlayState(Game* game,string userCode = " ");
	~PlayState();
	void render();
	void update();
	void handleEvent(SDL_Event& event);

	//METODOS DEL CICLO DE JUEGO PARA LOS COMPONENTES
	void collision();
	void powerUp();
	void eat() { foodCount -= 1; score++; }
	//bool save(string filename);
	//void saveState();

	//GETS Y SETS
	MapCell getCell(int row, int col);
	int getPacPosX();
	int getPacPosY();
	int getRows();
	int getCols();
	int getTileWidth() { return TILE_W; }
	int getTileHeight() { return TILE_H; }
	void setCell(int row, int col, MapCell type);
	void addFood() { foodCount++; }				//Quiza hay que llevarse foodCount a GameMap
	bool isPowered() { return powered; }
	void saveToFile(string userCode);
};


