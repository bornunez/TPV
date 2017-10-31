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
	SDL_Renderer* renderer = nullptr;
	uint w = 0;	//	Texture	width (pixels)
	uint h = 0;	//	Texture	height (pixels)
	uint fw = 0; //	Frame width	(pixels)	
	uint fh = 0; //	Frame height (pixels)
	uint numRows = 1;
	uint numCols = 1;

public:
	Texture();
	Texture(SDL_Renderer* r) : renderer(r) {};
	~Texture(); //{ free(); }
	bool load(string filename, uint numRows = 1, uint numCols = 1);
	void render(const SDL_Rect&	rect, SDL_RendererFlip	flip = SDL_FLIP_NONE);
	void renderFrame(const SDL_Rect& destRect, int row, int col, SDL_RendererFlip flip = SDL_FLIP_NONE);
	uint getNumRows() { return numRows; }
	uint getNumCols() { return numCols; }

	//En vez de SDL_DEST se pone {0,0,WINWIDHT, WOINHEIGTH, o lo que sea
};

