#pragma once
#include <string>
#include <sstream>

using namespace std;

enum Direction { Right, Down, Left, Up, None };

// Estructura para las texturas
typedef struct {
	string filename;
	int numRows;
	int numCols;
} TextureAtributes;

//

// Estructura para las regiones del juego
typedef struct {
	int w;
	int h;
} Region;

class Utilities
{
public:
	static string intToStr(int number);
	static void enumToDir(Direction dir, int&ndx, int& ndy);
	static Direction backDir(Direction currDir);
};

