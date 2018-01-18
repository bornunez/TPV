#pragma once
#include "GameState.h"
#include "MainMenuState.h"
#include <stack>
using namespace std;

class GameStateMachine
{
private:
	stack<GameState*> gameStateStack;


public:
	GameStateMachine();
	~GameStateMachine();
	virtual GameState* currentState();
	void pushState(GameState* state);
	void changeState(GameState* state);
	void exit();
	void popState();
	bool empty() { return gameStateStack.empty(); }


};

