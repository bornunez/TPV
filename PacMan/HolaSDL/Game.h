#pragma once
#include <sstream>
#include "Utilities.h"
#include "SDL_ttf.h"
#include "Texture.h"
#include "SDLError.h"

class GameStateMachine;
class GameState;

//CONSTANTES DE ARRAYS 
const int NUM_TEXTS = 5;
const int NUM_MENUTEXTS = 2;
const int NUM_TEXTURES = 10;

const int FRAME_RATE = 100;

//CONSTANTES
const string TEXT_PATH = "..\\images\\";
const string LEVEL_PATH = "..\\levels\\";
const string SCORETABLE_PATH = "..\\users\\scoretable.txt";
const Region windowReg{ 1000, 750 };

//Nombre de las texturas 
enum TextureName { Characters, Background, FoodTexture, PowerUpTexture, SpriteFont, PlayButton, ExitButton, LoadButton, ResumeButton,SaveButton};
enum TextName { ScoreText, LevelText, LifeText, StartText, LoadText };
enum MapCell { Empty, Wall, Food, PowerUp };

class Game {
private:
	const TextureAtributes TEXTURE_ATRIBUTES[NUM_TEXTURES]
	{ { "characters1.png", 4, 14 },{ "wall2.png", 1, 1 } ,{ "food2.png",1,1 },{ "cereza.png",1,1 },{ "spritefont.png",5,13 },{"playbutton.png",1,1 },{ "exitbutton.png",1,1 },{ "loadbutton.png",1,1 },{ "resumebutton.png",1,1 },{ "savebutton.png",1,2 } };

	//PROPIEDADES Y VARIABLES DE LA VENTANA
	SDL_Window*	window = nullptr;
	SDL_Renderer* renderer = nullptr;

	GameStateMachine* stateMachine;

	bool exit = false;
	bool hasSaveFile = false;

	//Region canvas{ 0,0 };

	//Region GUI{ 0,0 };

	int winX, winY; // Posición de la ventana

	//PROPIEDADES Y VARIABLES DEL JUEGO (Flags de control)
	Texture* textures[NUM_TEXTURES]; // | PERSONAJES | MURO | COMIDA | BONUS
	Texture* texts[NUM_TEXTS]; //Textos
	Font* gameFont;
	string userName;

	void loadTextures();

public:

	Game();
	~Game();
	void run();
	void render();
	void update();
	void handleEvents();
	Texture* getTexture(int numTexture) { return textures[numTexture]; };
	Texture* getText(int numText) { return texts[numText]; };
	void pushGameState(GameState* gameState);
	void popGameState();
	GameStateMachine* getStateMachine() { return stateMachine; }
	void setUserName(string userName) { this->userName = userName; }
	string getUserName() { return this->userName; }
	void setHasSaveFile(bool hasSave) { this->hasSaveFile = hasSave; }
	bool isHasSaveFile() { return this->hasSaveFile; }
	void endGame() { this->exit = true; }
	uint writeCode();
};
