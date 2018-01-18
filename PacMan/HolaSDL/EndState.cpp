#include "EndState.h"
#include "PlayState.h"


void EndState::newGame(Game * game)
{
	game->getStateMachine()->popState();
}
void EndState::exit(Game * game)
{
	game->getStateMachine()->exit();
}

EndState::EndState(Game* game) : GameState(game)
{
	int yAlign = (windowReg.h / (END_BUTTONS * 2 + 1));

	stage.push_back(new MenuButton(MIDDLESCREEN, 1 * yAlign, B_WIDTH, B_HEIGHT, game->getTexture(PlayButton), newGame, game));
	stage.push_back(new MenuButton(MIDDLESCREEN, 3 * yAlign, B_WIDTH, B_HEIGHT, game->getTexture(ExitButton), exit, game));

}
EndState::~EndState()
{
}

void EndState::handleEvent(SDL_Event event)
{
	GameState::handleEvent(event);
}

