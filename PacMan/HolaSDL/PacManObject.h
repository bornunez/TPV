#pragma once
#include "GameObject.h"
#include "PlayState.h"

class PacManObject : public GameObject
{
protected:
	PlayState* playState = nullptr;

public:
	PacManObject();
	~PacManObject();
	virtual void loadFromFile(ifstream& file) = 0;
	virtual void saveToFile(ofstream& file) = 0;
};

