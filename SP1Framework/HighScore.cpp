#include "HighScore.h"
#include "MapGenerator.h"

using std::ofstream;
using std::ifstream;
using std::ios;
using std::endl;


void setHighScore(int score)
{
	ofstream highScore("highScore.txt", ios::app);
	highScore << score << endl;
	highScore.close();
}

int getHighScore()
{
	std::ostringstream ss, ss2;
	int scores, highestScore = 0;

	ifstream highScore;
	highScore.open("highScore.txt");

	while (true)
	{
		if (highScore.eof())
			break;

		if (highestScore == 0)
			highScore >> highestScore;

		highScore >> scores;
		if (scores > highestScore)
			highestScore = scores;
	}
	highScore.close();

	return highestScore;
}