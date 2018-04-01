#pragma once
#include "MainMenuState.h"
#include <fstream>

MainMenuState::MainMenuState(Game* game) : GameState(game)
{
	int yAlign = (windowReg.h / (MENU_BUTTONS * 2 + 1));

	stage.push_back(new MenuButton(MIDDLESCREEN, 1 * yAlign, B_WIDTH, B_HEIGHT, game->getTexture(PlayButton), menuToPlay, game));
	stage.push_back(new MenuButton(MIDDLESCREEN, 3 * yAlign, B_WIDTH, B_HEIGHT, game->getTexture(ExitButton), exitFromMenu, game));
	stage.push_back(new MenuButton(MIDDLESCREEN, 5 * yAlign, B_WIDTH, B_HEIGHT, game->getTexture(LoadButton), loadGame, game));
}

void MainMenuState::menuToPlay(Game* game)
{
	game->getStateMachine()->pushState(new Colisiones(game));

}

void MainMenuState::exitFromMenu(Game* game)
{
	game->getStateMachine()->popState();
}


void MainMenuState::loadGame(Game* game)
{
	uint code = game->writeCode();
	game->pushGameState(new PlayState(game,Utilities::intToStr(code)));
}

void MainMenuState::handleEvent(SDL_Event& event) {
	GameState::handleEvent(event);
}
