#include "SDL_ttf.h"
#include <iostream>
using namespace std;
#pragma once

class Font
{
private:
	TTF_Font*	font = nullptr;
public:
	Font() {};
	Font(string	filename, int	size);
	~Font() { free(); }

	bool load(string	filename, int	size);	
	void free();
	SDL_Surface* generateSurface(string	text, SDL_Color	color) const;
};