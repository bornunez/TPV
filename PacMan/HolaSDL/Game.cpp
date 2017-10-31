#include "Game.h"



Game::Game()
{
	// Inicialización del sistema, ventana y renderer
	SDL_Init(SDL_INIT_EVERYTHING);
	winX = winY = SDL_WINDOWPOS_CENTERED;
	window = SDL_CreateWindow("Pac-Man", winX, winY, winWidth, winHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	TILE_H = winHeight / MAP_ROWS;
	TILE_W = winWidth / MAP_COLS;
	//Notificamos que todo ha ido bien
	cout << "Window created"<<endl;
	//Y cargamos texturas y creamos los objetos y personajes
	loadTextures();
	loadGO();

}


Game::~Game()
{
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
	//SDL_Delay(WaitSecs); //Esperamos 5 segundos antes de salir

	// Finalización
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::loadTextures() {
	// | PERSONAJES | MURO | COMIDA | BONUS |

	//Creamos las Texturas	
	Texture* Chars_TX = new Texture(renderer,TEXT_PATH);
	Texture* Wall_TX = new Texture(renderer,TEXT_PATH);
	Texture* Food_TX = new Texture(renderer, TEXT_PATH);
	Texture* PowerUp_TX = new Texture(renderer, TEXT_PATH);
	//Cargamos las texturas
	Chars_TX->load("characters1.png", 4, 14);
	Wall_TX->load("wall2.png");
	Food_TX->load("food2.png");
	PowerUp_TX->load("burguer1.png");
	//Guardamos las texturas
	Textures[0] = Chars_TX;
	Textures[1] = Wall_TX;
	Textures[2] = Food_TX;
	Textures[3] = PowerUp_TX;
}

void Game::loadGO() {
	// | FANT 0 | FANT 1 | ... | PACMAN | MURO | COMIDA | VITAMINA |

	//Creacion de GO
	//GameObject* Fant_0 = new GameObject(Textures[0], 0, 0, 100, 100, 0, 0); //Modelo para personajes
	for (int i = 0; i < 4; i++) {
		//Cargamos los 4 fantasmas
		gameObjects[i] = new GameObject(Textures[0],i*TILE_W, 0,TILE_W ,TILE_H, i * 2, 0);
	}
	gameObjects[4] = new GameObject(Textures[0],TILE_W*4, 0, TILE_W, TILE_H,11,0); //Modelo para fondos
	//Y guardamos los GO
}


void Game::render() {
	//AQUI LLAMAMOS AL RENDER DE CADA ENTIDAD
	cout << "Render" << endl;
	for (int i = 0; i < 5; i++)
	{
		gameObjects[i]->render();
	}
	SDL_RenderPresent(renderer);
}
void Game::update() {
	//AQUI SE LLAMA AL UPDATE DE CADA ENTIDAD
	cout << "Update"<<endl;
}
void Game::handleEvents() {

	SDL_Event event;
	cout << "Events" << endl;
	//CONTROL DE EVENTOS / ACTIVAMOS FLAGS
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			exit = true;
	}
}
