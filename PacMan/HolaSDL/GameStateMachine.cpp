#include "GameStateMachine.h"



GameStateMachine::GameStateMachine()
{
	//PlayState* ps = new PlayState(game);
	//pushState(ps);
}


GameStateMachine::~GameStateMachine()
{
}


GameState* GameStateMachine::currentState()
{
	return gameStateStack.top();
}

void GameStateMachine::pushState(GameState* state)
{
	gameStateStack.push(state);
}

void GameStateMachine::changeState(GameState* state)
{
	popState();
	pushState(state);

}

void GameStateMachine::exit()
{
	while (!gameStateStack.empty())
	{
		popState();
	}
}

void GameStateMachine::popState()
{
	if (!gameStateStack.empty()) {
		GameState* top = gameStateStack.top();
		gameStateStack.pop();
		delete top;
	}
}