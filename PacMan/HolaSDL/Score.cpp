#include "Score.h"

Score::Score()
{

}


Score::~Score()
{
}

bool Score::load(const string& filename) {

	ifstream ld(filename);
	ScoreReg NewScore;
	string name;
	if (!ld.is_open()) //En caso de que haya problemas, salimos y devolvemos false
		return false;

	//Primera lectura para comprobar que no esta vacio
	getline(ld, name);

	//A partir de aqui Leemos nombre, leemos punt y saltamos linea
	while (!ld.fail())
	{
		//Guardamos la score
		ld >> NewScore.score;
		NewScore.nameReg = name;

		//Alamcenamos la score en la lista de las scores
		scores.push_back(NewScore);

		//Consumimos salto de linea
		getline(ld, name);

		//Y volvemos a leer el nombre. Ademas nos dara info de si ha fallado
		getline(ld, name);
	}
	//Cerramos el archivo por seguridad
	ld.close();
	return true;
}

void Score::printTopNScores(int n) {
	//Si sobrepasa el maximos, se muestra la lista entera
	if (n > scores.size())
		n = scores.size();
	for (int i = 0; i < n; i++) {
		cout << "TOP " << i+1 << " : "  << scores[i].nameReg ;
		cout << " || Score: " << scores[i].score << endl;
	}
}
void Score::addScore(const string& name, int score) {

	int i = 0;
	//Vamos a la primera posicion menor o igual a la score, y la introducimos ahi 
	//De tal modo que tengan prioridad los que antes consiguieron la puntuacion
	while (i < scores.size() && score <= scores[i].score)
		i++;

	ScoreReg newScore;
	//Guardamos la score en el registro auxiliar
	newScore.nameReg = name;
	newScore.score = score;
	scores.insert(scores.begin() + i, newScore);
}

//Borra un registro y lo crea con un nuevo score conservando el nombre y colocandolo en la posicion adecuada de la tabla
void Score::changeScoreReg(ScoreReg scoreToChange, int score) {
	for (int i = 0; i < scores.size(); i++) {
		if (scores[i].nameReg == scoreToChange.nameReg)
			scores.erase(scores.begin() + i);
	}
	// Se crea un usuario con el mismo nombre que el que se cambio pero con un nuevo score
	addScore(scoreToChange.nameReg, score);
}

bool Score::save(const string& filename) {
	ofstream sv(filename);

	//Comprobamos que se haya abierto correctamente, saliendo y activando el flag
	if (!sv.is_open())
		return false;

	for (int i = 0; i < scores.size(); i++) {
		sv << scores[i].nameReg << endl;
		sv << scores[i].score << endl;
	}
	//Cerramos el archivo
	sv.close();

	return true;
}
