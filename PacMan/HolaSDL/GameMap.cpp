#include "GameMap.h"

GameMap::GameMap()
{
	
}

// Constructor que asigna al mapa las texturas del muro, la comida y la vitamina.
GameMap::GameMap(Texture* wall, Texture* food, Texture* vitamin)
{
	this->wall = wall;
	this->food = food;
	this->vitamin = vitamin;
}

GameMap::~GameMap()
{
}

void GameMap::iniMapCell() {
	mapCell = new MapCell * [numRows];
	for (int i = 0; i < numRows; i++) {
		mapCell[i] = new MapCell[numCols];
	}
}

// Pinta el mapa, ver como quitar los argumentos para no tener que pasarlos cada vez, preguntar si hacer estaticas las variables
//E incluir aqui game.h
void GameMap::render(int cellWitdth, int cellHeight) {
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			//Dependiendo del valor de la casilla se pinta el sprite
			if (getCell(i, j) == Wall)
				wall->renderFrame({j * cellWitdth, i * cellHeight, cellWitdth, cellHeight}, 0, 0);
			else if (getCell(i, j) == Food)
				food->renderFrame({ j * cellWitdth, i * cellHeight, cellWitdth, cellHeight }, 0, 0);
			else if (getCell(i, j) == Vitamins)
				vitamin->renderFrame({ j * cellWitdth, i * cellHeight, cellWitdth, cellHeight }, 0, 0);
		}	
	}
}
