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
	//Creamos las Texturas	
	Texture* Text1 = new Texture(renderer,TEXT_PATH);
	Texture* Text2 = new Texture(renderer,TEXT_PATH);

	//Cargamos las texturas
	Text1->load("name1", 1, 6);
	Text2->load("name2", 1, 1);
	//Guardamos las texturas
	Textures[0] = Text1;
	Textures[1] = Text2;
}

void Game::loadGO() {
	//Creacion de GO
	GameObject* GO_0 = new GameObject(Textures[0], 0, 0, 100, 100, 0, 0); //Modelo para personajes
	GameObject* GO_1 = new GameObject(Textures[1], 0, 0, winWidth, winHeight); //Modelo para fondos
	//Y guardamos los GO
	gameObjects[0] = GO_0;
	gameObjects[1] = GO_1;
}


void Game::render() {
	//AQUI LLAMAMOS AL RENDER DE CADA ENTIDAD
	cout << "Render" << endl;
	for (int i = 0; i < NUM_GO; i++)
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
