#include "GameMap.h"


GameMap::GameMap() {

}

GameMap::GameMap(Game* game, Texture* cellTex, Texture* food, Texture* powerUp)
{
	this->game = game;
	CellTex = cellTex;
	FoodTex = food;
	PowerUpTex = powerUp;
}

GameMap::~GameMap()
{
	for (int i = 0; i < rows; i++) {
		delete mapCell[i];
	}
	delete mapCell;
}

void GameMap::initMap() {
	//INICIAMOS EL MAPA
	mapCell = new MapCell *[rows];
	for (int i = 0; i < rows; i++) {
		mapCell[i] = new MapCell[cols];
	}
}

void GameMap::update()
{
}

void GameMap::loadFromFile()
{
}

void GameMap::saveToFile()
{
}

void GameMap:: render(){
	SDL_Rect destRect;
	for(int i=0; i < rows;i++)
		for (int j = 0; j < cols; j++) {
			destRect.h = game->getTileHeight();
			destRect.w = game->getTileWidth();
			destRect.x = j*game->getTileWidth();
			destRect.y = i*game->getTileHeight();
			//Y RENDERIZAMOS
			if (mapCell[i][j] == Wall)
				CellTex->render(destRect);
			else if (mapCell[i][j] == Food)
				FoodTex->render(destRect);
			else if (mapCell[i][j] == PowerUp)
				PowerUpTex->render(destRect);
		}
}