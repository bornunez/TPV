#pragma once
#include "SDL.h"
#include "Texture.h"
#include <vector>
#include <fstream>

using namespace std;

enum MapCell { Empty, Wall, Food, PowerUp};

struct Position
{
	int X, Y;
};

class GameMap
{
private:
	int ROWS, COLS;
	Texture* CellTex;
	Texture* FoodTex;
	Texture* PowerUpTex;
	
	MapCell** mapCell;
public:
	GameMap();
	GameMap(Texture* cellTex, Texture* food, Texture* powerUp);
	~GameMap();
	void render(int frameW, int frameH);
	void initMap();

	//GETS Y SETS
	void setRows(int rows) { ROWS = rows; }
	void setCols(int cols) { COLS = cols; }
	MapCell getCell(int rows, int cols) { return mapCell[rows][cols]; }
	void setCell(int rows, int cols, MapCell cellType) { mapCell[rows][cols] = cellType; }
};

