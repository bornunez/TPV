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

EndState::EndState(Game* game, bool gameOver) : GameState(game)
{
	int yAlign = (windowReg.h / (END_BUTTONS * 2 + 1));
	stage.push_back(new MenuButton(MIDDLESCREEN, 1 * yAlign, B_WIDTH, B_HEIGHT, game->getTexture(PlayButton), newGame, game));
	stage.push_back(new MenuButton(MIDDLESCREEN, 3 * yAlign, B_WIDTH, B_HEIGHT, game->getTexture(ExitButton), exit, game));

	if(gameOver)
		game->getText(GameOverText)->loadFromText("YOU LOSE", *gameFont, { 255,255,255 });
	else
		game->getText(GameOverText)->loadFromText("YOU WIN", *gameFont, { 255,255,255 });
}

EndState::~EndState()
{
}

void EndState::render()
{
	GameState::render();
	game->getText(GameOverText)->render({ MIDDLESCREEN, (windowReg.h / (END_BUTTONS * 2 + 1)) * 2, B_WIDTH,  B_HEIGHT });
}

void EndState::handleEvent(SDL_Event event)
{
	GameState::handleEvent(event);
}

