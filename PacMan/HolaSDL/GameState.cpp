#include "GameState.h"



GameState::GameState(Game* game)
{
	this->game = game;
	MIDDLESCREEN = (windowReg.w - B_WIDTH) / 2;
}

GameState::~GameState()
{
	cout << "Destruyendo Estado..." << endl;
	for (GameObject* o : stage)
		delete o;
}

void GameState::update()
{
	for (GameObject* o : stage)
		o->update();
}

void GameState::render()
{
	for (GameObject* o : stage)
		o->render();
}

void GameState::handleEvent(SDL_Event& event)
{
	bool handled = false;
	auto it = stage.begin();
	while (!handled && it != stage.end()) {

		if ((*it)->handleEvent(event))
			handled = true;
		else
			it++;
	}
}

