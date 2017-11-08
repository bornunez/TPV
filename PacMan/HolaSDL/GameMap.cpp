#include "GameMap.h"


GameMap::GameMap() {

}

GameMap::GameMap(Texture* cellTex, Texture* food, Texture* powerUp)
{
	CellTex = cellTex;
	FoodTex = food;
	PowerUpTex = powerUp;
}

GameMap::~GameMap()
{
	for (int i = 0; i < ROWS; i++) {
		delete mapCell[i];
	}
	delete mapCell;
}
void GameMap::initMap() {
	//INICIAMOS EL MAPA
	mapCell = new MapCell *[ROWS];
	for (int i = 0; i < COLS; i++) {
		mapCell[i] = new MapCell[COLS];
	}
}

void GameMap:: render(int frameW, int frameH){
	SDL_Rect destRect;
	for(int i=0; i<ROWS;i++)
		for (int j = 0; j < COLS; j++) {
			destRect.h = frameH;
			destRect.w = frameW;
			destRect.x = j*frameW;
			destRect.y = i*frameH;
			//Y RENDERIZAMOS
			if (mapCell[i][j] == Wall)
				CellTex->render(destRect);
			else if (mapCell[i][j] == Food)
				FoodTex->render(destRect);
			else if (mapCell[i][j] == PowerUp)
				PowerUpTex->render(destRect);
		}
}