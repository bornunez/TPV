#include "Game.h"



Game::Game()
{
	// Inicialización del sistema, ventana y renderer
	SDL_Init(SDL_INIT_EVERYTHING);
	winX = winY = SDL_WINDOWPOS_CENTERED;
	window = SDL_CreateWindow("Pac-Man", winX, winY, winWidth, winHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//Notificamos que todo ha ido bien
	cout << "Window created"<<endl;
	//Y cargamos texturas y creamos los objetos y personajes
	loadTextures();
	loadCharacters();
	gameMap = new GameMap(textures[1], textures[2], textures[3]);
	if(!error)
		if(loadMap("..\\levels\\level00.dat"))error=true;
}


void Game::run() {
	//AQUI SUCEDE EL BUCLE PRINCIPAL DEL JUEGO. 
	if (window == nullptr || renderer == nullptr)
		cout << "Error initializing SDL\n";
	else { // Programa que usa SDL
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // RGB y alpha
		SDL_RenderClear(renderer); // Borra la pantalla
		SDL_RenderPresent(renderer); // Muestra la escena
		cout << "SDL Sucesfully initializated, running game..."<<endl;

		//BUCLE PRINCIPAL DEL JUEGO
		while (!exit) {
			handleEvents();
			update();
			render();
			SDL_Delay(TICK_SPEED);
		}
	}
	// Finalización
	//SDL_RenderClear(renderer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::loadTextures() {
	// | PERSONAJES | MURO | COMIDA | BONUS |i

	//Creamos las Texturas	
	for(int i=0;i<NUM_TXTS;i++)
		textures[i] = new Texture(renderer, TEXT_PATH);

	//Cargamos las texturas
	if(textures[0]->load("characters1.png", 4, 14)) error = true;
	else if(textures[1]->load("wall2.png")) error = true;
	else if(textures[2]->load("food2.png")) error = true;
	else if(textures[3]->load("cereza.png"))error = true;
}
void Game::loadCharacters() {
	for (int i = 0; i < NUM_GHOST; i++) {
		ghosts[i] = new Ghost(textures[0], this,i * 2, 0);
	}
	PacMan = new Pac_Man(textures[0], this, 11, 0); //Cargamos a PACMAN
}

void Game::render() {
	//AQUI LLAMAMOS AL RENDER DE CADA ENTIDAD
	SDL_RenderClear(renderer);

	gameMap->render(TILE_W, TILE_H);
	//renderMap();

	cout << "Render" << endl;
	for (int i = 0; i < 4; i++)
	{
		ghosts[i]->render();
	}
	PacMan->render();
	SDL_RenderPresent(renderer);
}
void Game::update() {
	//AQUI SE LLAMA AL UPDATE DE CADA ENTIDAD
	for (int i = 0; i < 4; i++) {
		ghosts[i]->update();
	}
	collision();
	PacMan->update();
	collision();
	cout << "Update"<<endl;
}
void Game::handleEvents() {

	SDL_Event event;
	cout << "Events" << endl;
	//CONTROL DE EVENTOS / ACTIVAMOS FLAGS
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			exit = true;
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				dir = Left;
				break;
			case SDLK_RIGHT:
				dir = Right;
				break;
			case SDLK_UP:
				dir = Up;
				break;
			case SDLK_DOWN:
				dir = Down;
				break;
			default:
				break;
			}
			PacMan->setDir(dir);
		}
	}

}
bool Game::loadMap(string filename){
	ifstream file(filename);
	if (!file.is_open())
		return true;
	//LAS DOS PRIMERAS FILAS SON EL NUMERO DE FILAS Y COLUMNAS
	file >> MAP_ROWS;
	file >> MAP_COLS;
	TILE_H = winHeight / MAP_ROWS; TILE_W = winWidth / MAP_COLS;
	//Las asignamos
	gameMap->setCols(MAP_COLS);
	gameMap->setRows(MAP_ROWS);
	//E iniciamos el mapa
	gameMap->initMap();
	//Y EMPEZAMOS LA LECTURA
	int num;
	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLS; j++) {
			//MAPA: 0 -> VACIO || 1 -> MURO || 2 -> COMIDA || 3 -> VITAMINA || 5,6,7,8 -> FANTASMAS || 9 -> PACMAN
			file >> num;

			//CARGA MAPA
			if (num < 4)
				gameMap -> setCell(i, j, (MapCell)num);
			//CARGA FANTASMAS
			else if (num>4 && num<9) {
				gameMap->setCell(i, j, Empty);
				ghosts[num - 5]->init(j, i, TILE_W, TILE_H); //Ponemos las posiciones iniciales del Fant
			}
			//PACMAN
			else if (num == 9) {
				gameMap->setCell(i, j, Empty);
				PacMan ->init(j, i, TILE_W, TILE_H); //Cargamos a PACMAN
			}
		}
	}
	file.close();
	return false;
}

void Game::powerUp() {
	for (int i = 0; i < NUM_GHOST; i++)
		ghosts[i]->Die();
}

void Game::collision() {
	for (int i = 0; i < NUM_GHOST; i++) {
		bool collision = PacMan->getX() == ghosts[i]->getX() && PacMan->getY() == ghosts[i]->getY();
		if (collision && PacMan->isPowered())
			ghosts[i]->Die();
		else if (collision && !gameOver) {
			if (PacMan->die())
				gameOver = true;
		}
	}
}

Game::~Game()
{ /*
	//Borramos Personajes
	for (int i = 0; i < 4; i++)
		delete(ghosts[i]);
	delete(PacMan);
	//Borramos MApa
	delete gameMap;
	//Borramos texturas
	for (int i = 0; i < NUM_TXTS; i++)
		delete(textures[i]);*/
}

/*void Game::renderMap() {
	SDL_Rect destRect;
	for (int i = 0; i<MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLS; j++) {
			destRect.h = TILE_H;
			destRect.w = TILE_W;
			destRect.x = j*TILE_W;
			destRect.y = i*TILE_H;
			//Y RENDERIZAMOS
			int celltype = (int)(gameMap->getCell(i, j));
			if (celltype > 0 )
				textures[celltype]->render(destRect);
		}
}*/
