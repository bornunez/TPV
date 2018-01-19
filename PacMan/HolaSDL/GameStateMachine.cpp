#include "GameStateMachine.h"



GameStateMachine::GameStateMachine()
{
}


GameStateMachine::~GameStateMachine()
{
}

//Devuelve el ultimo elemento insertado en la pila, si la pila esta vacia devuelve nullptr
GameState* GameStateMachine::currentState()
{
	if(!empty())
		return gameStateStack.top();
	else 
		return nullptr;
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