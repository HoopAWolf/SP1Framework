#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>


using namespace std;

class MapGenerator {

public:

	//----------------THIS IS THE WALL SYMBOL FROM ASCII----------------
	char walls = 178, floors = 176, stair = 'X', enemy = 146, player = '#', bomb = 64;

	//----------------THIS IS THE COLOUR CODES----------------
	WORD floorColor = 0x0A, wallColor = 0x0C, blackColor = 0x0000, echoedFloor = 0x08, enemyColor = 0x0C;

	//----------------ALL ENEMY CURRENT POSITIONS----------------
	vector<COORD> allEnemyPosition;
	//----------------ALL ENEMY TIMER FOR MOVEMENTS----------------
	vector<long double> allEnemytimer;


	//----------------GENERATE MAP----------------
	void generateMap(short PlayerX, short PlayerY, short mazeSizeX, short mazeSizeY, double elapsedTimer);

	//----------------GET CHARACTER FROM MAP ARRAY USING X AND Y COORDINATES----------------
	char getArrayCharacter(short x, short y);

	//----------------GENERATE MAP ONCE----------------
	void setRenderMapAlready(bool boolean);
	//----------------RESET HARDNESS LEVEL OR GOAL CHANCE SPAWNING(FOR GENERATING MAP AGAIN)----------------
	void setBackToDefault(bool hardnessLevel, bool goalChanceSpawning, bool resetFloor, bool resetStair, bool resetLevel);
	//----------------INCREASE DIFFICULTY AS LEVEL PROGRESSES----------------
	void increaseDifficulty();

	//----------------GET LOCATION OF GOAL----------------
	int getStairLocationX();
	int getStairLocationY();

	//----------------GET FLOOR LEVEL----------------
	int getFloorLevel();

	//----------------REPLACE MAP ELEMENTS USING COORDINATES X AND Y----------------
	void replaceMapCharacterXY(short x, short y, char newChar);

	//----------------AI MOVEMENTS----------------
	void moveAI(short playerLocationX, short playerLocationY, double elapsedTimer);
	//----------------GENERATE ENEMY----------------
	void generateEnemy(short StairX, short StairY);
	//----------------CHECK FOR EMPTY SPACES(FOR AI MOVEMENTS)----------------
	bool checkForEmptySpace(short x, short y);

};	


