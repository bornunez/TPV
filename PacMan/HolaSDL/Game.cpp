#include "Game.h"         
#include "PacMan.h"        
#include "SmartGhost.h" 
#include "GameMap.h"

Game::Game()
{
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
		login();
	}
}

// CREA LA PANTALLA EN LA QUE SE ELIGE SI EMPEZAR UNA NUEVA PARTIDA O CARGAR UNA MEDIANTE CODIGO 
void Game::login() {
	SDL_Event event;
	// Muestra los textos de NEW y LOAD
	SDL_RenderClear(renderer);
	texts[StartText]->render({ windowReg.w / 2 - 100,  windowReg.h / 2 - 200, 200, 200 });
	texts[LoadText]->render({ windowReg.w / 2 - 100, windowReg.h / 2 + 100, 200, 200 });
	SDL_RenderPresent(renderer);

	while (menu) {
		SDL_WaitEvent(&event);

		if (event.type == SDL_MOUSEBUTTONDOWN) {
			int x, y;
			SDL_GetMouseState(&x, &y);

			if ((x > windowReg.w / 2 - 100 && x < windowReg.w / 2 + 100) && (y > windowReg.h / 2 - 200 && y < windowReg.h)) {		//new
				hasSaveFile = false;
				menu = false;
			}
			if ((x > windowReg.w / 2 - 100 && x < windowReg.w / 2 + 100) && (y > windowReg.h / 2 + 100 && y < windowReg.h + 300)) {		//load

				SDL_RenderClear(renderer);
				texts[StartText]->loadFromText("Escribe el codigo", *gameFont, { 255,255,255 });
				texts[StartText]->render({ windowReg.w/2-200,  windowReg.h / 2 - 200, 400, 200 });
				SDL_RenderPresent(renderer);

				userName = Utilities::intToStr(writeCode());
				ifstream infile(LEVEL_PATH + userName + ".pac");
				hasSaveFile = infile.good();

				menu = false;
			}
		}
	}
	//Se carga la tabla
	scoreTable.load(SCORETABLE_PATH);
}

//BUCLE PRINCIPAL DEL JUEGO
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

	for (uint i = 0; i < NUM_TEXTS; i++) 
		texts[i] = new Texture(renderer, "");
	
	//Se cargan aqui porque no cambian con cada ciclo, asi no consume recursos en el update
	texts[StartText]->loadFromText("Start", *gameFont, { 255,255,255 }); 
	texts[LoadText]->loadFromText("Load", *gameFont, { 255,255,255 }); 

}

//LLAMA AL RENDER DE CADA ENTIDAD
void Game::render() {

	SDL_RenderClear(renderer);

	gameMap->render();

	for (GameCharacter* c : characters) {
		c->render();
	}
	// Pinta todos los textos menus los del menu inicial
	for(int i = 0; i < NUM_TEXTS - NUM_MENUTEXTS; i++)
		texts[i]->render({ canvas.w, i*110, GUI.w, 100 });


	SDL_RenderPresent(renderer);

	//Decorativo: Tiempo de pacman muerto
	//pacMan->waitDeathTime();
}

//LLAMA AL UPDATE DE CADA ENTIDAD
void Game::update() {

	powered = pacMan->hasEnergy();

	for (GameCharacter* c : characters) {
		c->update();
		collision();
	}

	adjustTexts();
}

//CONTROL DE EVENTOS / ACTIVAMOS FLAGS
void Game::handleEvents() {

	SDL_Event event;
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
}

//CARGA EL NIVEL Y ACTUALIZA LAS DIMENSIONES
void Game::checkLevel() {

	string levelStr;
	//Si no hay un nivel guardado, cargamos el del nivel actual
	if (!hasSaveFile)
		levelStr = LEVEL_PATH + "level0" + Utilities::intToStr(level) + ".pac";
	else  // Si lo hay, lo cargaremos
		levelStr = LEVEL_PATH + userName + ".pac";
	if (!loadLevel(levelStr, hasSaveFile)) error = true;
	//Actualizamos los textos al nuevo canvas
		adjustTexts();
	//Si hubiera cargado el mapa, hay que indicar que no vamos a volver a cargar el mismo
	hasSaveFile = false;
	pacMan->init(TILE_W, TILE_H);
}

void Game::endGame() {
	// Finalización
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	//HAYAS PERDIDO O HAYAS GANADO
	if (!exit && !error) {

		if (remove((LEVEL_PATH + userName + ".pac").c_str()) != 0)
			cout << "Error deleting file";

		system("cls");
		if (!gameOver) {
			cout << "YOU WIN! ENTER YOUR USERNAME !!!!" << endl;
			
		}
		else
			cout << "YOU DIED :(  ENTER YOUR USERNAME" << endl;

		cin >> userName;
		cin.get();
		manageScoreTable();
	}
}

//GUARDAMOS EL ESTADO ACTUAL EN UN FICHERO CON EL NOMBRE DEL JUGADOR
bool Game::save(string filename)
{
	ofstream file(LEVEL_PATH + filename + ".pac");
	 
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

//ACTUALIZA LOS TEXTOS CON SU CONTENIDO ACTUAL
void Game::adjustTexts() {

	texts[ScoreText]->loadFromText("Score " + Utilities::intToStr(score), *gameFont, { 255,255,255 });
	texts[LevelText]->loadFromText("Level " + Utilities::intToStr(level), *gameFont, { 255,255,255 });
	texts[LifeText]->loadFromText("Lives " + Utilities::intToStr(pacMan->lifes()), *gameFont, { 255,255,255 });
}

void Game::collision() {
	pacCollision();
	fantCollision();
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

//Administra la tabla de puntaciones, pidiendo al jugador su nombre para incluirlo en ella y mostrando el top 10.
void Game::manageScoreTable()
{
	bool validateUser = false;
	system("cls");

	for (int i = 0; (i < scoreTable.getScoreReg().size()) && !validateUser; i++) {
		if (scoreTable.getScoreReg()[i].nameReg == userName) {
			actualUserReg = scoreTable.getScoreReg()[i];
			validateUser = true;
		}
	}
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

	//Borramos Mapa
	delete gameMap;

	for (int i = 0; i < NUM_TEXTS; i++)
		delete(texts[i]);

	//Borramos texturas
	for (int i = 0; i < NUM_TEXTURES; i++)
		delete(textures[i]);

	delete(gameFont);
}

// DESTRUYE LOS FANTASMAS Y REINICIA LA ENERGIA, SE UTILIZA CADA VEZ QUE SE SUPERA UN NIVEL.
void Game::destroyGhosts() {
	list<GameCharacter*>::iterator it = characters.begin();
	it++;
	while (it != characters.end()) {
		delete (*it);
		it = characters.erase(it);
	}

	powered = false;
}

//CARGA EL NIVEL DESDE FICHERO
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
			ghost = new SmartGhost(textures[Characters], this, SMARTGHOST_SPRITE, 0, TILE_W, TILE_H);

		ghost->loadFromFile(file);
		characters.push_back(ghost);
	}
	pacMan->loadFromFile(file);
	if ((*characters.begin()) != pacMan)
		characters.insert(characters.begin(), pacMan);			//Se lee el ultimo pero se inserta el primero para evitar el iterador inverso

	return true;
}

//GUARDA EN UN FICHERO LA PARTIDA, PIDIENDO UN CODIGO DE ACCESO
void Game::saveState() {
	SDL_Event event;
	uint code = 0;
	save(Utilities::intToStr(writeCode()));
	saveSt = false;
}


//PASARLE LA TEXTURA CON EL TEXTO QUE VA A ESCRIBIR EN PANTALLA?
uint Game::writeCode() {
	SDL_Event event;
	uint code = 0;
	bool endWrite = false;

	while (!endWrite) {
		SDL_WaitEvent(&event);
		if (event.key.keysym.sym == SDLK_RETURN)
			endWrite = true;
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) {
				code = 10 * code + (event.key.keysym.sym - SDLK_0);
				//textos[3]->loadFromText(Utilities::intToStr(code), *gameFont, { 255,255,255 });
			}
		}
	}
	return code;
}

bool Game::freeSpace(int x, int y, int& nx, int& ny) //Dados dos fantasmas, devolvera true si hay espacio libre alrededor de los fantasmas
{
	vector<pair<int, int>> cardinals; //Las posiciones adyacentes a los fantasmas
	Utilities::getCardinals(x, y, cardinals);
	//Ya tenemos listo nuestro vector de las casillas adyacentes, ahora vamos a cribarlo: Si hay muro o fantasma -> Lo eliminamos
	vector<pair<int, int>>::iterator it;
	for (it = cardinals.begin(); it != cardinals.end();) {
		int actX = (*it).first;
		int actY = (*it).second;
		//Si esta ocupada, la quitamos
		if (getCell(actX, actY) == Wall || isGhost(actX, actY))
			it = cardinals.erase(it);
		else
			it++;
	}
	//Ahora acabamos escogiendo uno aleatorio, en caso de que haya
	if (cardinals.size() > 0) {
		int rdn = rand() % cardinals.size();
		nx = cardinals[rdn].first;
		ny = cardinals[rdn].second;
		return true;
	}
	else
		return false;
}

bool Game::isGhost(int x, int y)
{
	list<GameCharacter*>::iterator it = characters.begin();
	it++; //Nos saltamos a pacman
	while ((it != characters.end()) && ((x != (*it)->getX()) || (y != (*it)->getY())))
		it++;

	return it != characters.end();
}

void Game::pacCollision()
{
	list<GameCharacter*>::iterator it = characters.begin();
	bool destroy = false;
	list<GameCharacter*>::iterator smartIt; //It auxiliar para saber donde esta el smartghost

	for (it++; it != characters.end(); it++) {
		//Flag de que han colisionado
		bool col = pacMan->getX() == (*it)->getX() && pacMan->getY() == (*it)->getY();

		SmartGhost* smart = dynamic_cast<SmartGhost*>(*it); //Vemos el tipo de fant que es

		if (col && (powered || (smart != nullptr && smart->isOld()))) {
			if (smart == nullptr) // si es fant normal
				(*it)->die();
			else {
				smartIt = it;
				destroy = true;
			}
			score += 20;
		}
		else if (col && !gameOver) {
			for (GameCharacter* c : characters)
				c->die();

			if (pacMan->isDead())
				gameOver = true;
		}
	}
	if (destroy) {
		GameCharacter* aux = *smartIt;
		characters.erase(smartIt);
		delete(aux);
		numGhost--;
		SDL_Delay(100);
	}
}


void Game::fantCollision()
{
	list<GameCharacter*>::iterator me = characters.begin(); //Me es el fantasma que evaluamos a - b  a - c  a - d // b - c b - d  // c - d 
	for (me++; me != characters.end(); me++) {

		SmartGhost* smartMe = dynamic_cast<SmartGhost*>(*me); //Soy inteligente?
															  //Si soy inteligente, adulto y puedo tener hijos me tengo que preocupar de colisionar
		if (smartMe != nullptr && smartMe->isAdult() && !smartMe->hasChild()) {
			list<GameCharacter*>::iterator other = me; //Iterador de los fant a colisionar
			for (other++; other != characters.end(); other++) {

				SmartGhost* smartOther = dynamic_cast<SmartGhost*>(*other); //Es el otro fantasma inteligente?+
																			//Si el otro es inteligente, adulto y puede tener hijos, paso a ver si colisionamos
				if (smartOther != nullptr && smartOther->isAdult() && !smartOther->hasChild()) {
					bool coll = (smartMe->getX() == smartOther->getX()) && (smartMe->getY() == smartOther->getY());
					if (coll) {
						//Si ha llegado hasta aqui es porque pueden tener hijos. Falta ver si hay espacio
						int nx, ny;
						if (freeSpace(smartMe->getX(), smartMe->getY(), nx, ny)) {
							cout << "HIJO" << endl;
							smartMe->isFather(); smartOther->isFather();
							SmartGhost* ghost = new SmartGhost(textures[Characters], this, 4 * 2, 0, TILE_W, TILE_H);
							ghost->init(nx, ny, None);
							characters.push_back(ghost);
						}
					}
				}
			}
		}
	}
}

int Game::getPacPosX() { return pacMan->getX(); }

int Game::getPacPosY() { return pacMan->getY(); }

MapCell Game::getCell(int row, int col) { return gameMap->getCell(row, col); }

void Game::setCell(int row, int col, MapCell type) { gameMap->setCell(row, col, type); }

int Game::getRows() { return gameMap->getRows(); }

int Game::getCols() { return gameMap->getCols(); }
