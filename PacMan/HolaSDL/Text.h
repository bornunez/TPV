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
	
	//TAMA�O DEL CARACTER
	int charW = 0;
	int charH = 0;

	void drawChar(char c,int n);

public:
	Text();
	Text(Texture* TEX, string TEXT,int boxX,int boxY, int boxW, int boxH);
	~Text();
	void set(string newText, int boxX, int boxY, int boxW, int boxH);
	void setText(string newText) { text = newText;	charW = box.w / text.size(); charH = box.h;}
	void scaleBox(int newWidth, int newHeight) { box.h = newHeight; box.w = newWidth; }
	void render();
};

