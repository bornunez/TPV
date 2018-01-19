#pragma once
#include "Game.h"
#include "GameStateMachine.h"

Game::Game()
{
	// Inicializacion del sistema, ventana y renderer
	SDL_Init(SDL_INIT_EVERYTHING);
	winX = winY = SDL_WINDOWPOS_CENTERED;
	window = SDL_CreateWindow("Pac-Man", winX, winY, windowReg.w, windowReg.h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	try {
		if (window == nullptr) 
			throw SDLError("Error initializing window: " + (string)SDL_GetError());
		if (renderer == nullptr) 
			throw SDLError("Error initializing renderer: " + (string)IMG_GetError());
		if (TTF_Init() < 0) 
			throw SDLError("Error initializing TTF: " + (string)TTF_GetError());

		//Notificamos que todo ha ido bien
		cout << "Window created" << endl;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // RGB y alpha
		SDL_RenderClear(renderer); // Borra la pantalla
		SDL_RenderPresent(renderer); // Muestra la escena
		cout << "SDL Sucesfully initializated, running game..." << endl;

		//Cargamos texturas
		loadTextures();

		stateMachine = new GameStateMachine();
		stateMachine->pushState(new MainMenuState(this));
	}
	catch (exception &e)
	{
		cerr << "Caught: " << e.what() << endl;
		cerr << "Type: " << typeid(e).name() << endl;
		exit = true;
	};
}
	
	//Hacer un try que al detectar un fallo pare todo lo demas?
//INICIALIZAMOS LAS TEXTURAS
void Game::loadTextures() {
	for (uint i = 0; i < NUM_TEXTURES; i++) {
		textures[i] = new Texture(renderer, TEXT_PATH);
		const TextureAtributes atributes = TEXTURE_ATRIBUTES[i];
		textures[i]->load(atributes.filename, atributes.numRows, atributes.numCols);
	}

	for (uint i = 0; i < NUM_TEXTS; i++)
		texts[i] = new Texture(renderer, "");
}

//BUCLE PRINCIPAL DEL JUEGO
void Game::run() {


	uint32_t startTime, frameTime;
	//BUCLE PRINCIPAL DEL JUEGO

		//BUCLE PRINCIPAL DEL NIVEL
		while (!stateMachine->empty() && !exit)
		{
			// Gestion del tiempo
			startTime = SDL_GetTicks();

			update();
			render();
			handleEvents();

			frameTime = SDL_GetTicks() - startTime;
			if (frameTime < FRAME_RATE)
				SDL_Delay(FRAME_RATE - frameTime);
		}
}




//LLAMA AL RENDER DE CADA ENTIDAD
void Game::render() {
	SDL_RenderClear(renderer);

	stateMachine->currentState()->render();

	SDL_RenderPresent(renderer);
}

//LLAMA AL UPDATE DE CADA ENTIDAD
void Game::update() {
	stateMachine->currentState()->update();
}

//CAPTURA LA PULSACION DE TECLAS NUMERICAS PARA GUARDAR UN CODIGO HASTA QUE EL USUARIO PULSA ENTER
uint Game::writeCode()
{
	SDL_Event event;
	uint code = 0;
	bool endWrite = false;

	while (!endWrite) {
		while (SDL_PollEvent(&event)) {
			if (event.key.keysym.sym == SDLK_RETURN)
				endWrite = true;
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) {
					code = 10 * code + (event.key.keysym.sym - SDLK_0);
				}
			}
		}
	}
	return code;
}

//CONTROL DE EVENTOS / ACTIVAMOS FLAGS
void Game::handleEvents() {
	SDL_Event event;

	while (!exit && SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			exit = true;
		else if(!stateMachine->empty())
				stateMachine->currentState()->handleEvent(event);
	}
}


Game::~Game()
{ 
	for (Texture* t : texts)
		delete t;

	for (Texture* t : textures)
		delete t;
	delete stateMachine;
}

void Game::pushGameState(GameState* gameState) { stateMachine->pushState(gameState); }
void Game::popGameState() { stateMachine->popState(); }
