#include "Game.h"



Game::Game()
{
	init();
	loadMap();
	//meter en .h una estructura con los atributos para cargar en bucle. array de estructuras rellenado en el .h??

	//Se crean las texturas 
	Texture *text1 = new Texture(renderer);
	Texture *text2 = new Texture(renderer);
	Texture *text3 = new Texture(renderer);
	
	text1->load("wall2.png", 1, 1);		  //Bloque
	text2->load("characters1.png", 4, 14); //PacMan y fantasmas
	text3->load("food2.png", 1, 1);		  //Cocos

	//Se insertan las texturas
	textures[0] = text1;
	textures[1] = text2;
	textures[2] = text3;

	// Se crean los GO
	pacMan = new PacMan(text2, this, 0, 0, 32, 32, 0, 10);			//PACMAN
	Ghost *red = new Ghost(text2, this, 0, 0, 32, 32, 0, 0);		//ROJO
	Ghost *orange = new Ghost(text2, this, 0, 0, 32, 32, 0, 2);		//NARANJA
	Ghost *pink = new Ghost(text2, this, 0, 0, 32, 32, 0, 4);		//ROSA
	Ghost *blue = new Ghost(text2, this, 0, 0, 32, 32, 0, 6);		//AZUL

	//Se insertan los fantasmas
	ghosts[0] = red;
	ghosts[1] = orange;
	ghosts[2] = pink;
	ghosts[3] = blue;
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
	map->render(cellWitdth, cellHeight);
	//Se actualiza PacMan
	pacMan->update();
	//Se actualiza el movimiento de los fantasmas
	for (uint i = 0; i <  ghosts.size(); i++) {
		ghosts[i]->update();
	}
}
void Game::handleEvents() {
	
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		//If user closes the window
		if (e.type == SDL_QUIT) {
			exit = true;
		}
		//If user presses any key
		if (e.type == SDL_KEYDOWN) {
			exit = true;
		}
		//If user clicks the mouse
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			exit = true;
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

void Game::loadMap() {
	//Aqui va la carga del MAPA
	ifstream file;
	file.open("MAP.txt");
	if (file.is_open()) {
		int auxInt;
		string aux;
		file >> auxInt;
		map->setNumRows(auxInt);
		file >> auxInt;
		map->setNumCols(auxInt);

		map->iniMapCell();

		getline(file, aux);


		cellWitdth = WIN_WIDTH / map->getNumCols();
		cellHeight = WIN_HEIGHT / map->getNumRows();

		while (!file.fail()) {
			for (int i = 0; i < map->getNumRows(); i++) {
				for (int j = 0; j < map->getNumCols(); j++) {
					int aux;
					//Se lee en un entero auxiliar
					file >> aux;
					//Se convierte el entero a la enum MapCell
					map->setCell(i, j, (MapCell)aux);
				}
				getline(file, aux);
			}
		}
	}
	
}