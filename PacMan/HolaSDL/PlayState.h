#pragma once
#include <vector>
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
	const int MAX_LEVEL = 0;	//A 0 para solo tener que pasar el primero para probar la funcionalidad

	const int FRAME_RATE = 100;
	const int NUM_SCORES_TOP = 10;

	int MAP_ROWS;
	int MAP_COLS;
	int TILE_H, TILE_W;


	//PROPIEDADES Y VARIABLES DE LA VENTANA
	Region canvas{ 0,0 };
	Region GUI{ 0,0 };
	int winX, winY; // Posición de la ventana

	//PROPIEDADES Y VARIABLES DEL JUEGO (Flags de control)
	bool gameOver = false;
	bool powered = false;

	int foodCount = 0;
	uint32_t auxTime = 0;

	string userCode;

	// Registro de puntuacion
	Score scoreTable;
	int score = 0;
	int level = 0;
	
	bool hasSaveFile = false; //Será true si existe un archivo de guardado
	ScoreReg actualUserReg;			// Registro que se asigna si un usuario inicia sesion y ya estaba registrado previamente

	//VARIABLES DEL JUEGO
	GameMap* gameMap;
	uint numGhost;
	PacMan* pacMan;

	//METODOS AUXILIARES
	bool loadLevel(string filename, bool saved);
	void screenRatioConfig();
	void adjustTexts(); //Ajusta los textos en funcion del GUI
	void destroyGhosts();
	void checkLevel();
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

	//GETS Y SETS
	MapCell getCell(int row, int col);
	int getPacPosX();
	int getPacPosY();
	int getRows();
	int getCols();
	int getTileWidth() { return TILE_W; }
	int getTileHeight() { return TILE_H; }
	void setCell(int row, int col, MapCell type);
	void addFood() { foodCount++; }				
	bool isPowered() { return powered; }
	void saveToFile(string userCode);
};


