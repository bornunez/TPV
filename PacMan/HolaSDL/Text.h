#pragma once
#include "Texture.h"

struct TextBox {
	int x, y, w, h;
};
class Text
{
private:
	Texture* texture = nullptr;
	string text;

	//CAJA DESTINO
	TextBox box;
	
	//TAMAÑO DEL CARACTER
	int charW = 0;
	int charH = 0;

	void drawChar(char c,int n);

public:
	Text();
	Text(Texture* TEX, string TEXT,int boxX,int boxY, int boxW, int boxH);
	~Text();
	void setText(string newText) { text = newText; }
	void scaleBox(int newWidth, int newHeight) { box.h = newHeight; box.w = newWidth; }
	void render();
};

