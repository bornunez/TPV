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
void Text::set(string newText, int boxX, int boxY, int boxW, int boxH)
{
	text = newText;
	box.x = boxX; box.y = boxY;
	box.w = boxW; box.h = boxH;
	charW = boxW / text.size(); charH = boxH;
}
void Text::drawChar(char c,int n) {
	SDL_Rect destRect;
	//RECT DEL CARACTER A PINTAR
	destRect.w = charW; destRect.h = charH;
	destRect.x = box.x + n * charW; destRect.y = box.y;

	int row, col;
	bool draw = true;

	//NUMEROS 0 - 9
	if (c >= '0' && c <= '9') {
		row = 0;
		col = c - '0';
	}
	//MAYUSCULAS
	else if (c >= 'A' && c <= 'Z') {
		row = 1 + (c - 'A') / (('Z' - 'A' + 1) / 2);
		col = ((c - 'A') % ((('Z' - 'A') / 2) + 1));
	}
	//MINUSCULAS
	else if (c >= 'a' && c <= 'z') {
		row = 3 + ((c - 'a') / (('z' - 'a' + 1) / 2));
		col = ((c - 'a') % ((('z' - 'a') / 2) + 1));
	}
	else
		draw = false;
	if(draw)
		texture->renderFrame(destRect, row, col);
}

void Text::render() {
	for (int i = 0; i < text.size(); i++) {
		drawChar(text[i], i);
	}
}
