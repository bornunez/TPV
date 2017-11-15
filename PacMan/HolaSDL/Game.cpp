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
	gameMap = new GameMap(textures[Background], textures[FoodText], textures[PowerUpText]);
	if(!error)
		if(loadMap("..\\levels\\level01.dat"))error=true;


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

		uint32_t startTime, frameTime;
		//BUCLE PRINCIPAL DEL JUEGO
		while (!exit && !gameOver && foodCount > 0) {
			// Gestion del tiempo
			startTime = SDL_GetTicks();

			handleEvents();
			update();
			render();
			
			frameTime = SDL_GetTicks() - startTime;
			if (frameTime < FRAME_RATE)
				SDL_Delay(FRAME_RATE - frameTime);
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

	for (uint i = 0; i < NUM_TXTS; i++) {
		textures[i] = new Texture(renderer, TEXT_PATH);
		const TextureAtributes atributes = TEXTURE_ATRIBUTES[i];
		textures[i]->load(atributes.filename, atributes.numRows, atributes.numCols);
	}

	/*
	//Creamos las Texturas	
	for(int i=0;i<NUM_TXTS;i++)
		textures[i] = new Texture(renderer, TEXT_PATH);

	//Cargamos las texturas
	if(textures[0]->load("characters1.png", 4, 14)) error = true;
	else if(textures[1]->load("wall2.png")) error = true;
	else if(textures[2]->load("food2.png")) error = true;
	else if(textures[3]->load("cereza.png"))error = true;
	*/
}
void Game::loadCharacters() {
	for (int i = 0; i < NUM_GHOST; i++) {
		ghosts[i] = new Ghost(textures[Characters], this,i * 2, 0);
	}
	pacMan = new Pac_Man(textures[Characters], this, 10, 0); //Cargamos a PACMAN
}

void Game::render() {
	//AQUI LLAMAMOS AL RENDER DE CADA ENTIDAD
	SDL_RenderClear(renderer);

	gameMap->render(TILE_W, TILE_H);

	cout << "Render" << endl;
	for (int i = 0; i < NUM_GHOST; i++)
	{
		ghosts[i]->render();
	}
	pacMan->render();
	SDL_RenderPresent(renderer);
}
void Game::update() {
	//AQUI SE LLAMA AL UPDATE DE CADA ENTIDAD
	int tickTime = SDL_GetTicks();
	if(powered)
		powered = (auxTime + POWERTIME) > tickTime;
	for (int i = 0; i < NUM_GHOST; i++) {
		ghosts[i]->update();
	}
	collision();
	pacMan->update();
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
			if (event.key.keysym.sym == SDLK_LEFT)
				pacMan->setDir(Left);
			else if (event.key.keysym.sym == SDLK_RIGHT)
				pacMan->setDir(Right);
			else if (event.key.keysym.sym == SDLK_UP)
				pacMan->setDir(Up);
			else if (event.key.keysym.sym == SDLK_DOWN)
				pacMan->setDir(Down);
			

				/*
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
			
			pacMan->setDir(dir);
			*/
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

	// >>>>>>>>> AQUI AJUSTAMOS LA PANTALLA EN FUNCION DEL MAPA
	screenRatioConfig();

	TILE_H = canvasHeight / MAP_ROWS; TILE_W = canvasWidth / MAP_COLS;  //VAMOS A DIBUJAR EL MAPA EN FUNCION DEL CANVAS
	//Las asignamos
	gameMap->setCols(MAP_COLS);
	gameMap->setRows(MAP_ROWS);
	//E iniciamos el mapa
	gameMap->initMap();
	//Y EMPEZAMOS LA LECTURA
	int num;
	foodCount = 0;

	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLS; j++) {
			//MAPA: 0 -> VACIO || 1 -> MURO || 2 -> COMIDA || 3 -> VITAMINA || 5,6,7,8 -> FANTASMAS || 9 -> PACMAN
			file >> num;

			//CARGA MAPA
			if (num < 4) {
				gameMap->setCell(i, j, (MapCell)num);
				if (num == 2)
					foodCount+=1;
			}

			//CARGA FANTASMAS
			else if (num>4 && num<9) {
				gameMap->setCell(i, j, Empty);
				ghosts[num - 5]->init(j, i, TILE_W, TILE_H); //Ponemos las posiciones iniciales del Fant
			}
			//PACMAN
			else if (num == 9) {
				gameMap->setCell(i, j, Empty);
				pacMan ->init(j, i, TILE_W, TILE_H); //Cargamos a PACMAN
			}
		}
	}
	file.close();
	return false;
}

void Game::powerUp() {
	powered = true;
	auxTime = SDL_GetTicks();
}

void Game::collision() {
	for (int i = 0; i < NUM_GHOST; i++) {
		bool collision = pacMan->getX() == ghosts[i]->getX() && pacMan->getY() == ghosts[i]->getY();
		if (collision && powered)
			ghosts[i]->Die();
		else if (collision && !gameOver) {
			if (pacMan->die())
				gameOver = true;
		}
	}
}

void Game::screenRatioConfig() {
	//VAMOS A CONFIGURAR EL TAMAÑO DEL CANVAS EN FUNCION DEL GUI
	//DE TAL MODO QUE LOS TILES DEL MAPA SIEMPRE SEAN CUADRADOS Y OCUPEN LA MAXIMA PANTALLA POSIBLE
	
	GUIWidth = winWidth * ((float) GUI_Ratio /(float) 100);

	canvasWidth = winWidth - GUIWidth;
	canvasHeight = winHeight;
	//AHORA VAMOS A ADAPTAR EL CANVAS PARA QUE MANTENGA LA RELACION DEL MAPA
	float mapRatio = (float)MAP_ROWS / (float)MAP_COLS; //Cuantas veces es mayor el alto que el ancho

	if (((float)canvasHeight / mapRatio) <= canvasWidth)
		canvasWidth = (float)canvasHeight / mapRatio;
	else 
		canvasHeight = (float)canvasWidth * mapRatio;
	GUIHeight = canvasHeight;

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
