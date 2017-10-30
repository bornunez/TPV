#include "Game.h"



Game::Game()
{
	// Inicialización del sistema, ventana y renderer
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Pac-Man", winX, winY, winWidth, winHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	winX = winY = SDL_WINDOWPOS_CENTERED;
	cout << "Window created"<<endl;
}


Game::~Game()
{
}
void Game::run() {
	if (window == nullptr || renderer == nullptr)
		cout << "Error initializing SDL\n";
	else { // Programa que usa SDL
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // RGB y alpha
		SDL_RenderClear(renderer); // Borra la pantalla
		SDL_RenderPresent(renderer); // Muestra la escena
		cout << "SDL Sucesfully initializated, running game..."<<endl;
		while (!exit) {
			update();
			render();
			handleEvents();
			SDL_Delay(50);
		}
	}
	//SDL_Delay(WaitSecs); //Esperamos 5 segundos antes de salir

	// Finalización
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void Game::render() {
	SDL_RenderPresent(renderer);
}
void Game::update() {

}
void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			exit = true;
	}
}
