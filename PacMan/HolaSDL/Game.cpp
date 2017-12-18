#include "Game.h"         
#include "PacMan.h"        
#include "SmartGhost.h" 
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
	if (TTF_Init() < 0) {
		cout << "Error initializing TTF\n";
		error = true;
	}

	else {
		//Notificamos que todo ha ido bien
		cout << "Window created" << endl;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // RGB y alpha
		SDL_RenderClear(renderer); // Borra la pantalla
		SDL_RenderPresent(renderer); // Muestra la escena
		cout << "SDL Sucesfully initializated, running game..." << endl;

		//Y cargamos texturas y creamos los objetos y personajes
		gameFont = new Font(TEXT_PATH + "ARCADECLASSIC.TTF", 20);
		loadTextures();
		gameMap = new GameMap(this, textures[Background], textures[FoodTexture], textures[PowerUpTexture]);
		pacMan = new PacMan(textures[Characters], this, 10, 0, 0,0);		//NEW PACMAN DE PRUEBA
		loadText();
	}
}

//AQUI SUCEDE EL BUCLE PRINCIPAL DEL JUEGO. 
void Game::run() {

	uint32_t startTime, frameTime;
	//BUCLE PRINCIPAL DEL JUEGO
	while (!exit && !gameOver && level <= MAX_LEVEL) {
		checkLevel();

		//BUCLE PRINCIPAL DEL NIVEL
		while (!exit && !gameOver && !error && foodCount > 0)
		{
			// Gestion del tiempo
			startTime = SDL_GetTicks();

			handleEvents();
			update();
			render();

			if (saveSt)
				saveState();

			frameTime = SDL_GetTicks() - startTime;
			if (frameTime < FRAME_RATE)
				SDL_Delay(FRAME_RATE - frameTime);
		}
		if (!exit && !gameOver && !error) {
			destroyGhosts();
			level++;
		}
	}
	endGame();
}

//INICIALIZAMOS LAS TEXTURAS
void Game::loadTextures() { 
	for (uint i = 0; i < NUM_TEXTURES; i++) {
		textures[i] = new Texture(renderer, TEXT_PATH);
		const TextureAtributes atributes = TEXTURE_ATRIBUTES[i];
		textures[i]->load(atributes.filename, atributes.numRows, atributes.numCols);
	}

	for (uint i = 0; i < NUM_TEXTOS; i++) 
		textos[i] = new Texture(renderer, "");
	

	textos[0]->loadFromText(userName, *gameFont, { 255,255,255 }); //Se carga aqui porque no cambia, asi no consume recursos en el update


}

//INICIALIZAMOS TODOS LOS TEXTOS
void Game::loadText() {
	for(int i = 0; i < NUM_TEXTS; i++)
		texts[i] = new Text(textures[SpriteFont], "SampleText", 0, 0, 0, 0);
}

//LLAMA AL RENDER DE CADA ENTIDAD
void Game::render() {

	SDL_RenderClear(renderer);

	gameMap->render();

	for (GameCharacter* c : characters) {
		c->render();
	}

	for(int i=0; i< NUM_TEXTS; i++)
		texts[i]->render();

	for(int i = 0; i< NUM_TEXTOS; i++)
		textos[i]->render({ canvas.w, i*110, GUI.w, 100 });

	//textos[0]->render({ canvas.w, 10, GUI.w, 100 });		//ver como poner el size de altura y ancho
	//textos[1]->render({ canvas.w, 110, GUI.w, 100 });

	SDL_RenderPresent(renderer);

	//Decorativo: Tiempo de pacman muerto
	pacMan->isDead();
}

void Game::update() {
	//AQUI SE LLAMA AL UPDATE DE CADA ENTIDAD
	int tickTime = SDL_GetTicks();
	if(powered)
		powered = (auxTime + POWERTIME) > tickTime;

	for (GameCharacter* c : characters) {
		c->update();
		collision();
	}


	textos[1]->loadFromText(Utilities::intToStr(score), *gameFont, { 255,255,255 });
	textos[2]->loadFromText(Utilities::intToStr(level), *gameFont, { 255,255,255 });



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
			else if (event.key.keysym.sym == SDLK_s) {
				saveSt = true;
			}
		}
	}
}


void Game::powerUp() {
	score += 10;
	powered = true;
	auxTime = SDL_GetTicks();
}

//CARGA EL NIVEL Y ACTUALIZA LAS DIMENSIONES
void Game::checkLevel() {

	string levelStr;
	//Si no hay un nivel guardado, cargamos el del nivel actual
	if (!hasSaveFile)
		levelStr = LEVEL_PATH + "level0" + Utilities::intToStr(level) + ".dat";
	else  // Si lo hay, lo cargaremos
		levelStr = LEVEL_PATH + userName + ".dat";
	if (!loadLevel(levelStr, hasSaveFile)) error = true;
	//Actualizamos los textos al nuevo canvas
		adjustTexts();
	//Si hubiera cargado el mapa, hay que indicar que no vamos a volver a cargar el mismo
	hasSaveFile = false;
	pacMan->init(TILE_W, TILE_H);
}

void Game::endGame() {
	//if (!gameOver && !error)
	//	save(userName);

	// Finalización
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	//if (saveSt)				//Si se pulsa S sera true y se hara el saveState
		//saveState();

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

//GUARDAMOS EL ESTADO ACTUAL EN UN FICHERO CON EL NOMBRE DEL JUGADOR
bool Game::save(string filename)
{
	ofstream file(LEVEL_PATH + filename + ".dat");
	 
	if (!file.is_open())
		return true;
	//INFO DEL MAPA
	file << level << " " << score << endl;;
	//GUARDAMOS EL MAPA
	gameMap->saveToFile(file);

	file << numGhost << endl;;

	//Se guardan los fantasmas y PacMan
	list<GameCharacter*>::reverse_iterator it;
	for (it = characters.rbegin(); it != characters.rend(); ++it) {
		(*it)->saveToFile(file);
		file << endl;
	}
	file.close();

	return false;
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

void Game::collision() {
	list<GameCharacter*>::iterator it = characters.begin();
	for (it++; it != characters.end(); ++it) {
		bool collision = pacMan->getX() == (*it)->getX() && pacMan->getY() == (*it)->getY();
		if (collision && powered) {
			(*it)->die();
			score += 20;
		}
		else if (collision && !gameOver) {
			for (GameCharacter* c : characters) 
				c->die();
			
			if (pacMan->isDead())
				gameOver = true;

			texts[LifeText]->setText("Lives - " + Utilities::intToStr(pacMan->lifes()));
		}
	}
}

//CONFIGURA EL TAMAÑO DEL CANVAS EN FUNCION DEL GUI
//DE MODO QUE LOS TILES DEL MAPA SIEMPRE SEAN CUADRADOS Y OCUPEN LA MAXIMA PANTALLA POSIBLE
void Game::screenRatioConfig() {

	GUI.w = windowReg.w * ((float) GUI_Ratio /(float) 100);

	canvas.w = windowReg.w - GUI.w;
	canvas.h = windowReg.h;
	//AHORA VAMOS A ADAPTAR EL CANVAS PARA QUE MANTENGA LA RELACION DEL MAPA
	float mapRatio = (float)gameMap->getRows() / (float)gameMap->getCols(); //Cuantas veces es mayor el alto que el ancho

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
	//Borramos Personajes
	for (GameCharacter* c : characters) 
		delete(c);

	//Borramos textos
	for (int i = 0; i < NUM_TEXTS; i++)
		delete(texts[i]);

	//Borramos Mapa
	delete gameMap;

	for (int i = 0; i < NUM_TEXTOS; i++)
		delete(textos[i]);

	//Borramos texturas
	for (int i = 0; i < NUM_TEXTURES; i++)
		delete(textures[i]);

	delete(gameFont);

}

// Destruye los fantasmas, se utiliza cada vez que se supera un nivel.
void Game::destroyGhosts() {
	list<GameCharacter*>::iterator it = characters.begin();
	for (it++; it != characters.end(); ++it) 
		delete(*it);
}

bool Game::loadLevel(string filename, bool saved) {
	//Estas lo mismo van al .h
	uint typeGhost;
	Ghost* ghost;

	ifstream file(LEVEL_PATH + filename);
	if (!file.is_open())
		return false;

	//Si existe partida guardada la carga
	if (saved) {
		file >> level >> score;
	}

	gameMap->loadFromFile(file);

	screenRatioConfig();
	TILE_H = canvas.h / gameMap->getRows(); TILE_W = canvas.w / gameMap->getCols();  //VAMOS A DIBUJAR EL MAPA EN FUNCION DEL CANVAS

	file >> numGhost;

	for (uint i = 0; i < numGhost; i++) {
		file >> typeGhost;
		if (typeGhost == 0)
			ghost = new Ghost(textures[Characters], this, i * 2, 0, TILE_W, TILE_H);
		else
			ghost = new SmartGhost(textures[Characters], this, i * 2, 0, TILE_W, TILE_H);

		ghost->loadFromFile(file);
		characters.push_back(ghost);
	}
	pacMan->loadFromFile(file);
	characters.insert(characters.begin(), pacMan);			//Se lee el ultimo pero se inserta el primero para evitar el iterador inverso
	return true;
}

void Game::saveState() {
	SDL_Event event;
	uint code = 0;

	cout << "Introduce el codigo numerico: ";

	while (saveSt && !exit) {
		SDL_WaitEvent(&event);
		if (event.key.keysym.sym == SDLK_RETURN)
			saveSt = false;
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym  <= SDLK_9) {
				code = 10 * code + (event.key.keysym.sym  - SDLK_0);
				textos[3]->loadFromText(Utilities::intToStr(code), *gameFont, { 255,255,255 });
			}
		}
	}
	save(Utilities::intToStr(code));
}

MapCell Game::getCell(int row, int col) { return gameMap->getCell(row, col); }

int Game::getPacPosX()
{
	return pacMan->getX();
}

int Game::getPacPosY()
{
	return pacMan->getY();
}


void Game::setCell(int row, int col, MapCell type) { gameMap->setCell(row, col, type); }

int Game::getRows() { return gameMap->getRows(); }

int Game::getCols() { return gameMap->getCols(); }
