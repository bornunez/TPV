#pragma once
#include	"Texture.h"
class Game;
using	namespace	std;

enum MapCell { Empty, Wall, Food, Vitamins };


class GameMap
{

private:
	int numRows;
	int numCols;
	MapCell** mapCell;

public:
	Texture* wall;
	Texture* food;
	Texture* vitamin;

	GameMap();
	GameMap(Texture* wall, Texture* food, Texture* vitamin);
	~GameMap();

	void render(int cellWitdth, int cellHeight);
	void iniMapCell();
	void setCell(int row, int col, MapCell obj) { mapCell[row][col] = obj; }
	MapCell getCell(int row, int col) { return mapCell[row][col]; }
	void setNumRows(uint rows) { numRows = rows; }
	void setNumCols(uint cols) { numCols = cols; }
	int getNumCols() { return numCols; }
	int getNumRows() { return numRows; }
};