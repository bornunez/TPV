#pragma once
#include "PlayState.h"   
#include "PacMan.h"        
#include "SmartGhost.h" 
#include "GameMap.h"
#include "EndState.h"
#include "PauseState.h"
#include "GameStateMachine.h"
#include "GameObject.h"


PlayState::PlayState(Game* game, string userCode) : GameState(game)
{
	this->userCode = userCode;
	//Y cargamos texturas y creamos los objetos y personajes
	gameMap = new GameMap(this, this->game, this->game->getTexture(Background), this->game->getTexture(FoodTexture), this->game->getTexture(PowerUpTexture));  //a lo mejor sobra pasarle gam

	pacMan = new PacMan(this->game->getTexture(Characters), this, this->game, 10, 0, 0, 0);
	stage.push_back(pacMan);
	stage.push_back(gameMap);

	if (userCode != " ") {
		ifstream infile(LEVEL_PATH + userCode + ".pac");
		hasSaveFile = infile.good();
	}
	checkLevel();
}

PlayState::~PlayState() {
	delete gameFont;
}

//LLAMA AL RENDER DE CADA ENTIDAD
void PlayState::render() {
	GameState::render();
	// Pinta los textos
	for (int i = 0; i < NUM_TEXTS - NUM_MENUTEXTS; i++)
		game->getText(i)->render({ canvas.w, i * 110, GUI.w, 100 });
}

//LLAMA AL UPDATE DE CADA ENTIDAD
void PlayState::update() {

	if (level <= MAX_LEVEL) {
		if (foodCount <= 0) {
			destroyGhosts();
			level++;
			checkLevel();
		}

		powered = pacMan->hasEnergy();

		pacMan->update();
		updateGhosts();

		adjustTexts();
	}

	
	if(level > MAX_LEVEL || gameOver)
		game->getStateMachine()->changeState(new EndState(this->game, gameOver));
}

void PlayState::updateGhosts() {
	list<GameObject*>::iterator it = stage.begin();
	//Hacemos esto en este orden, porque hay que ver las colisiones antes y despues de que se muevan, para evitar que se cruzen con pacman sin matarlo
	//Primero movemos al pacman y vemos las colisiones
	pacCollision();
	it++;
	//Y ahora movemos a los fantasmas, mirando a ver si se cruzan
	for (it++; it != stage.end(); it++) {
		(*it)->update();
		fantCollision();
	}
	//Y finalmente vemos si ha comido a pacman
	pacCollision();
}

//CONTROL DE EVENTOS / ACTIVAMOS FLAGS
void PlayState::handleEvent(SDL_Event& event) {

	//Aqui vamos a ver el input de el juego. 
		if (event.type == SDL_KEYDOWN) {
			//Si se pulsa la tecla esc, salimos de la partida (Pero no del juego)
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				game->getStateMachine()->pushState(new PauseState(game));	
			}
			//Si no, le mandamos el input a pacman
			else {
				pacMan->handleEvent(event);
			}
	}
}
void PlayState::powerUp() {
	score += 10;
	powered = true;
}

//CARGA EL NIVEL Y ACTUALIZA LAS DIMENSIONES
void PlayState::checkLevel() {
	string levelStr;

	//Si no hay un nivel guardado, cargamos el del nivel actual
	if (!hasSaveFile)
		levelStr = LEVEL_PATH + "level0" + Utilities::intToStr(level) + ".pac";
	else  // Si lo hay, lo cargaremos
		levelStr = LEVEL_PATH + userCode + ".pac";
	if (loadLevel(levelStr, hasSaveFile)) {
		//Si hubiera cargado el mapa, hay que indicar que no vamos a volver a cargar el mismo
		hasSaveFile = false;
		pacMan->init(TILE_W, TILE_H);
	}
}

//ACTUALIZA LOS TEXTOS CON SU CONTENIDO ACTUAL
void PlayState::adjustTexts() {

	game->getText(ScoreText)->loadFromText("Score " + Utilities::intToStr(score), *gameFont, { 255,255,255 });
	game->getText(LevelText)->loadFromText("Level " + Utilities::intToStr(level), *gameFont, { 255,255,255 });
	game->getText(LifeText)->loadFromText("Lives " + Utilities::intToStr(pacMan->lifes()), *gameFont, { 255,255,255 });
}

void PlayState::collision() {
	pacCollision();
	fantCollision();
}



// DESTRUYE LOS FANTASMAS Y REINICIA LA ENERGIA, SE UTILIZA CADA VEZ QUE SE SUPERA UN NIVEL.
void PlayState::destroyGhosts() {
	list<GameObject*>::iterator it = stage.begin();
	it++;
	it++;
	while (it != stage.end()) {
		delete (*it);
		it = stage.erase(it);
	}

	powered = false;
}

//CARGA EL NIVEL DESDE FICHERO, DEVUELVE FALSE SI HAY FALLO AL CARGARLO Y TERMINA EL JUEGO
bool PlayState::loadLevel(string filename, bool saved) {
	uint typeGhost;
	Ghost* ghost;

	ifstream file(LEVEL_PATH + filename);

	try {
		if (!file.is_open())
			throw FileNotFoundError("Fallo cargando fichero: ", filename);

		//Si existe partida guardada la carga
		if (saved) {
			file >> level >> score;
		}

		//forma de capturarlo?
		gameMap->loadFromFile(file);

		screenRatioConfig();
		TILE_H = canvas.h / gameMap->getRows(); TILE_W = canvas.w / gameMap->getCols();  //VAMOS A DIBUJAR EL MAPA EN FUNCION DEL CANVAS

		file >> numGhost;

		for (uint i = 0; i < numGhost; i++) {
			file >> typeGhost;
			if (typeGhost != 0 && typeGhost != 1)
				throw FileFormatError("Tipo de fantasma no valido: ");

			if (typeGhost == 0)
				ghost = new Ghost(this->game->getTexture(Characters), this, this->game, i * 2, 0, TILE_W, TILE_H);
			else
				ghost = new SmartGhost(this->game->getTexture(Characters), this, this->game, SMARTGHOST_SPRITE, 0, TILE_W, TILE_H);

			ghost->loadFromFile(file);
			stage.push_back(ghost);
		}
		pacMan->loadFromFile(file);

		return true;
	}
	catch (exception &e)
	{
		cerr << "Caught: " << e.what() << endl;
		cerr << "Type: " << typeid(e).name() << endl;
		if (saved)			//Si cargar partida guardada falla se vuelve al menu principal.
			game->popGameState();
		else                //Si falla la carga de un nivel basico se sale del juego
			this->game->endGame();

		return false;
	};
}

//Dados dos fantasmas, devolvera true si hay espacio libre alrededor de los fantasmas
bool PlayState::freeSpace(int x, int y, int& nx, int& ny) 
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

//Comprueba si hay un fantasma en la casilla de coordenadas x, y
bool PlayState::isGhost(int x, int y)
{
	list<GameObject*>::iterator it = stage.begin();
	it++; //Nos saltamos a pacman
	it++; //Saltamos GameMap
	while ((it != stage.end()) && (x != static_cast<GameCharacter*>((*it))->getX() || y != static_cast<GameCharacter*>((*it))->getY()))
		it++;

	return it != stage.end();
}

//
void PlayState::pacCollision()
{
	list<GameObject*>::iterator it = stage.begin();
	bool destroy = false;
	list<GameObject*>::iterator smartIt; //It auxiliar para saber donde esta el smartghost
	it++;
	it++;
	while(!gameOver && it != stage.end()) {
		//Flag de que han colisionado
		bool col = pacMan->getX() == static_cast<GameCharacter*>((*it))->getX() && pacMan->getY() == static_cast<GameCharacter*>((*it))->getY();

		SmartGhost* smart = dynamic_cast<SmartGhost*>(*it); //Vemos el tipo de fant que es

		if (col && (powered || (smart != nullptr && smart->isOld()))) {
			if (smart == nullptr) // si es fant normal
				static_cast<GameCharacter*>((*it))->die();
			else {
				smartIt = it;
				destroy = true;
			}
			score += 20;
		}
		else if (col && !gameOver) {
			// Se llama a die de pacman y de fantasmas
			pacMan->die();
			list<GameObject*>::iterator it = stage.begin(); 
			it++;
			for (it++; it != stage.end(); it++) {
				static_cast<GameCharacter*>(*it)->die();			
			}						
			if (pacMan->isDead()) {
				gameOver = true; 
			}
		}
			it++;
	}
	if (destroy) {
		delete(*smartIt);
		stage.erase(smartIt);
		numGhost--;
	}
}

void PlayState::fantCollision()
{
	list<GameObject*>::iterator me = stage.begin(); //Me es el fantasma que evaluamos a - b  a - c  a - d // b - c b - d  // c - d 
	me++;
	for (me++; me != stage.end(); me++) {

		SmartGhost* smartMe = dynamic_cast<SmartGhost*>(*me); //Soy inteligente?
															  //Si soy inteligente, adulto y puedo tener hijos me tengo que preocupar de colisionar
		if (smartMe != nullptr && smartMe->isAdult() && !smartMe->hasChild()) {
			list<GameObject*>::iterator other = me; //Iterador de los fant a colisionar

			for (other++; other != stage.end(); other++) {

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
							SmartGhost* ghost = new SmartGhost(this->game->getTexture(Characters), this, this->game, 4 * 2, 0, TILE_W, TILE_H);
							ghost->init(nx, ny, None);
							stage.push_back(ghost);
							numGhost++;
						}
					}
				}
			}
		}
	}
}

//Configura el canvas para que siempre se mantenga la relacion
void PlayState::screenRatioConfig() {

	GUI.w = windowReg.w * ((float)GUI_Ratio / (float)100);

	canvas.w = windowReg.w - GUI.w;
	canvas.h = windowReg.h;
	//AHORA VAMOS A ADAPTAR EL CANVAS PARA QUE MANTENGA LA RELACION DEL MAPA
	float mapRatio = (float)gameMap->getRows() / (float)gameMap->getCols(); //Cuantas veces es mayor el alto que el ancho

	if (((float)canvas.h / mapRatio) <= canvas.w)
		canvas.w = (float)canvas.h / mapRatio;
	else
		canvas.h = (float)canvas.w* mapRatio;
	GUI.h = canvas.h;
}

//GUARDAMOS EL ESTADO ACTUAL EN UN FICHERO CON EL NOMBRE DEL JUGADOR
void PlayState::saveToFile(string userCode)
{
	ofstream file(LEVEL_PATH + userCode + ".pac");

	if (!file.is_open())
		throw new FileNotFoundError("No se ha podido crear el archivo: ", userCode);
	//INFO DEL MAPA
	file << level << " " << score << endl;;
	//GUARDAMOS EL MAPA
	gameMap->saveToFile(file);

	file << numGhost << endl;;

	//Se guardan los fantasmas y PacMan
	list<GameObject*>::iterator it = stage.begin();
	it++;
	for (it++; it != stage.end(); it++) {
		static_cast<GameCharacter*> (*it)->saveToFile(file);
		file << endl;
	}
	pacMan->saveToFile(file);
	file.close();
}

int PlayState::getRows() { return gameMap->getRows(); }

int PlayState::getCols() { return gameMap->getCols(); }

int PlayState::getPacPosX() { return pacMan->getX(); }

int PlayState::getPacPosY() { return pacMan->getY(); }

MapCell PlayState::getCell(int row, int col) { return gameMap->getCell(row, col); }

void PlayState::setCell(int row, int col, MapCell type) { gameMap->setCell(row, col, type); }
