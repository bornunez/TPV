#pragma once
#include "SDL.h";
#include "SDL_image.h";
#include <string>;

using namespace std;
typedef unsigned int uint;

class Texture
{
private:
	SDL_Texture* texture = nullptr;
	uint w = 0;	//	Texture	width (pixels)
	uint h = 0;	//	Texture	height (pixels)
	uint fw = 0; //	Frame width	(pixels)	
	uint fh = 0; //	Frame height (pixels)
public:
	Texture();
	~Texture();
	bool load(SDL_Renderer*	renderer, string filename, uint numRows = 1, uint numCols = 1);
	void render(SDL_Renderer*	renderer, const SDL_Rect&	rect, SDL_RendererFlip	flip = SDL_FLIP_NONE);
	void renderFrame(SDL_Renderer*	renderer, const SDL_Rect& destRect, int row, int col, SDL_RendererFlip flip = SDL_FLIP_NONE);
};

