#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

class MapGenerator {

public:
	//----------------GENERATE MAP----------------
	void generateMap(int PlayerX, int PlayerY);

	//----------------GET CHARACTER FROM MAP ARRAY USING X AND Y COORDINATES----------------
	char getArrayCharacter(int x, int y);

	//----------------GENERATE MAP ONCE----------------
	void setRenderMapAlready(bool boolean);
	//----------------RESET HARDNESS LEVEL OR GOAL CHANCE SPAWNING(FOR GENERATING MAP AGAIN)----------------
	void setBackToDefault(bool hardnessLevel, bool goalChanceSpawning);
	//----------------INCREASE DIFFICULTY AS LEVEL PROGRESSES----------------
	void increaseDifficulty();

	//----------------GET LOCATION OF GOAL----------------
	int getStairLocationX();
	int getStairLocationY();

};	


