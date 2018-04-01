#include "Personaje.h"



void Personaje::render()
{
	SDL_Rect destRect;
	destRect.x = rect->x; destRect.y = rect->y;
	destRect.w = rect->w; destRect.h = rect ->h;
	texture->render(destRect);
}

Personaje::~Personaje()
{
}
