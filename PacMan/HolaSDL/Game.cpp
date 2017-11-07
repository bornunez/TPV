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
	Position PacMan;
	Position Fant[4];
	loadTextures();
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
			update();
			render();
			handleEvents();
			SDL_Delay(TICK_SPEED);
		}
	}
	// Finalización
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::loadTextures() {
	// | PERSONAJES | MURO | COMIDA | BONUS |

	//Creamos las Texturas	
	for(int i=0;i<NUM_TXTS;i++)
		textures[i] = new Texture(renderer, TEXT_PATH);

	//Cargamos las texturas
	if(textures[0]->load("characters1.png", 4, 14)) error = true;
	else if(textures[1]->load("wall2.png")) error = true;
	else if(textures[2]->load("food2.png")) error = true;
	else if(textures[3]->load("cereza.png"))error = true;

	gameMap = new GameMap(textures[1], textures[2], textures[3]);
}

void Game::render() {
	//AQUI LLAMAMOS AL RENDER DE CADA ENTIDAD
	SDL_RenderClear(renderer);

	gameMap->render(TILE_W, TILE_H);

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
	PacMan->update();
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
			/*if (num == 0)
				gameMap->setCell(i, j, Empty);
			else if (num == 1 || num == 4)
				gameMap->setCell(i, j, Wall);
			else if (num == 2) {
				gameMap->setCell(i, j, Food);
				numComida++;
			}
			else if (num == 3)
				gameMap->setCell(i, j, PowerUp);*/
			if (num < 5)
				gameMap -> setCell(i, j, (MapCell)num);
			//CARGA FANTASMAS
			else if (num>4 && num<9) {
				gameMap->setCell(i, j, Empty);
				ghosts[num - 5] = new Ghost(textures[0], j, i, TILE_W, TILE_H, (num - 5) * 2, 0);
			}
			//PACMAN
			else if (num == 9) {
				gameMap->setCell(i, j, Empty);
				PacMan = new Pac_Man(textures[0],j, i, TILE_W, TILE_H,this, 11, 0); //Cargamos a PACMAN
			}
		}
	}
	file.close();
	return false;
}

Game::~Game()
{
	//Borramos MApa
	delete gameMap;
	//Borramos Personajes
	for (int i = 0; i < 4; i++)
		delete(ghosts[i]);
	delete(PacMan);
	//Borramos texturas
	for (int i = 0; i < NUM_TXTS; i++)
		delete(textures[i]);
}
