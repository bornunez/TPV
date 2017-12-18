#include "Font.h"



Font::Font(string	filename, int	size) {
	load(filename, size);
}




bool Font::load(string	filename, int	size) {
	font = TTF_OpenFont(filename.c_str(), size);
	return true;
}


void Font::free() {
	if (font != nullptr)	
		TTF_CloseFont(font);
	font = nullptr;
}


SDL_Surface* Font::generateSurface(string	text, SDL_Color	color)	const {
	return	TTF_RenderText_Solid(font, text.c_str(), color);
}