#pragma once
#include "PacManObject.h"
#include "FileFormatError.h"

using namespace std;

//enum MapCell { Empty, Wall, Food, PowerUp};

struct Position
{
	int X, Y;
};

class GameMap : public PacManObject
{
private:
	int rows, cols;
	Texture* CellTex;
	Texture* FoodTex;
	Texture* PowerUpTex;
	MapCell** mapCell;

public:
	GameMap();
	GameMap(PlayState* playState, Game* game, Texture* cellTex, Texture* food, Texture* powerUp);
	~GameMap();
	void initMap();

	//GETS Y SETS
	uint getRows() { return rows; }
	uint getCols() {return cols; }
	MapCell getCell(int rows, int cols) { return mapCell[rows][cols]; }
	void setCell(int rows, int cols, MapCell cellType) { mapCell[rows][cols] = cellType; }

	void render();
	void update();
	void loadFromFile(ifstream& file);
	void saveToFile(ofstream& file);
	bool handleEvent(SDL_Event& e);
};

