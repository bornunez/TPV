#include "GameMap.h"


GameMap::GameMap()
{
	
}


GameMap::~GameMap()
{
}


/*
void GameMap::render(SDL_Renderer*	renderer) {
//PINTAR EL MAPA
//Se crea un Rect y se le asigna el tamaño que tendra cada sprite
SDL_Rect dest;
dest.w = Game::cellWitdth;
dest.h = Game::cellHeight;

for (int i = 0; i < getnumRows(); i++) {
for (int j = 0; j < getnumCols(); j++) {

dest.x = j *  Game::cellWitdth;
dest.y = i * Game::cellHeight;

//Dependiendo del valor de la casilla se pinta el sprite
if (getCell(i, j) == Wall)
wall->renderFrame(renderer, dest, 0, 0);
else if (getCell(i, j) == Food)
food->renderFrame(renderer, dest, 0, 0);
else if (getCell(i, j) == Vitamins)
vitamin->renderFrame(renderer, dest, 0, 0);
}
}

}*/