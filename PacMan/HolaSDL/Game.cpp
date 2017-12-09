#include "Game.h"         
#include "PacMan.h"        
#include "Ghost.h" 
#include "GameMap.h"

Game::Game()
{
	//Se pide usuario para el juego antes de crearlo
	login();

	// Inicialización del sistema, ventana y renderer
	SDL_Init(SDL_INIT_EVERYTHING);
	winX = winY = SDL_WINDOWPOS_CENTERED;
	window = SDL_CreateWindow("Pac-Man", winX, winY, windowReg.w, windowReg.h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (window == nullptr || renderer == nullptr) {
		cout << "Error initializing SDL\n";
		error = true;
	}
	else {
		//Notificamos que todo ha ido bien
		cout << "Window created" << endl;

		//Y cargamos texturas y creamos los objetos y personajes
		loadTextures();
		loadCharacters();
		gameMap = new GameMap(this, textures[Background], textures[FoodTexture], textures[PowerUpTexture]);
		loadText();
	}
}


void Game::run() {

	//AQUI SUCEDE EL BUCLE PRINCIPAL DEL JUEGO. 
	if (!error){
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // RGB y alpha
		SDL_RenderClear(renderer); // Borra la pantalla
		SDL_RenderPresent(renderer); // Muestra la escena
		cout << "SDL Sucesfully initializated, running game..."<<endl;

		uint32_t startTime, frameTime;
		//BUCLE PRINCIPAL DEL JUEGO
		while (!exit && !gameOver && level <= MAX_LEVEL) {

			//CARGAMOS EL NIVEL Y ACTUALIZAMOS LAS DIMENSIONES
			string levelStr;
			//Si no hay un nivel guardado, cargamos el del nivel actual
			if (!hasSaveFile) 
				levelStr = LEVEL_PATH + "level0" + Utilities::intToStr(level) + ".dat";
			else  // Si lo hay, lo cargaremos
				levelStr = LEVEL_PATH + userName + ".dat";
			if (loadMap(levelStr,hasSaveFile)) error = true;
			//Actualizamos los textos al nuevo canvas
			adjustTexts();

			//Si hubiera cargado el mapa, hay que indicar que no vamos a volver a cargar el mismo
			hasSaveFile = false;

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
	if (!gameOver && !error) save(userName);
	
	// Finalización
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	//HAYAS PERDIDO O HAYAS GANADO
	if (!exit && !error) {

		system("cls");
		if (!gameOver)
			cout << "YOU WIN, " + userName + " !!!!";
		else
			cout << "YOU DIED :( ";
		cin.get();

		if (remove((LEVEL_PATH + userName + ".dat").c_str()) != 0)
			cout << "Error deleting file";
	}
	manageScoreTable();
}

//INICIALIZAMOS LAS TEXTURAS
void Game::loadTextures() { 
	for (uint i = 0; i < NUM_TEXTURES; i++) {
		textures[i] = new Texture(renderer, TEXT_PATH);
		const TextureAtributes atributes = TEXTURE_ATRIBUTES[i];
		textures[i]->load(atributes.filename, atributes.numRows, atributes.numCols);
	}
}

//INICIALIZAMOS LOS PERSONAJES, SE LES COLOCARA EN LA LECTURA DEL MAPA
void Game::loadCharacters() {
	for (int i = 0; i < NUM_GHOST; i++) {
		ghosts[i] = new Ghost(textures[Characters], this,i * 2, 0);
	}
	pacMan = new PacMan(textures[Characters], this, 10, 0); //Cargamos a PACMAN
}

//INICIALIZAMOS TODOS LOS TEXTOS
void Game::loadText() {
	for(int i=0;i< NUM_TEXTS; i++)
		texts[i] = new Text(textures[SpriteFont], "SampleText", 0, 0, 0, 0);
}
void Game::render() {
	//AQUI LLAMAMOS AL RENDER DE CADA ENTIDAD
	SDL_RenderClear(renderer);

	gameMap->render();

	for (int i = 0; i < NUM_GHOST; i++)
	{
		ghosts[i]->render();
	}
	for(int i=0; i< NUM_TEXTS; i++)
		texts[i]->render();
	pacMan->GameCharacter::render();
	SDL_RenderPresent(renderer);

	//Decorativo: Tiempo de pacman muerto
	pacMan->isDead();

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
	//SI LEEMOS DE ARCHIVO HAY INFORMACION ADICIONAL
	if (saved) {
		//Info mapa
		file >> level;
		file >> score;
		//Info fantasmas
		for (int i = 0; i < NUM_GHOST; i++) {
			file >> iniGhost[i].x;
			file >> iniGhost[i].y;
		}
		//InfoPacman
		file >> iniPacman.x;
		file >> iniPacman.y;
		int lifes;
		file >> lifes;
		pacMan->setLifes(lifes);
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

//GUARDAMOS EL ESTADO ACTUAL EN UN FICHERO CON EL NOMBRE DEL JUGADOR
bool Game::save(string filename)
{
	ofstream file(LEVEL_PATH + filename+".dat");
	 
	if (!file.is_open())
		return true;
	//INFO DEL MAPA
	file << MAP_ROWS << " " << MAP_COLS << endl << level << endl << score << endl;
	//INFO DE LOS FANTASMAS (Su pos inicial)
	for (int i = 0; i < NUM_GHOST; i++) {
		file << ghosts[i]->getIniX() << " ";
		file << ghosts[i]->getiniY() << endl;
	}
	//INFO PACMAN (Pos inicial)
	file << pacMan->getIniX() << " " << pacMan->getiniY() << endl;
	file << pacMan->lifes() << endl;
	//GUARDAMOS EL MAPA
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

MapCell Game::getCell(int row, int col)
{
	return gameMap->getCell(row, col);
}

void Game::setCell(int row, int col, MapCell type)
{
	gameMap->setCell(row, col, type);
}


//HAY FANTASMA EN LA POSICION? SI LO HAY, DEVOLVEMOS CUAL
bool Game:: is_Ghost(int& rows, int& cols, int& ghost_num) {
	bool exit = false;
	ghost_num = 0;
	while (ghost_num < NUM_GHOST && !exit)
	{
		exit = (ghosts[ghost_num]->getX() == cols) && (ghosts[ghost_num]->getY() == rows);
		if(!exit) ghost_num++;
	}
	return ghost_num < NUM_GHOST;
}

//AJUSTAMOS LAS DIMENSIONES DE LOS TEXTOS AL GUI NUEVO
void Game::adjustTexts()
{
	int charSize = 20;    //TEXTO      X      Y     W             H
	texts[ScoreText]->set("Score", canvas.w, 50, charSize * 10, charSize);
	texts[UserText]->set(userName, canvas.w, 100, charSize * 10, charSize);
	texts[LevelText]->set("Level - " + Utilities::intToStr(level) , canvas.w, 150, charSize * 9, charSize);
	texts[LifeText]->set("Lives - " + Utilities::intToStr(pacMan->lifes()), canvas.w, 200 , charSize * 9, charSize);
}

//"MOTOR" DE COLISIONES >>>> ACTIVA FLAGS
void Game::collision() {
	for (int i = 0; i < NUM_GHOST; i++) {
		bool collision = pacMan->getX() == ghosts[i]->getX() && pacMan->getY() == ghosts[i]->getY();
		if (collision && powered) {
			ghosts[i]->die();
			score += 20;
		}
		else if (collision && !gameOver) {
			if (pacMan->die())
				gameOver = true;
			for (int i = 0; i < NUM_GHOST; i++)
				ghosts[i]->die();
			texts[LifeText]->setText("Lives - " + Utilities::intToStr(pacMan->lifes()));
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

	cout << "Introduce tu nombre: ";
	getline(cin, userName);
	
	//Se carga la tabla
	scoreTable.load(SCORETABLE_PATH);

	// Si el nombre de usuario ya existe se guarda en actualUserReg el registro que le corresponde segun la DB
	for (int i = 0; (i < scoreTable.getScoreReg().size()) && !validateUser; i++) {
		if (scoreTable.getScoreReg()[i].nameReg == userName) {
			actualUserReg = scoreTable.getScoreReg()[i];
			validateUser = true;
		}
	}
	if (!validateUser) 
		cout << "El usuario no existe, se creara un nuevo registro con tu nombre de usuario";
	if (validateUser) {
		ifstream infile(LEVEL_PATH + userName + ".dat");
		hasSaveFile = infile.good();
	}
}

//Administra la tabla de puntaciones, pidiendo al jugador su nombre para incluirlo en ella y mostrando el top 10.
void Game::manageScoreTable() {

	system("cls");

	//Si el usuario existe se edita el score de su antiguo registro
	if (validateUser) {
		if (actualUserReg.score < score) {
			scoreTable.changeScoreReg(actualUserReg, score);
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
