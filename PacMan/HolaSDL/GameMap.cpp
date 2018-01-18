#include "GameMap.h"


GameMap::GameMap() {

}

GameMap::GameMap(PlayState* playState, Game* game, Texture* cellTex, Texture* food, Texture* powerUp)
{
	this->game = game;
	CellTex = cellTex;
	FoodTex = food;
	PowerUpTex = powerUp;
	this->playState = playState; //moverlo a pacmanobject
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

//CARGA DE FICHERO LOS ARCHIVOS NECESARIOS PARA EL MAPA
void GameMap::loadFromFile(ifstream& file)
{
	//Las asignamos
	file >> rows >> cols;
	try {
		if (rows <= 0 || cols <= 0)
			throw FileFormatError("Tamaño de mapa incorrecto");

		//E iniciamos el mapa
		if (mapCell == nullptr)
			initMap();
	
		//Y EMPEZAMOS LA LECTURA
		int num;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				//MAPA: 0 -> VACIO || 1 -> MURO || 2 -> COMIDA || 3 -> VITAMINA || 5,6,7,8 -> FANTASMAS || 9 -> PACMAN
					file >> num;
					if (num > 9)
						throw FileFormatError("Valor de celda incorrecto: " + num);

					//CARGA MAPA
					if (num < 4) {
						setCell(i, j, (MapCell)num);
						if (num == 2)
							playState->addFood();
					}
			}
		}
	}
	catch (exception &e)
	{
		cerr << "Caught: " << e.what() << endl;
		cerr << "Type: " << typeid(e).name() << endl;
		this->game->endGame();
	};
}

//GUARDA EN FICHERO LOS ARCHIVOS DEL MAPA NECESARIOS
void GameMap::saveToFile(ofstream& file)
{	
	int data;
	file << rows << " " << cols << endl;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (getCell(i, j) == Wall)
				data = 1;
			else if (getCell(i, j) == Empty)
				data = 0;
			else if (getCell(i, j) == Food)
				data = 2;
			else if (getCell(i, j) == PowerUp)
				data = 3;
			file << data;
			if (j < cols - 1)
				file << " ";
		}
		file << endl;
	}
}

bool GameMap::handleEvent(SDL_Event & e)
{
	return false;
}

//PINTA EL MAPA EN PANTALLA
void GameMap:: render(){
	SDL_Rect destRect;
	for(int i=0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			destRect.h = playState->getTileHeight();
			destRect.w = playState->getTileWidth();
			destRect.x = j*playState->getTileWidth();
			destRect.y = i*playState->getTileHeight();
			//Y RENDERIZAMOS
			if (mapCell[i][j] == Wall)
				CellTex->render(destRect);
			else if (mapCell[i][j] == Food)
				FoodTex->render(destRect);
			else if (mapCell[i][j] == PowerUp)
				PowerUpTex->render(destRect);
		}
}