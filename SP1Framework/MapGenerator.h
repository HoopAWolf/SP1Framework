#ifndef _MAPGENERATOR_H
#define _MAPGENERATOR_H

#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>


using  std::vector;

class MapGenerator {

public:

	//----------------THIS IS THE WALL SYMBOL FROM ASCII----------------
	char walls = 178, floors = 176, stair = 'X', enemy = 146, bomb = 64, torch = 'I', bombDrop = 'B', ammoDrop = 'R', verticalLazer = '|', horizontalLazer = 196, enemyGrave = 206;

	bool hellMode;

	COORD bombCoord;

	//----------------THIS IS THE COLOUR CODES----------------
	WORD floorColor = 0x0A, wallColor = 0x0C, blackColor = 0x0000, echoedFloor = 0x08, enemyColor = 0x0C, torchColor = 0x0D, itemDropColor = 0x0D, lazerColor = 0xAC;

	//----------------ALL ENEMY CURRENT POSITIONS----------------
	vector<COORD> allEnemyPosition;
	//----------------ALL ENEMY TIMER FOR MOVEMENTS----------------
	vector<long double> allEnemytimer;

	//----------------ALL LAZER TIMER FOR MOVEMENTS----------------
	vector<long double> allLazerTimer;

	//----------------ALL LAZER POSITION FOR MOVEMENTS----------------
	vector<COORD> allLazerPosition;

	//----------------ALL LAZER POSITION FOR MOVEMENTS----------------
	vector<long int> allLazerFacing;// 0 - UP, 1 - DOWN, 2 - LEFT, 3 - RIGHT

	//----------------GENERATE MAP----------------
	void generateMap(short PlayerX, short PlayerY, short mazeSizeX, short mazeSizeY, double elapsedTimer);
	//----------------3X3 VISION----------------
	void playerVision(int elapsedTimer, int PlayerX, int PlayerY);

	//----------------GET CHARACTER FROM MAP ARRAY USING X AND Y COORDINATES----------------
	char getArrayCharacter(short x, short y);

	//----------------GENERATE MAP ONCE----------------
	void setRenderMapAlready(bool boolean);
	//----------------RESET HARDNESS LEVEL OR GOAL CHANCE SPAWNING(FOR GENERATING MAP AGAIN)----------------
	void setBackToDefault(bool hardnessLevel, bool goalChanceSpawning, bool resetFloor, bool resetStair, bool resetLevel);
	//----------------INCREASE DIFFICULTY AS LEVEL PROGRESSES----------------
	void increaseDifficulty();
	//----------------RENDERING TORCH LIT UP AREA----------------
	void torchView();
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
	//----------------CHECK FOR EMPTY SPACES(FOR AI MOVEMENTS AND LAZER)----------------
	bool checkForEmptySpace(short x, short y);
	bool checkForEmptySpaceLazer(short x, short y);

	//----------------SPAWNS LAZER----------------
	void spawnLazer(short facing, int playerX, int playerY, double elapsedTimer);
	//----------------MOVES LAZER----------------
	void moveLazer(double elapsedTimer);
	//----------------RESET LAZER FACING, COORD AND TIMING VECTORS----------------
	void resetLazer();
	//----------------RENDER LIGHT ON LAZER----------------
	void lazerView();
};	


#endif