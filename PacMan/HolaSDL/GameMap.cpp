#include "GameMap.h"


GameMap::GameMap()
{
	
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


void GameMap::render(uint cellWitdth, uint cellHeight) {
//PINTAR EL MAPA
//Se crea un Rect y se le asigna el tamaño que tendra cada sprite
SDL_Rect dest;
dest.w = cellWitdth;
dest.h = cellHeight;

for (int i = 0; i < numRows; i++) {
	for (int j = 0; j < numCols; j++) {

		dest.x = j * cellWitdth;
		dest.y = i * cellHeight;

		//Dependiendo del valor de la casilla se pinta el sprite
		if (getCell(i, j) == Wall)
			wall->renderFrame(dest, 0, 0);
		else if (getCell(i, j) == Food)
			food->renderFrame(dest, 0, 0);
		else if (getCell(i, j) == Vitamins)
			vitamin->renderFrame(dest, 0, 0);
}
}

}