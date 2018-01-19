#include "Utilities.h"

string Utilities::intToStr(int number)
{
	string result;         

	stringstream convert;   

	convert << number;      

	result = convert.str(); 

	return result;
}
void Utilities::enumToDir(Direction dir, int&ndx, int& ndy) {
	if (dir == Left) {
		ndx = -1;
		ndy = 0;
	}
	else if (dir == Up) {
		ndx = 0;
		ndy = -1;
	}
	else if (dir == Down) {
		ndx = 0;
		ndy = 1;
	}
	else if (dir == Right) {
		ndx = 1;
		ndy = 0;
	}
}

Direction Utilities::dirToEnum(int dx, int dy) {
	Direction dir;
	if (dx == -1 && dy == 0)
		dir = Left;

	else if (dx == 0 && dy == -1)
		dir = Up;

	else if (dx == 0 && dy == 1)
		dir = Down;

	else if (dx == 1 && dy == 0)
		dir = Right;

	else if (dx == 0 && dy == 0)
		dir = None;

	return dir;
}


Direction Utilities::backDir(Direction currDir) { //DADA UNA DIRECCION DIR, DEVOLVEMOS LA INVERSA
	if (currDir == Up)
		return Down;
	else if (currDir == Down)
		return Up;
	else if (currDir == Left)
		return Right;
	else if (currDir == Right)
		return Left;
	else
		return None;
}

void Utilities::getCardinals(int x, int y, vector<pair<int, int>>& cardinals)
{
	pair<int, int> actCard;
	for (int i = 0; i < 4; i++) {
		int nx, ny;
		enumToDir((Direction)i, nx, ny);
		//Ya tenemos las direcciones
		actCard.first = x + nx;
		actCard.second = y + ny;
		//Y ahora lo guardamos en el vector
		cardinals.push_back(actCard);
	}
}
