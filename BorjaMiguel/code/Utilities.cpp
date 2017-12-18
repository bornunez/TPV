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