#include "Inventory.h"
#include "MapGenerator.h"
#include "Entity.h"

Inventory inven;
extern MapGenerator mapGen;
extern Entity entityBase;

//------------GET THE ITEM FROM INVENTORY SLOT------------
string Inventory::getInventoryItem(int i)
{
	//------------ONLY 4 SLOT------------
	if(i < 4)
		return InventoryStorage.InventorySlot[i];

	return "-";
}

void Inventory::resetInventory()
{
	InventoryStorage.bombCount = 0;
	InventoryStorage.shotsLeft = 0;
	InventoryStorage.torchCount = 0;
}

//------------SET THE BOMB QUANTITY------------
void Inventory::setInventoryBombCount(short i)
{
	InventoryStorage.bombCount = i;
}

//------------SET THE LASER RIFLE AMMO QUANTITY------------
void Inventory::setInventoryLaserRifleAmmountCount(short i)
{
	InventoryStorage.shotsLeft = i;
}

//------------SET THE TORCH QUANTITY------------
void Inventory::setInventoryTorchCount(short i)
{
	InventoryStorage.torchCount = i;
}

//------------MINE WALLS TAKING DIRECTION AND POSITION AS PARAMETER------------
void Inventory::mineWall(short i, short charPosX, short charPosY)
{
	switch (i) {
		//------------UP------------
	case 0:
		if (mapGen.getArrayCharacter(charPosX, charPosY - 1) == mapGen.walls && charPosY - 1 != 0)
			mapGen.replaceMapCharacterXY(charPosX, charPosY - 1, mapGen.floors);
		break;
		//------------DOWN------------
	case 1:
		if (mapGen.getArrayCharacter(charPosX, charPosY + 1) == mapGen.walls  && charPosY + 1 != 23)
			mapGen.replaceMapCharacterXY(charPosX, charPosY + 1, mapGen.floors);
		break;
		//------------LEFT------------
	case 2:
		if (mapGen.getArrayCharacter(charPosX - 1, charPosY) == mapGen.walls  && charPosX - 1 != 0)
			mapGen.replaceMapCharacterXY(charPosX - 1, charPosY, mapGen.floors);
		break;
		//------------RIGHT------------
	case 3:
		if (mapGen.getArrayCharacter(charPosX + 1, charPosY) == mapGen.walls && charPosX + 1 != 43)
			mapGen.replaceMapCharacterXY(charPosX + 1, charPosY, mapGen.floors);
		break;
	default:
		//------------DEFAULT IN CASE THINGS GOES WRONG------------
		break;
	}
}

//------------EXPLODE FROM THE POSITION OF BOMB------------
void Inventory::useExplosion(short bombPosX, short bombPosY, int playerX, int playerY)
{
	//------------BOMB RADIUS Y------------
	for (int i = bombPosY - InventoryStorage.bombRadius; i <= bombPosY + InventoryStorage.bombRadius; i++)
	{
		//------------BOMB RADIUS X------------
		for (int j = (bombPosX - InventoryStorage.bombRadius) - 2; j <= (bombPosX + InventoryStorage.bombRadius) + 2; j++)
		{
			if (i != 0 && i != 23 && j != 0 && j != 43)
			{
				//------------BOOM------------
				if (mapGen.getArrayCharacter(j, i) == mapGen.walls)
					mapGen.replaceMapCharacterXY(j, i, mapGen.floors);

				else if (mapGen.getArrayCharacter(j, i) == mapGen.enemy)
				{
					mapGen.replaceMapCharacterXY(j, i, mapGen.floors);

					for (int k = 0; k < mapGen.allEnemyPosition.size(); k++)
					{
						if (mapGen.allEnemyPosition[k].X == j && mapGen.allEnemyPosition[k].Y == i)
						{
							mapGen.replaceMapCharacterXY(mapGen.allEnemyPosition[k].X, mapGen.allEnemyPosition[k].Y, mapGen.enemyGrave);
							mapGen.allEnemyPosition.erase(mapGen.allEnemyPosition.begin() + k);
							mapGen.allEnemytimer.erase(mapGen.allEnemytimer.begin() + k);
						}
					}
				}

				if (j == playerX && i == playerY)
					entityBase.damagePlayer(3);
			}
		}
	}

	mapGen.bombCoord.X = 0;
	mapGen.bombCoord.Y = 0;
}

//------------CLEAR INVENTORY BY RESETING ALL SLOTS TO "-" CHARACTER------------
void Inventory::clearInventory()
{
	InventoryStorage.InventorySlot.clear();
	for(int i = 0; i < 4; i ++)
		InventoryStorage.InventorySlot.push_back("-");
}

//------------SET INVENTORY------------
void Inventory::setInventory(bool setRickAxe, bool setBomb, bool setRifle, bool setTorch)
{

	std::ostringstream str, str2, str3;

	if (getInventorySize() > 0)
	{
		if (setRickAxe)
			InventoryStorage.InventorySlot[0] = " - Motivational Wall Breaker [Press Q]";

		if (setBomb)
		{
			str << " - Binary Bomb [Press W] Ammount Left: ";

			//------------SET THE COUNT OF BOMBS------------
			str << InventoryStorage.bombCount;
			InventoryStorage.InventorySlot[1] = str.str();
		}

		if (setRifle)
		{
			str2 << " - Recursive Rifle [Press E] Shots Left: ";
			//------------SET THE COUNT OF AMMO------------
			str2 << InventoryStorage.shotsLeft;
			InventoryStorage.InventorySlot[2] = str2.str();
		}

		if (setTorch)
		{
			str3 << " - Debugger Torch [Press R] Ammount Left: ";
			//------------SET THE COUNT OF TORCH------------
			str3 << InventoryStorage.torchCount;
			InventoryStorage.InventorySlot[3] = str3.str();
		}
	}
}


//------------GET INVENTORY SIZE------------
int Inventory::getInventorySize()
{
	return 	InventoryStorage.InventorySlot.size();
}


//------------DECREASE BOMB QUANTITY BY 1------------
void Inventory::decreaseBombCount()
{
	InventoryStorage.bombCount--;
}

//-----------RESET EXPLOSION COUNTDOWN TO 4------------
void Inventory::resetBombCountDown()
{
	InventoryStorage.bombCountDown = 4;
}

//------------DECREASE EXPLOSION COUNTDOWN BY 1------------
void Inventory::decreaseBombCountDown()
{
	 InventoryStorage.bombCountDown--;
}

//------------RETURN EXPLOSION COUNT DOWN TIME------------
short Inventory::getBombCountDown()
{
	return InventoryStorage.bombCountDown;
}


//------------DECREASES BOMB COOLDOWN BY 1------------
void Inventory::decreaseBombCoolDown()
{
	InventoryStorage.bombCoolDown--;
}

//------------DECREASE RICKAXE COOLDOWN BY 1------------
void Inventory::decreaseRickAxeCoolDown()
{
	InventoryStorage.rickAxeCoolDown--;
}

//------------DECREASE LASER RIFLE COOLDOWN BY 1------------
void Inventory::decreaseLaserRifleCoolDown()
{
	InventoryStorage.laserRifleCoolDown--;
}


//------------START BOMB COOLDOWN------------
void Inventory::startBombCoolDown()
{
	InventoryStorage.bombCoolDown = 15;
}

//------------START RICKAXE COOLDOWN------------
void Inventory::startRickAxeCoolDown()
{
	InventoryStorage.rickAxeCoolDown = 5;
}

//------------START LASER RIFLE COOLDOWN------------
void Inventory::startLaserRifleCoolDown()
{
	InventoryStorage.laserRifleCoolDown = 1;
}



//------------RETURNS BOMB COOLDOWN------------
short Inventory::getBombCoolDown()
{
	return InventoryStorage.bombCoolDown;
}

//------------RETURNS RICKAXE COOLDOWN------------
short Inventory::getRickAxeCoolDown()
{
	return InventoryStorage.rickAxeCoolDown;
}

//------------RETURNS LASER RIFLE COOLDOWN------------
short Inventory::getLaserRifleCoolDown()
{
	return InventoryStorage.laserRifleCoolDown;
}



//------------RESET BOMB COOLDOWN------------
void Inventory::resetBombCoolDown()
{
	InventoryStorage.bombCoolDown = 0;
}

//------------RESET RICKAXE COOLDOWN------------
void Inventory::resetRickAxeCoolDown()
{
	InventoryStorage.rickAxeCoolDown = 0;
}

//------------RESET LASER RIFLE COOLDOWN------------
void Inventory::resetLaserRifleCoolDown()
{
	InventoryStorage.laserRifleCoolDown = 0;
}



//------------RETURNS TORCH QUANTITY------------
short Inventory::getTorchCount()
{
	return InventoryStorage.torchCount;
}

//------------RETURNS LASER RIFLE AMMO QUANTITY------------
short Inventory::getLaserRifleAmmoCount()
{
	return InventoryStorage.shotsLeft;
}

//------------RETURNS BOMB QUANTITY------------
short Inventory::getBombCount()
{
	return InventoryStorage.bombCount;
}


//------------DECREASE TORCH QUANTITY BY 1------------
void Inventory::decreaseTorchCount()
{
	InventoryStorage.torchCount--;
}

//------------DECREASE LASER RIFLE AMMO QUANTITY BY 1------------
void Inventory::decreaseLaserRifleAmmoCount()
{
	InventoryStorage.shotsLeft--;
}
