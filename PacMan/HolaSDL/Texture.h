#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <iostream>

using namespace std;
typedef unsigned int uint;

class Texture
{
private:
	string PATH;
	SDL_Texture* texture = nullptr;
	SDL_Renderer* renderer = nullptr;
	//INFORMACION DE LA IMAGEN
	uint w = 0;	//	Texture	width (pixels)
	uint h = 0;	//	Texture	height (pixels)
	uint fw = 0; //	Frame width	(pixels)	
	uint fh = 0; //	Frame height (pixels)
public:
	Texture(SDL_Renderer* r,const string path);
	~Texture();
	bool load(string filename, uint numRows = 1, uint numCols = 1);
	void render(const SDL_Rect&	rect, SDL_RendererFlip	flip = SDL_FLIP_NONE);
	void renderFrame(const SDL_Rect& destRect, int row, int col, SDL_RendererFlip flip = SDL_FLIP_NONE);
};

