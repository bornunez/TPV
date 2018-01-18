#include "PauseState.h"
#include "GameStateMachine.h"


void PauseState::resume(Game * game)
{
	game->getStateMachine()->popState();
}

void PauseState::exitFromMenu(Game * game)
{
	game->getStateMachine()->exit();
}

void PauseState::saveGame(Game * game)
{
	uint code = game->writeCode();
	game->getStateMachine()->popState();
	static_cast<PlayState*> (game->getStateMachine()->currentState())->saveToFile(Utilities::intToStr( code));
	cout << code;
	game->getStateMachine()->exit();
}

PauseState::PauseState(Game* game) : GameState(game)
{
	int yAlign = (windowReg.h / (PAUSE_BUTTONS* 2 + 1));

	stage.push_back(new MenuButton(MIDDLESCREEN, 1 * yAlign, B_WIDTH, B_HEIGHT, game->getTexture(ResumeButton), resume, game));
	stage.push_back(new MenuButton(MIDDLESCREEN, 2 * yAlign, B_WIDTH, B_HEIGHT, game->getTexture(ExitButton), exitFromMenu, game));
	stage.push_back(new MenuButton(MIDDLESCREEN, 3 * yAlign, B_WIDTH, B_HEIGHT, game->getTexture(SaveButton), saveGame, game));
}


PauseState::~PauseState()
{
}
