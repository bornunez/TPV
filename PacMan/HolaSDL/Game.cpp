#include "Game.h"         


Game::Game()
{
	// Inicialización del sistema, ventana y renderer
	SDL_Init(SDL_INIT_EVERYTHING);
	winX = winY = SDL_WINDOWPOS_CENTERED;
	window = SDL_CreateWindow("Pac-Man", winX, winY, windowReg.w, windowReg.h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//Notificamos que todo ha ido bien
	cout << "Window created"<<endl;

	//Y cargamos texturas y creamos los objetos y personajes
	loadTextures();
	loadCharacters();
	gameMap = new GameMap(textures[Background], textures[FoodTexture], textures[PowerUpTexture]);
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
		while (!exit && !gameOver) {

			//CARGAMOS EL NIVEL Y ACTUALIZAMOS LAS DIMENSIONES
			string levelStr = LEVEL_PATH + "level0" + Utilities::intToStr(level) + ".dat";
			loadMap("..\\levels\\miniman.dat",true);
			//if (loadMap(levelStr)) error = true;
			loadText();

			//BUCLE PRINCIPAL DEL NIVEL
			while (!exit && !gameOver && !error && foodCount > 0)
			{
				// Gestion del tiempo
				startTime = SDL_GetTicks();

				handleEvents();
				update();
				render();

				frameTime = SDL_GetTicks() - startTime;
				if (frameTime < FRAME_RATE)
					SDL_Delay(FRAME_RATE - frameTime);
			}
			if(!exit && !gameOver && !error) level++;
		}
	}
	if (!gameOver && !error) save("miniman");
	// Finalización
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::loadTextures() {

	for (uint i = 0; i < NUM_TEXTURES; i++) {
		textures[i] = new Texture(renderer, TEXT_PATH);
		const TextureAtributes atributes = TEXTURE_ATRIBUTES[i];
		textures[i]->load(atributes.filename, atributes.numRows, atributes.numCols);
	}
}
void Game::loadCharacters() {

	for (int i = 0; i < NUM_GHOST; i++) {
		ghosts[i] = new Ghost(textures[Characters], this,i * 2, 0);
	}
	pacMan = new Pac_Man(textures[Characters], this, 10, 0); //Cargamos a PACMAN
}
void Game::loadText() {
	texts[0] = new Text(textures[SpriteFont], "score 000", canvas.w, 50, 20 * 10, 20);
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
	texts[0]->render();
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

	texts[0]->setText("Score " + Utilities::intToStr(score));

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
			else if (event.key.keysym.sym == SDLK_ESCAPE)
				exit = true;
		}
	}

}
bool Game::loadMap(string filename, bool saved){
	ifstream file(filename);
	if (!file.is_open())
		return true;
	//LAS DOS PRIMERAS FILAS SON EL NUMERO DE FILAS Y COLUMNAS
	file >> MAP_ROWS;
	file >> MAP_COLS;
	//SI ES UN MAPA CARGADO AQUI SE CONTIENE LA INFORMACION DEL ESTADO
	struct IniPos { int x; int y; };
	//LAS POSICIONES INICIALES DEL MAPA ORIGINAL
	IniPos iniGhost[NUM_GHOST];
	IniPos iniPacman;
	if (saved) {
		file >> level;
		file >> score;
		for (int i = 0; i < NUM_GHOST; i++) {
			file >> iniGhost[i].x;
			file >> iniGhost[i].y;
		}
		file >> iniPacman.x;
		file >> iniPacman.y;
	}
	

	// >>>>>>>>> AQUI AJUSTAMOS LA PANTALLA EN FUNCION DEL MAPA
	screenRatioConfig();

	TILE_H = canvas.h / MAP_ROWS; TILE_W = canvas.w / MAP_COLS;  //VAMOS A DIBUJAR EL MAPA EN FUNCION DEL CANVAS
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
				if(!saved)
					ghosts[num - 5]->init(j, i, TILE_W, TILE_H); //Ponemos las posiciones iniciales del Fantasma
				else {
					ghosts[num - 5]->init(iniGhost[num - 5].x, iniGhost[num - 5].y, TILE_W, TILE_H); //Ponemos las posiciones iniciales del Fantasma
					ghosts[num - 5]->set(j, i);
				}
			}
			//PACMAN
			else if (num == 9) {
				gameMap->setCell(i, j, Empty);
				if(!saved)
					pacMan ->init(j, i, TILE_W, TILE_H); //Cargamos a PACMAN
				else {
					pacMan->init(iniPacman.x, iniPacman.y, TILE_W, TILE_H); //Cargamos a PACMAN
					pacMan->set(j, i);
				}
			}
		}
	}
	file.close();
	return false;
}

void Game::powerUp() {
	score += 10;
	powered = true;
	auxTime = SDL_GetTicks();
}

     
bool Game::save(string filename)
{
	ofstream file(LEVEL_PATH + filename+".dat");
	 
	if (!file.is_open())
		return true;
	file << MAP_ROWS << " " << MAP_COLS << endl << level << endl << score << endl;
	for (int i = 0; i < NUM_GHOST; i++) {
		file << ghosts[i]->getIniX() << " ";
		file << ghosts[i]->getiniY() << endl;
	}
	file << pacMan->getIniX() << " " << pacMan->getiniY() << endl;
	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLS; j++)
		{
			int data = 0;
			if (gameMap->getCell(i, j) == Wall)
				data = 1;
			else if (is_Ghost(i, j, data))
				data = 5 + data;
			else if (pacMan->getX() == j && pacMan->getY() == i)
				data = 9;
			else if (gameMap->getCell(i, j) == Empty)
				data = 0;
			else if (gameMap->getCell(i, j) == Food)
				data = 2;
			else if (gameMap->getCell(i, j) == PowerUp)
				data = 3;
			file << data;
			if(j<MAP_COLS-1)
				file << " ";
		}
		file << endl;
	}
	file.close();

	return false;
}

bool Game:: is_Ghost(int& rows, int& cols, int& ghost_num) {
	bool exit = false;
	ghost_num = 0;
	while (ghost_num < NUM_GHOST && !exit)
	{
		exit = (ghosts[ghost_num]->getX() == cols) && (ghosts[ghost_num]->getY() == rows);
		if(!exit) ghost_num++;
	}
	if (exit)
		cout << "hola";
	return ghost_num < NUM_GHOST;
}

void Game::collision() {
	for (int i = 0; i < NUM_GHOST; i++) {
		bool collision = pacMan->getX() == ghosts[i]->getX() && pacMan->getY() == ghosts[i]->getY();
		if (collision && powered) {
			ghosts[i]->Die();
			score += 20;
		}
		else if (collision && !gameOver) {
			if (pacMan->die())
				gameOver = true;
		}
	}
}

void Game::screenRatioConfig() {
	//VAMOS A CONFIGURAR EL TAMAÑO DEL CANVAS EN FUNCION DEL GUI
	//DE TAL MODO QUE LOS TILES DEL MAPA SIEMPRE SEAN CUADRADOS Y OCUPEN LA MAXIMA PANTALLA POSIBLE
	
	GUI.w = windowReg.w * ((float) GUI_Ratio /(float) 100);

	canvas.w = windowReg.w - GUI.w;
	canvas.h = windowReg.h;
	//AHORA VAMOS A ADAPTAR EL CANVAS PARA QUE MANTENGA LA RELACION DEL MAPA
	float mapRatio = (float)MAP_ROWS / (float)MAP_COLS; //Cuantas veces es mayor el alto que el ancho

	if (((float)canvas.h / mapRatio) <= canvas.w)
		canvas.w= (float)canvas.h/ mapRatio;
	else 
		canvas.h= (float)canvas.w* mapRatio;
	GUI.h = canvas.h;

}

Game::~Game()
{ 
	//Borramos textos
	for (int i = 0; i < NUM_TEXTS; i++)
		delete(texts[i]);

	//Borramos Mapa
	delete gameMap;
	//Borramos Personajes
	for (int i = 0; i < NUM_GHOST; i++)
		delete(ghosts[i]);
	delete(pacMan);

	//Borramos texturas
	for (int i = 0; i < NUM_TEXTURES; i++)
		delete(textures[i]);
}
