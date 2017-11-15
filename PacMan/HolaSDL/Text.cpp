#include "Text.h"



Text::Text()
{
}


Text::Text(Texture* TEX, string TEXT, int boxX, int boxY, int boxW, int boxH) {
	text = TEXT;
	texture = TEX;
	box.h = boxH; box.w = boxW;
	box.y = boxY; box.x = boxX;
	charW = boxW / text.size(); charH = boxH;
}
Text::~Text() {

}
void Text::drawChar(char c,int n) {
	SDL_Rect destRect;
	//RECT DEL CARACTER A PINTAR
	destRect.w = charW; destRect.h = charH;
	destRect.x = box.x + n * charW; destRect.y = box.y;

	int row, col;

	if (c >= '0' && c <= '9') {
		row = 0;
		col = c - '0';
	}
	else if (c >= 'A' && c <= 'Z') {
		row = 1 + (c - 'A') % (('A' - 'Z') / 2);
		col = c % (('A' - 'Z') / 2);
	}
	else if (c >= 'a' && c <= 'z') {
		row = 1 + (c - 'a') % (('a' - 'z') / 2);
		col = c % (('a' - 'z') / 2);
	}

	texture->renderFrame(destRect, row, col);
}

void Text::render() {
	for (int i = 0; i < text.size(); i++) {
		drawChar(text[i], i);
	}
}