#include "MapGenerator.h"

char mapArray[45][45] = { 0,};
bool renderMapAlready = false;
int XlocationX = 0, XlocationY = 0, hardness = 95, chanceForGoal = 5;

void MapGenerator::generateMap(int PlayerX, int PlayerY){

		COORD c;
		//----------------THIS IS THE COLOUR CODES----------------
		WORD floorColor = 0x0A, wallColor = 0x0C, blackColor = 0x0000, echoedFloor = 0x08;

		//----------------THIS IS THE WALL SYMBOL FROM ASCII----------------
		char walls = 178, floors = 176, stair = 'X';

		//----------------THIS IS TO SET THE SIZE OF MAZE----------------
		int x = 43, y = 23;
		if (!renderMapAlready) {

			bool spawnPoint = false;

			//----------------THIS USES TIME AS A FACTOR IN THE RAND----------------
			srand(time(NULL));

			//----------------THE LOOP FOR THE OUTER RING AND MAKES UP THE SIZE OF THE MAZE----------------
			for (int j = 0; j <= y; j++)
			{
				//----------------THE LOOP INNER MAZE----------------
				for (int i = 0; i <= x; i++)
				{
					if (j == 0 || j == y)
					{
						//----------------THIS COUTS THE OUTER RING AS WELL----------------
						mapArray[i][j] = walls;
					}
					else
					{
						if (i == 0 || i == x)
						{
							//----------------THIS COUTS THE OUTER RING AS WELL----------------
							mapArray[i][j] = walls;
						}
						else
						{
							/*----------------THIS IS WHERE THE JUICY PART OF THE MAZE COMES IN----------------
							----------------USING RAND % 100 MAKES ALL THE NUMBER GENERATED LESS THAN 100----------------*/
						if (rand() % 100 > hardness)
							{
								//----------------THIS GIVES THE FACTOR OF RANDOMNESS AND CREATES A MAZE----------------
								if (rand() % 100 < chanceForGoal && !spawnPoint)
								{
									//----------------THIS SETS THE END POINT----------------
									mapArray[i][j] = stair;

									//----------------SAVE LOCATION OF END POINT----------------
									XlocationX = i;
									XlocationY = j;
									spawnPoint = true;
								}
								else
								{
									//----------------THIS SETS THE WALLS----------------
									mapArray[i][j] = walls;
								}

							}
							else
							{
								//----------------THIS SETS THE EMPTY SPACES----------------
								mapArray[i][j] = floors;
							}

						}
					}
				}
				//----------------INCREASES CHANCE OF SPAWNING GOAL POINT----------------
				if(!spawnPoint)
					chanceForGoal++;
			}

			//----------------SETTING PLAYER LOCATION TO FLOOR SO IT WILL NOT STUCK IN WALL OR DIE A TERRIBU DEATHFU----------------
			mapArray[PlayerX][PlayerY] = floors;
			renderMapAlready = true;
		}
	}

//----------------GET CHARACTER FROM MAP ARRAY USING X AND Y COORDINATES----------------
	char MapGenerator::getArrayCharacter(int x, int y)
	{
		return mapArray[x][y];
	}

	//----------------GENERATE MAP ONCE----------------
	void MapGenerator::setRenderMapAlready(bool boolean)
	{
		renderMapAlready = boolean;
	}

	//----------------GET LOCATION OF GOAL (X)----------------
	int MapGenerator::getStairLocationX()
	{
		return XlocationX;
	}

	//----------------GET LOCATION OF GOAL (Y)----------------
	int MapGenerator::getStairLocationY()
	{
		return XlocationY;
	}

	//----------------RESET HARDNESS LEVEL OR GOAL CHANCE SPAWNING(FOR GENERATING MAP AGAIN)----------------
	void MapGenerator::setBackToDefault(bool hardnessLevel, bool goalChanceSpawning)
	{
		if(hardnessLevel)
			hardness = 95;

		if(goalChanceSpawning)
			chanceForGoal = 5;
	}

	//----------------INCREASE DIFFICULTY AS LEVEL PROGRESSES----------------
	void MapGenerator::increaseDifficulty()
	{
		if(hardness >= 67)
			hardness -= 2;
	}

	