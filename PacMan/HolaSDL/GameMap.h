#pragma once
#include "GameObject.h"

using namespace std;

//enum MapCell { Empty, Wall, Food, PowerUp};

struct Position
{
	int X, Y;
};

class GameMap : protected GameObject
{
private:
	int rows, cols;
	Texture* CellTex;
	Texture* FoodTex;
	Texture* PowerUpTex;
	MapCell** mapCell;

public:
	GameMap();
	GameMap(Game* game, Texture* cellTex, Texture* food, Texture* powerUp);
	~GameMap();
	void initMap();

	//GETS Y SETS
	void setRows(int rows) { this->rows = rows; }
	void setCols(int cols) { this->cols = cols; }
	MapCell getCell(int rows, int cols) { return mapCell[rows][cols]; }
	void setCell(int rows, int cols, MapCell cellType) { mapCell[rows][cols] = cellType; }

	void render();
	void update();
	void loadFromFile();
	void saveToFile();
};

