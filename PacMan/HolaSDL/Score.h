#pragma once
#include <vector>;
#include <string>;
#include <fstream>;
#include <iostream>;

using namespace std;

struct ScoreReg {
	string nameReg;
	int score;
};

class Score
{
private:
	vector <ScoreReg> scores;

public:
	Score();
	Score(const string& filename);
	~Score();
	bool load(const string& filename);
	bool save(const string& filename);
	vector <ScoreReg> getScoreReg() { return scores; }
	void changeScoreReg(ScoreReg scoreToChange, int score);
	void printTopNScores(int n);
	void addScore(const string& name, int score);
};

