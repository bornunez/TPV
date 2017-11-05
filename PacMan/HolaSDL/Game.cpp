#include "Game.h"



Game::Game()
{
	init();

	loadTextures();
	//Hacer que los textures del gameMap apunten a las de Game, esto deberia hacerse mediante una funcion para que no sean publicas
	map = new GameMap(textures[0], textures[2], textures[3]); //Pasarle por el constructor las texturas de juego ??
	loadMap();

	

	// Se crean los GO
	/*
	pacMan = new PacMan(textures[1], this, 0, 0, cellWitdth, cellHeight, 0, 10);		//PACMAN
	ghosts[0] = new Ghost(textures[1], this, 0, 0, cellWitdth, cellHeight, 0, 0);		//ROJO
	ghosts[1] = new Ghost(textures[1], this, 0, 0, cellWitdth, cellHeight, 0, 2);		//NARANJA
	ghosts[2] = new Ghost(textures[1], this, 0, 0, cellWitdth, cellHeight, 0, 4);		//ROSA
	ghosts[3] = new Ghost(textures[1], this, 0, 0, cellWitdth, cellHeight, 0, 6);		//AZUL
	*/
}


Game::~Game()
{
}

void Game::run() {
	
	SDL_Event	event;

	while (!exit) {
		//Linea de tratar tiempo
		handleEvents();
		update();
		render();
	}
}

void Game::render() {
	SDL_RenderClear(renderer);		//	Borra	la	pantalla

	map->render(cellWitdth, cellHeight);
	//Se pinta PacMan
	pacMan->render();

	//Se pintan los fantasmas
	for (uint i = 0; i < ghosts.size(); i++) {
		ghosts[i]->render();
	}

	SDL_RenderPresent(renderer);	//	Muestra	la	escena
}

void Game::update() {
	//Se actualiza PacMan
	pacMan->update();
	//Se actualiza el movimiento de los fantasmas
	for (uint i = 0; i <  ghosts.size(); i++) {
		ghosts[i]->update();
	}
}
void Game::handleEvents() {
	
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		//If user closes the window
		if (event.type == SDL_QUIT) {
			exit = true;
		}
		//If user presses any key
		if (event.type == SDL_KEYDOWN) {
			exit = true;
		}
		//If user clicks the mouse
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			exit = true;
		}
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_DOWN) {
			}
			else if (event.key.keysym.sym == SDLK_UP) {
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
			}
		}
	}
}

bool Game::nextCell() {
	return true;
}


void Game::init() {
	int winX, winY;	//	Posición	de	la	ventana
	winX = winY = SDL_WINDOWPOS_CENTERED;

	//	Inicialización	del	sistema,	ventana	y	renderer
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("First	test	with	SDL", winX, winY,
		WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (window == nullptr || renderer == nullptr)
		cout << "Error	initializing	SDL\n";
}

//Carga el mapa
void Game::loadMap() {

	ifstream file;
	file.open("MAP.txt");
	if (file.is_open()) {
		uint auxInt;
		string aux;

		//Leer num filas y columnas y guardarlas en map
		file >> auxInt;
		map->setNumRows(auxInt);
		file >> auxInt;
		map->setNumCols(auxInt);

		// Inicializa la matriz del mapa
		map->iniMapCell();

		getline(file, aux);

		// Asigna el tamaño de cada celda del juego
		cellWitdth = WIN_WIDTH / map->getNumCols();
		cellHeight = WIN_HEIGHT / map->getNumRows();

		// Asigna a cada elemento de la matriz su numero que corresponde con una textura del GameMap
		while (!file.fail()) {
			for (int i = 0; i < map->getNumRows(); i++) {
				for (int j = 0; j < map->getNumCols(); j++) {
					int aux;
					//Se lee en un entero auxiliar
					file >> aux;

					// Se asigna la posicion inicial de pac man y los fantasmas
					if (aux > 4) {
						iniPositions(aux, j, i);
						aux = 0;
					}
					//Se convierte el entero a la enum MapCell y se inserta en la matriz
					map->setCell(i, j, (MapCell)aux);
				}
				getline(file, aux);
			}
		}
	}
	
}

void Game::loadTextures() {
	//meter en .h una estructura con los atributos para cargar en bucle. array de estructuras rellenado en el .h??
	//Inicializar el array en un bucle	//Se crean las texturas 
	for (uint i = 0; i < textures.size(); i++) {
		textures[i] = new Texture(renderer);
	}

	textures[0]->load("wall2.png", 1, 1);		 //Bloque
	textures[1]->load("characters1.png", 4, 14); //PacMan y fantasmas
	textures[2]->load("food2.png", 1, 1);		 //Cocos
	textures[3]->load("burguer1.png", 1, 1);	 //Vitamina
}

//Esto es por si los fantasmas no se comportan todos igual, si no hay que tener controlado que fantasma
//pertenece a cada casilla se puede quitar esta funcion y guardar el fantasma en la parte del array que le toque random
//Cambiar el nombre por iniGO y que el constructor reciba inix e iniy?
void Game::iniPositions(uint goNumber, uint iniX, uint iniY) {
	if (goNumber == 5) {
		ghosts[0] = new Ghost(textures[1], this, iniX* cellWitdth, iniY * cellHeight, cellWitdth, cellHeight, 0, 0);
		ghosts[0]->setIniPosition(iniX, iniY);
	} 
	else if (goNumber == 6) {
		ghosts[1] = new Ghost(textures[1], this, iniX* cellWitdth, iniY * cellHeight, cellWitdth, cellHeight, 0, 2);
		ghosts[1]->setIniPosition(iniX, iniY);
	}
	else if (goNumber == 7) {
		ghosts[2] = new Ghost(textures[1], this, iniX* cellWitdth, iniY * cellHeight, cellWitdth, cellHeight, 0, 4);
		ghosts[2]->setIniPosition(iniX, iniY);
	}
	else if (goNumber == 8) {
		ghosts[3] = new Ghost(textures[1], this, iniX * cellWitdth, iniY * cellHeight, cellWitdth, cellHeight, 0, 6);
		ghosts[3]->setIniPosition(iniX, iniY);
	}
	else if (goNumber == 9) {
		pacMan = new PacMan(textures[1], this, iniX * cellWitdth, iniY * cellHeight, cellWitdth, cellHeight, 0, 10);
		pacMan->setIniPosition(iniX, iniY);
	}
}