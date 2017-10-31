#pragma once

#include "SDL.h"
#include "GameObject.h"
#include <vector>

using namespace std;

enum MapCell { Empty, Wall, Food, Vitamins };

class GameMap
{
private:
	Texture* CellTex;
	Texture* EmptyCellTex;
public:
	GameMap();
	~GameMap();

};

