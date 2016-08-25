#include "MapGenerator.h"
#include "Entity.h"
#include "Inventory.h"
#include "Curse.h"

COORD coord;

char mapArray[45][45] = { '0',};
extern Console g_Console;
int bombCountDownTimer = 0;

bool renderMapAlready = false;

short XlocationX = 0, XlocationY = 0,
hardness = 95 /*THE HIGHER THE HARDNESS, THE EASIER IT IS*/, chanceForGoal = 5, 
FloorNo = 0, 
timer;

MapGenerator mapGen;
extern Entity entityBase;
extern Inventory inven;
extern Curse curse;

void MapGenerator::generateMap(short PlayerX, short PlayerY, short mazeSizeX, short mazeSizeY, double elapsedTimer)
{

COORD c;

if (!renderMapAlready) {

	allEnemyPosition.clear();
	allEnemytimer.clear();
	bool spawnPoint = false;

	//----------------THIS USES TIME AS A FACTOR IN THE RAND----------------
	srand(time(NULL));

	//----------------THE LOOP FOR THE OUTER RING AND MAKES UP THE SIZE OF THE MAZE----------------
	for (int j = 0; j <= mazeSizeY; j++)
	{
		//----------------THE LOOP INNER MAZE----------------
		for (int i = 0; i <= mazeSizeX; i++)
		{
			if (j == 0 || j == mazeSizeY)
			{
				//----------------THIS COUTS THE OUTER RING AS WELL----------------
				mapArray[i][j] = walls;
			}
			else
			{
				if (i == 0 || i == mazeSizeX)
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
							//----------------THIS SETS THE END POINT ALSO SAVE LOCATION OF END POINT----------------
							if (XlocationX == 0 && XlocationY == 0){
								XlocationX = i;
								XlocationY = j;
							}
							else
							{
								
									if (i < (mazeSizeX / 2) && j < (mazeSizeY / 2)
										&& PlayerX < (mazeSizeX / 2) && PlayerY < (mazeSizeY / 2))
									{
										XlocationX = i + (rand() % 20) + 5;
										XlocationY = j + (rand() % 20) + 5;

									}
									else if (i < (mazeSizeX / 2) && j >(mazeSizeY / 2)
										&& PlayerX < (mazeSizeX / 2) && PlayerY >(mazeSizeY / 2))
									{
										XlocationX = i + (rand() % 20) + 5;
										XlocationY = j - (rand() % 20) + 5;

									}
									else if (i > (mazeSizeX / 2) && j < (mazeSizeY / 2)
										&& PlayerX >(mazeSizeX / 2) && PlayerY < (mazeSizeY / 2))
									{
										XlocationX = i - (rand() % 20) + 5;
										XlocationY = j + (rand() % 20) + 5;

									}
									else if (i > (mazeSizeX / 2) && j > (mazeSizeY / 2)
										&& PlayerX > (mazeSizeX / 2) && PlayerY > (mazeSizeY / 2))
									{
										XlocationX = i - (rand() % 20) + 5;
										XlocationY = j - (rand() % 20) + 5;

									}
									else
									{
										XlocationX = i;
										XlocationY = j;
									}
								}
							//----------------MAKE SURE TO NOT SPAWN END POINT OUTSIDE OF MAZE----------------
							if (XlocationX > 0 && XlocationX < 43 && XlocationY > 0 && XlocationY < 23)
							{
								mapArray[XlocationX][XlocationY] = stair;
								spawnPoint = true;
							}
						}
						else
						{
							//----------------THIS SETS THE WALLS----------------
							if (mapArray[i][j] != stair)
								mapArray[i][j] = walls;
						}

					}
					else
					{
						//----------------THIS SETS THE EMPTY SPACES AND DROPS----------------
						if (mapArray[i][j] != stair)
						{
							if (rand() % 100 < 1 && getFloorLevel() > 17)
							{
								if (rand() % 100 < 65)
									mapArray[i][j] = mapGen.ammoDrop;
								else
									mapArray[i][j] = bombDrop;
							}
							else
								mapArray[i][j] = floors;
						}
					}

				}
			}
		}
		//----------------INCREASES CHANCE OF SPAWNING GOAL POINT----------------
		if (!spawnPoint)
			chanceForGoal++;
	}

	//----------------SETTING PLAYER LOCATION TO FLOOR SO IT WILL NOT STUCK IN WALL OR DIE A TERRIBU DEATHFU----------------
	mapArray[PlayerX][PlayerY] = floors;
	FloorNo++;

	if (getFloorLevel() > 15)
	{
		if (getFloorLevel() % 2 == 0)
		{
			inven.setInventoryBombCount(((getFloorLevel() > 25) ? inven.getBombCount() + 0 : inven.getBombCount() + 1));
			inven.setInventoryLaserRifleAmmountCount(((getFloorLevel() > 25) ? inven.getLaserRifleAmmoCount() + 0 : inven.getLaserRifleAmmoCount() + 1));
		}
		inven.setInventoryTorchCount(((getFloorLevel() > 23) ? 2 : 1));
	}

	if (getFloorLevel() > 5)
		curse.startARandomCurse();

	//----------------GENERATE ENEMY----------------
	if (getFloorLevel() > 15)
	{
		generateEnemy(XlocationX, XlocationY);
	}

	bombCountDownTimer = 0;

	renderMapAlready = true;
}

if (renderMapAlready)
{
	inven.clearInventory();
	//---------------------GIVE ITEM AT CERTAIN LEVEL---------------------
	if (getFloorLevel() > 1)
		inven.setInventory(((getFloorLevel() > 5) ? true : false), ((getFloorLevel() > 15) ? true : false), ((getFloorLevel() > 19) ? true : false), ((getFloorLevel() > 17) ? true : false));

	if(allEnemyPosition.size() != 0)
		moveAI(PlayerX, PlayerY, elapsedTimer);

	if (allLazerPosition.size() != 0)
		moveLazer(elapsedTimer);

	mapArray[XlocationX][XlocationY] = stair;
	inven.torchLocation.clear();

	//----------------WRITING THE MAP FROM ARRAY INTO THE CONSOLE BUFFER----------------
	for (int j = 0; j < mazeSizeY; j++)
	{
		COORD torchCoord;
		c.Y = j;
		for (int i = 0; i < mazeSizeX; i++)
		{
			c.X = i;

			//----------------SETTING MAP TO TOTAL DARKNESSSSSS AS DARK AS SHISHANTH'S HEART----------------
			if (mapArray[i][j] == torch)
			{
				g_Console.writeToBuffer(c, mapArray[i][j], torchColor);
				torchCoord.X = i;
				torchCoord.Y = j;
				inven.torchLocation.push_back(torchCoord);
			}
			else if (mapArray[i][j] == verticalLazer || mapArray[i][j] == horizontalLazer)
				g_Console.writeToBuffer(c, mapArray[i][j], lazerColor);
			else if (mapArray[i][j] == bomb)
			{
				if (bombCountDownTimer == 0)
					bombCountDownTimer = elapsedTimer;

				//----------------SETTING BOMB COUNTDOWN----------------
				if (inven.getBombCountDown() > 0)
				{
					if (bombCountDownTimer == 0)
						bombCountDownTimer = elapsedTimer + 1;

					if (elapsedTimer > bombCountDownTimer)
					{
						//------------DECREASE BOMB COUNT DOWN------------
						inven.decreaseBombCountDown();

					}

					if (inven.getBombCountDown() <= 0)
					{
						//----------------EXPLODE WHEN BOMB REACHES 0----------------
						inven.useExplosion(i, j, PlayerX, PlayerY);
						mapGen.replaceMapCharacterXY(i, j, mapGen.floors);
					}

					//----------------MAKE THE BOMB FLASH AND EXPLODE----------------
					if (bombCountDownTimer % 2 == 0)
						g_Console.writeToBuffer(c, mapGen.getArrayCharacter(i, j), 0x0C/*red*/);
					else
						g_Console.writeToBuffer(c, mapGen.getArrayCharacter(i, j), 0x0A/*green*/);

					bombCountDownTimer = elapsedTimer + 1;
				}
			}

			else //----------------SETTING MAP TO TOTAL DARKNESSSSSS AS DARK AS SHISHANTH'S HEART----------------
				g_Console.writeToBuffer(c, mapArray[i][j], blackColor);

		}
	}

	//--------------------------------IF PLAYER PICK UP ITEM--------------------------------------
	if (mapGen.getArrayCharacter(PlayerX, PlayerY) == mapGen.bombDrop)
	{
		mapGen.replaceMapCharacterXY(PlayerX, PlayerY, mapGen.floors);
		inven.setInventoryBombCount(inven.getBombCount() + 1);
	}

	if (mapGen.getArrayCharacter(PlayerX, PlayerY) == mapGen.ammoDrop)
	{
		mapGen.replaceMapCharacterXY(PlayerX, PlayerY, mapGen.floors);
		inven.setInventoryLaserRifleAmmountCount(inven.getLaserRifleAmmoCount() + 1);
	}

}

}

//----------------GET CHARACTER FROM MAP ARRAY USING X AND Y COORDINATES----------------
char MapGenerator::getArrayCharacter(short x, short y)
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
void MapGenerator::setBackToDefault(bool hardnessLevel, bool goalChanceSpawning, bool resetFloor, bool resetStair, bool resetLevel)
{
	if(hardnessLevel)
		hardness = 95;

	if(goalChanceSpawning)
		chanceForGoal = 5;

	if (resetFloor)
		FloorNo = 0;

	if (resetStair)
		mapArray[XlocationX][XlocationY] = floors;

	if (resetLevel)
		renderMapAlready = false;
}

//----------------INCREASE DIFFICULTY AS LEVEL PROGRESSES----------------
void MapGenerator::increaseDifficulty()
{
	if(hardness >= 65)
		hardness -= 2;
}

//----------------GET FLOOR LEVEL----------------
int MapGenerator::getFloorLevel()
{
	return FloorNo;
}

//----------------REPLACE MAP ELEMENTS USING COORDINATES X AND Y----------------
void MapGenerator::replaceMapCharacterXY(short x, short y, char newChar)
{
	mapArray[x][y] = newChar;
}


//----------------GENERATE ENEMY----------------
void MapGenerator::generateEnemy(short StairX, short StairY)
{
	srand(time(NULL));
	allEnemyPosition.clear();
	allEnemytimer.clear();

	int radiusSpawnX = (rand() % 4) + 1, radiusSpawnY = (rand() % 4) + 1;

	for (int y = -radiusSpawnY; y <= radiusSpawnY; y++)
	{
		for (int x = -radiusSpawnX; x <= radiusSpawnX; x++)
		{
			//---------------------INCRASE CHANCE OF SPAWNING PER LEVEL---------------------
			if (rand() % 100 > ((getFloorLevel() > 18) ? ((getFloorLevel() > 20) ? 65 : 75): 85))
			{
				if (y != 0 && x != 0)
				{
					if (getArrayCharacter(StairX + x, StairY + y) == floors)
					{
						coord.X = StairX + x;
						coord.Y = StairY + y;

						mapArray[coord.X] [coord.Y] = enemy;
						allEnemyPosition.push_back(coord);
					}

				}
			}
		}
	}
}

//----------------AI MOVEMENTS----------------
void MapGenerator::moveAI(short playerLocationX, short playerLocationY, double elapsedTimer)
{
	srand(time(NULL));

	bool isPlayerNearby = false;
	int differenceBetweenPlayerAndEnemyX, differenceBetweenPlayerAndEnemyY;

	//----------------SETTING MOVEMENT TIMER----------------
	if (allEnemytimer.size() == 0)
	{
		for (int i = 0; i < allEnemyPosition.size(); i++)
			allEnemytimer.push_back(elapsedTimer);
	}

	for (int j = 0; j < allEnemyPosition.size(); j++)
	{

		int entityUpdatedLocationX = allEnemyPosition[j].X, entityUpdatedLocationY = allEnemyPosition[j].Y;

		//----------------CHECK RADIUS OF 10 X 8----------------
		for (int y = allEnemyPosition[j].Y - 4; y <= allEnemyPosition[j].Y + 4; y++)
		{
			for (int x = allEnemyPosition[j].X - 5; x <= allEnemyPosition[j].X + 5; x++)
			{
				if (x == playerLocationX && y == playerLocationY)
				{
					isPlayerNearby = true;

					//----------------GETTING DISTANCE BETWEEN ENEMY AND PLAYER----------------
					differenceBetweenPlayerAndEnemyX = allEnemyPosition[j].X - playerLocationX;
					differenceBetweenPlayerAndEnemyY = allEnemyPosition[j].Y - playerLocationY;

					//----------------BREAK ONCE CALCULATED DISTANCE----------------
					break;

				}
			}
		}

		//----------------THIS TIMER SLOWS DOWN THEIR MOVEMENTS----------------
		if (elapsedTimer > allEnemytimer[j] + 0.3)
		{

			//----------------CHECKING IF PLAYER IS NEARBY----------------
			if (isPlayerNearby)
			{
				if (differenceBetweenPlayerAndEnemyY != 0)
				{
					//----------------MOVING DOWN(+)----------------
					if (differenceBetweenPlayerAndEnemyY < 0)
					{
						//----------------CHECK FOR EMPTY SPACE BEFORE MOVING ENEMY----------------
						if (checkForEmptySpace(entityUpdatedLocationX, entityUpdatedLocationY + 1))
						{
							mapArray[entityUpdatedLocationX] [entityUpdatedLocationY + 1] = enemy;

							//----------------REPLACING LAST LOCATION WITH FLOOR----------------
							if (mapArray[entityUpdatedLocationX][entityUpdatedLocationY] != stair)
								mapArray[entityUpdatedLocationX][entityUpdatedLocationY] = floors;

							entityUpdatedLocationY += 1;

							//----------------UPDATING POSITION----------------
							coord.X = entityUpdatedLocationX;
							coord.Y = entityUpdatedLocationY;

							//----------------PUSHING UPDATED POSITION INTO VECTOR----------------
							allEnemyPosition[j] = coord;
							allEnemytimer[j] = elapsedTimer;
						}
					}
					//----------------MOVING UP(-)----------------
					else
					{
						if (checkForEmptySpace(entityUpdatedLocationX, entityUpdatedLocationY - 1))
						{
							mapArray[entityUpdatedLocationX][entityUpdatedLocationY - 1] = enemy;

							if (mapArray[entityUpdatedLocationX][entityUpdatedLocationY] != stair)
								mapArray[entityUpdatedLocationX][entityUpdatedLocationY] = floors;

							entityUpdatedLocationY -= 1;
							coord.X = entityUpdatedLocationX;
							coord.Y = entityUpdatedLocationY;
							allEnemyPosition[j] = coord;
							allEnemytimer[j] = elapsedTimer;
						}
					}
				}

				if (differenceBetweenPlayerAndEnemyX != 0)
				{
					//----------------MOVING RIGHT(+)----------------
					if (differenceBetweenPlayerAndEnemyX < 0)
					{
						if (checkForEmptySpace(entityUpdatedLocationX + 1, entityUpdatedLocationY))
						{
							mapArray[entityUpdatedLocationX + 1][entityUpdatedLocationY] = enemy;

							if (mapArray[entityUpdatedLocationX][entityUpdatedLocationY] != stair)
								mapArray[entityUpdatedLocationX][entityUpdatedLocationY] = floors;

							entityUpdatedLocationX += 1;
							coord.X = entityUpdatedLocationX;
							coord.Y = entityUpdatedLocationY;
							allEnemyPosition[j] = coord;
							allEnemytimer[j] = elapsedTimer;
						}
					}
					//----------------MOVING LEFT(-)----------------
					else
					{
						if (checkForEmptySpace(entityUpdatedLocationX - 1, entityUpdatedLocationY))
						{
							mapArray[entityUpdatedLocationX - 1][entityUpdatedLocationY] = enemy;

							if (mapArray[entityUpdatedLocationX][entityUpdatedLocationY] != stair)
								mapArray[entityUpdatedLocationX][entityUpdatedLocationY] = floors;

							entityUpdatedLocationX -= 1;
							coord.X = entityUpdatedLocationX;
							coord.Y = entityUpdatedLocationY;
							allEnemyPosition[j] = coord;
							allEnemytimer[j] = elapsedTimer;
						}
					}
				}
			}
			else
			{

				//----------------MOVING UP(-)----------------
				if (rand() % 100 >= 50)
				{
					if (checkForEmptySpace(entityUpdatedLocationX, entityUpdatedLocationY - 1))
					{
						mapArray[entityUpdatedLocationX][entityUpdatedLocationY - 1] = enemy;

						if (mapArray[entityUpdatedLocationX][entityUpdatedLocationY] != stair)
							mapArray[entityUpdatedLocationX][entityUpdatedLocationY] = floors;

						entityUpdatedLocationY -= 1;
						coord.X = entityUpdatedLocationX;
						coord.Y = entityUpdatedLocationY;
						allEnemyPosition[j] = coord;
						allEnemytimer[j] = elapsedTimer;
					}
				}
				//----------------MOVING DOWN(+)----------------
				if (rand() % 100 >= 50)
				{
					if (checkForEmptySpace(entityUpdatedLocationX, entityUpdatedLocationY + 1))
					{
						mapArray[entityUpdatedLocationX][entityUpdatedLocationY + 1] = enemy;

						if (mapArray[entityUpdatedLocationX][entityUpdatedLocationY] != stair)
							mapArray[entityUpdatedLocationX][entityUpdatedLocationY] = floors;

						entityUpdatedLocationY += 1;
						coord.X = entityUpdatedLocationX;
						coord.Y = entityUpdatedLocationY;
						allEnemyPosition[j] = coord;
						allEnemytimer[j] = elapsedTimer;
					}
				}


				//----------------MOVING RIGHT(+)----------------
				if (rand() % 100 >= 50)
				{
					if (checkForEmptySpace(entityUpdatedLocationX + 1, entityUpdatedLocationY))
					{
						mapArray[entityUpdatedLocationX + 1][entityUpdatedLocationY] = enemy;

						if (mapArray[entityUpdatedLocationX][entityUpdatedLocationY] != stair)
							mapArray[entityUpdatedLocationX][entityUpdatedLocationY] = floors;

						entityUpdatedLocationX += 1;
						coord.X = entityUpdatedLocationX;
						coord.Y = entityUpdatedLocationY;
						allEnemyPosition[j] = coord;
						allEnemytimer[j] = elapsedTimer;
					}
				}
				//----------------MOVING LEFT(-)----------------
				if (rand() % 100 >= 50)
				{
					if (checkForEmptySpace(entityUpdatedLocationX - 1, entityUpdatedLocationY))
					{
						mapArray[entityUpdatedLocationX - 1][entityUpdatedLocationY] = enemy;

						if (mapArray[entityUpdatedLocationX][entityUpdatedLocationY] != stair)
							mapArray[entityUpdatedLocationX][entityUpdatedLocationY] = floors;

						entityUpdatedLocationX -= 1;
						coord.X = entityUpdatedLocationX;
						coord.Y = entityUpdatedLocationY;
						allEnemyPosition[j] = coord;
						allEnemytimer[j] = elapsedTimer;
					}
				}

			}
		}
	}
}

//----------------LAZER MOVEMENTS----------------
void MapGenerator::moveLazer(double elapsedTimer)
{
	COORD lazerCoord;

	for (int i = 0; i < allLazerPosition.size(); i++)
	{

		if (elapsedTimer > allLazerTimer[i] + 0.1)
		{

			if (allLazerFacing[i] == 0)
			{
				if (checkForEmptySpaceLazer(allLazerPosition[i].X, allLazerPosition[i].Y - 1))
				{

					if (mapArray[allLazerPosition[i].X][allLazerPosition[i].Y - 1] == mapGen.enemy)
					{
						for (int j = 0; j < allEnemyPosition.size(); j++)
						{

							lazerCoord.X = allLazerPosition[i].X;
							lazerCoord.Y = allLazerPosition[i].Y - 1;

							if (lazerCoord.X == allEnemyPosition[j].X && lazerCoord.Y == allEnemyPosition[j].Y)
							{
								mapArray[allEnemyPosition[j].X][allEnemyPosition[j].Y] = mapGen.floors;
								mapArray[lazerCoord.X][lazerCoord.Y + 1] = mapGen.floors;
								allLazerPosition.erase(allLazerPosition.begin() + i);
								allLazerFacing.erase(allLazerFacing.begin() + i);
								allLazerTimer.erase(allLazerTimer.begin() + i);

								allEnemyPosition.erase(allEnemyPosition.begin() + j);
								allEnemytimer.erase(allEnemytimer.begin() + j);
							}
						}
						break;
					}

					mapArray[allLazerPosition[i].X][allLazerPosition[i].Y - 1] = mapGen.verticalLazer;

					if (mapArray[allLazerPosition[i].X][allLazerPosition[i].Y] != stair)
						mapArray[allLazerPosition[i].X][allLazerPosition[i].Y] = floors;

					lazerCoord.X = allLazerPosition[i].X;
					lazerCoord.Y = allLazerPosition[i].Y - 1;
					coord.X = lazerCoord.X;
					coord.Y = lazerCoord.Y;
					allLazerPosition[i] = coord;
					allLazerTimer[i] = elapsedTimer;

				}
				else
				{
					mapArray[allLazerPosition[i].X][allLazerPosition[i].Y] = mapGen.floors;
					allLazerPosition.erase(allLazerPosition.begin() + i);
					allLazerFacing.erase(allLazerFacing.begin() + i);
					allLazerTimer.erase(allLazerTimer.begin() + i);
				}
			}
			else if (allLazerFacing[i] == 1)
			{
				if (checkForEmptySpaceLazer(allLazerPosition[i].X, allLazerPosition[i].Y + 1))
				{

					if (mapArray[allLazerPosition[i].X][allLazerPosition[i].Y + 1] == mapGen.enemy)
					{
						for (int j = 0; j < allEnemyPosition.size(); j++)
						{

							lazerCoord.X = allLazerPosition[i].X;
							lazerCoord.Y = allLazerPosition[i].Y + 1;

							if (lazerCoord.X == allEnemyPosition[j].X && lazerCoord.Y == allEnemyPosition[j].Y)
							{
								mapArray[allEnemyPosition[j].X][allEnemyPosition[j].Y] = mapGen.floors;
								mapArray[lazerCoord.X][lazerCoord.Y - 1] = mapGen.floors;
								allLazerPosition.erase(allLazerPosition.begin() + i);
								allLazerFacing.erase(allLazerFacing.begin() + i);
								allLazerTimer.erase(allLazerTimer.begin() + i);

								allEnemyPosition.erase(allEnemyPosition.begin() + j);
								allEnemytimer.erase(allEnemytimer.begin() + j);
							}
						}
						break;

					}

					mapArray[allLazerPosition[i].X][allLazerPosition[i].Y + 1] = mapGen.verticalLazer;

					if (mapArray[allLazerPosition[i].X][allLazerPosition[i].Y] != stair)
						mapArray[allLazerPosition[i].X][allLazerPosition[i].Y] = floors;

					lazerCoord.X = allLazerPosition[i].X;
					lazerCoord.Y = allLazerPosition[i].Y + 1;
					coord.X = lazerCoord.X;
					coord.Y = lazerCoord.Y;
					allLazerPosition[i] = coord;
					allLazerTimer[i] = elapsedTimer;

				}
				else
				{
					mapArray[allLazerPosition[i].X][allLazerPosition[i].Y] = mapGen.floors;
					allLazerPosition.erase(allLazerPosition.begin() + i);
					allLazerFacing.erase(allLazerFacing.begin() + i);
					allLazerTimer.erase(allLazerTimer.begin() + i);
				}
			}
			else if (allLazerFacing[i] == 2)
			{
				if (checkForEmptySpaceLazer(allLazerPosition[i].X - 1, allLazerPosition[i].Y))
				{

					if (mapArray[allLazerPosition[i].X - 1][allLazerPosition[i].Y] == mapGen.enemy)
					{
						for (int j = 0; j < allEnemyPosition.size(); j++)
						{

							lazerCoord.X = allLazerPosition[i].X - 1;
							lazerCoord.Y = allLazerPosition[i].Y;

							if (lazerCoord.X == allEnemyPosition[j].X && lazerCoord.Y == allEnemyPosition[j].Y)
							{
								mapArray[allEnemyPosition[j].X][allEnemyPosition[j].Y] = mapGen.floors;
								mapArray[lazerCoord.X + 1][lazerCoord.Y] = mapGen.floors;
								allLazerPosition.erase(allLazerPosition.begin() + i);
								allLazerFacing.erase(allLazerFacing.begin() + i);
								allLazerTimer.erase(allLazerTimer.begin() + i);

								allEnemyPosition.erase(allEnemyPosition.begin() + j);
								allEnemytimer.erase(allEnemytimer.begin() + j);
							}
						}
						break;
					}

					mapArray[allLazerPosition[i].X - 1][allLazerPosition[i].Y] = mapGen.horizontalLazer;

					if (mapArray[allLazerPosition[i].X][allLazerPosition[i].Y] != stair)
						mapArray[allLazerPosition[i].X][allLazerPosition[i].Y] = floors;

					lazerCoord.X = allLazerPosition[i].X - 1;
					lazerCoord.Y = allLazerPosition[i].Y;
					coord.X = lazerCoord.X;
					coord.Y = lazerCoord.Y;
					allLazerPosition[i] = coord;
					allLazerTimer[i] = elapsedTimer;

				}
				else
				{
					mapArray[allLazerPosition[i].X][allLazerPosition[i].Y] = mapGen.floors;
					allLazerPosition.erase(allLazerPosition.begin() + i);
					allLazerFacing.erase(allLazerFacing.begin() + i);
					allLazerTimer.erase(allLazerTimer.begin() + i);
				}
			}
			else if (allLazerFacing[i] == 3)
			{
				if (checkForEmptySpaceLazer(allLazerPosition[i].X + 1, allLazerPosition[i].Y))
				{
					if (mapArray[allLazerPosition[i].X + 1][allLazerPosition[i].Y] == mapGen.enemy)
					{
						for (int j = 0; j < allEnemyPosition.size(); j++)
						{

							lazerCoord.X = allLazerPosition[i].X + 1;
							lazerCoord.Y = allLazerPosition[i].Y;

							if (lazerCoord.X == allEnemyPosition[j].X && lazerCoord.Y == allEnemyPosition[j].Y)
							{
								mapArray[allEnemyPosition[j].X][allEnemyPosition[j].Y] = mapGen.floors;
								mapArray[lazerCoord.X - 1][lazerCoord.Y] = mapGen.floors;
								allLazerPosition.erase(allLazerPosition.begin() + i);
								allLazerFacing.erase(allLazerFacing.begin() + i);
								allLazerTimer.erase(allLazerTimer.begin() + i);

								allEnemyPosition.erase(allEnemyPosition.begin() + j);
								allEnemytimer.erase(allEnemytimer.begin() + j);
							}
						}
						break;
					}

					mapArray[allLazerPosition[i].X + 1][allLazerPosition[i].Y] = mapGen.horizontalLazer;

					if (mapArray[allLazerPosition[i].X][allLazerPosition[i].Y] != stair)
						mapArray[allLazerPosition[i].X][allLazerPosition[i].Y] = floors;

					lazerCoord.X = allLazerPosition[i].X + 1;
					lazerCoord.Y = allLazerPosition[i].Y;
					coord.X = lazerCoord.X;
					coord.Y = lazerCoord.Y;
					allLazerPosition[i] = coord;
					allLazerTimer[i] = elapsedTimer;
				}
				else
				{
					mapArray[allLazerPosition[i].X][allLazerPosition[i].Y] = mapGen.floors;
					allLazerPosition.erase(allLazerPosition.begin() + i);
					allLazerFacing.erase(allLazerFacing.begin() + i);
					allLazerTimer.erase(allLazerTimer.begin() + i);
				}
			}
		}
	}
}

void MapGenerator::spawnLazer(short facing, int playerX, int playerY, double elapsedTimer)
{
	COORD lazerCoord;
	lazerCoord.X = playerX;
	lazerCoord.Y = playerY;

	allLazerPosition.push_back(lazerCoord);
	allLazerFacing.push_back(facing);
	allLazerTimer.push_back(elapsedTimer);

	mapArray[lazerCoord.X][lazerCoord.Y] = ((facing > 1) ? mapGen.horizontalLazer : mapGen.verticalLazer);
}

//----------------CHECK FOR EMPTY SPACES(FOR AI MOVEMENTS)----------------
bool MapGenerator::checkForEmptySpace(short x, short y)
{
	if (mapArray[x][y] != walls &&
		mapArray[x][y] != stair &&
		mapArray[x][y] != enemy)
		return true;

	return false;
}

//----------------CHECK FOR EMPTY SPACES(FOR AI MOVEMENTS)----------------
bool MapGenerator::checkForEmptySpaceLazer(short x, short y)
{
	if (mapArray[x][y] != walls &&
		mapArray[x][y] != stair)
		return true;

	return false;
}

void MapGenerator::resetLazer()
{
	if (allLazerPosition.size() != 0)
		allLazerPosition.clear();

	if (allLazerFacing.size() != 0)
		allLazerFacing.clear();

	if (allLazerTimer.size() != 0)
		allLazerTimer.clear();
}

void MapGenerator::playerVision(int elapsedTimer, int PlayerX, int PlayerY)
{
	COORD c;
	//---------------------THE 3 X 3 VISION---------------------
	if (curse.getActiveCurseShort() == 3 || curse.getActiveCurse2Short() == 3)
	{
		//---------------------CHECK FOR DARKNESS CURSE, DISPLAY HALF THE TIME---------------------
		if (elapsedTimer % 2 == 0)
		{
			for (int i = PlayerY + 1; i >= PlayerY - 1; i--)
			{
				c.Y = i;
				for (int j = PlayerX + 2; j >= PlayerX - 2; j--)
				{
					c.X = j;
					//----------------DISPLAYING FLOORS, END GOAL(STAIR), WALLS, ENEMIES----------------
					if (mapGen.getArrayCharacter(j, i) == mapGen.floors)
						g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i), mapGen.floorColor);

					else if (mapGen.getArrayCharacter(j, i) == mapGen.stair)
						g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i));

					else if (mapGen.getArrayCharacter(j, i) == mapGen.walls)
						g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i), mapGen.wallColor);

					else if (mapGen.getArrayCharacter(j, i) == mapGen.enemy)
						g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i), mapGen.enemyColor);

					else if (mapGen.getArrayCharacter(j, i) == mapGen.bombDrop || mapGen.getArrayCharacter(j, i) == mapGen.ammoDrop)
						g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i), mapGen.itemDropColor);
				}
			}
		}
	}
	else
	{
		for (int i = PlayerY + 1; i >= PlayerY - 1; i--)
		{
			c.Y = i;
			for (int j = PlayerX + 2; j >= PlayerX - 2; j--)
			{
				c.X = j;
				//----------------DISPLAYING FLOORS, END GOAL(STAIR), WALLS, ENEMIES----------------
				if (mapGen.getArrayCharacter(j, i) == mapGen.floors)
					g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i), mapGen.floorColor);

				else if (mapGen.getArrayCharacter(j, i) == mapGen.stair)
					g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i));

				else if (mapGen.getArrayCharacter(j, i) == mapGen.walls)
					g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i), mapGen.wallColor);

				else if (mapGen.getArrayCharacter(j, i) == mapGen.enemy)
					g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i), mapGen.enemyColor);

				else if (mapGen.getArrayCharacter(j, i) == mapGen.bombDrop || mapGen.getArrayCharacter(j, i) == mapGen.ammoDrop)
					g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i), mapGen.itemDropColor);
			}
		}
	}
}

//----------------RENDERING TORCH LIT UP AREA----------------
void MapGenerator::torchView()
{
	if (inven.torchLocation.size() > 0)
	{
		for (int i = 0; i < inven.torchLocation.size(); i++)
		{
			COORD torchCoord;
			for (int y = inven.torchLocation[i].Y - 2; y <= inven.torchLocation[i].Y + 2; y++)
			{
				torchCoord.Y = y;
				for (int x = inven.torchLocation[i].X - 4; x <= inven.torchLocation[i].X + 4; x++)
				{
					torchCoord.X = x;
					//----------------DISPLAYING FLOORS, END GOAL(STAIR), WALLS, ENEMIES----------------
					if (mapGen.getArrayCharacter(x, y) == mapGen.floors)
						g_Console.writeToBuffer(torchCoord, mapGen.getArrayCharacter(x, y), mapGen.floorColor);

					else if (mapGen.getArrayCharacter(x, y) == mapGen.stair)
						g_Console.writeToBuffer(torchCoord, mapGen.getArrayCharacter(x, y));

					else if (mapGen.getArrayCharacter(x, y) == mapGen.walls)
						g_Console.writeToBuffer(torchCoord, mapGen.getArrayCharacter(x, y), mapGen.wallColor);

					else if (mapGen.getArrayCharacter(x, y) == mapGen.enemy)
						g_Console.writeToBuffer(torchCoord, mapGen.getArrayCharacter(x, y), mapGen.enemyColor);
				}
			}
		}
	}
}