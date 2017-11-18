#include "Game.h"         


Game::Game()
{
	//Se pide usuario para el juego antes de crearlo
	login();

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
	//scoreTable.load(SCORETABLE_PATH);
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
			if (loadMap(levelStr)) error = true;
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
			level++;
		}

		//Finalización
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		manageScoreTable();
	}
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
}
void Game::handleEvents() {

	SDL_Event event;
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
bool Game::loadMap(string filename){
	ifstream file(filename);
	if (!file.is_open())
		return true;
	//LAS DOS PRIMERAS FILAS SON EL NUMERO DE FILAS Y COLUMNAS
	file >> MAP_ROWS;
	file >> MAP_COLS;

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
				ghosts[num - 5]->init(j, i, TILE_W, TILE_H); //Ponemos las posiciones iniciales del Fantasma
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
	score += 10;
	powered = true;
	auxTime = SDL_GetTicks();
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

void Game::login() {

	cout << "Introduce tu nombre";
	getline(cin, userName);
	
	//Se carga la tabla
	scoreTable.load(SCORETABLE_PATH);

	for (int i = 0; (i < scoreTable.getScoreReg().size()) && !validateUser; i++) {
		if (scoreTable.getScoreReg()[i].nameReg == userName) {
			validateUser = true;
		}
	}

	if (!validateUser) 
		cout << "El usuario no existe, se creara un nuevo registro con tu nombre de usuario";
}

//Administra la tabla de puntaciones, pidiendo al jugador su nombre para incluirlo en ella y mostrando el top 10.
void Game::manageScoreTable() {

	system("cls");

	//Si el usuario existe se edita el score de su antiguo registro
	if (validateUser) {
		for (int i = 0; i < scoreTable.getScoreReg().size() ; i++) {
			if (scoreTable.getScoreReg()[i].nameReg == userName && scoreTable.getScoreReg()[i].score < score)
				scoreTable.changeScoreReg(i, scoreTable.getScoreReg()[i], score);		
		}
	}
	else {
		// Si el usuario no existe se crea un nuevo registro para el
		scoreTable.addScore(userName, score);
	}

	scoreTable.save(SCORETABLE_PATH);
	
	//Pinta la tabla de puntuaciones
	system("cls");
	scoreTable.printTopNScores(NUM_SCORES_TOP);

	cout << "Pulsa Enter para salir";
	cin.get();
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
