#include "Inventory.h"
#include "MapGenerator.h"

Inventory inven;
extern MapGenerator mapGen;

//------------GET THE ITEM FROM INVENTORY SLOT------------
string Inventory::getInventoryItem(int i)
{
	//------------ONLY 4 SLOT------------
	if(i < 4)
		return InventoryStorage.InventorySlot[i];

	return "-";
}

//------------SET THE BOMB QUANTITY------------
void Inventory::setInventoryBombCount(short i)
{
	InventoryStorage.bombCount = i;
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
void Inventory::useExplosion(short bombPosX, short bombPosY)
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
			}
		}
	}

}

//------------CLEAR INVENTORY BY RESETING ALL SLOTS TO "-" CHARACTER------------
void Inventory::clearInventory()
{
	InventoryStorage.InventorySlot.clear();
	for(int i = 0; i < 4; i ++)
		InventoryStorage.InventorySlot.push_back("-");
}

//------------SET INVENTORY------------
void Inventory::setInventory(bool setRickAxe, bool setBomb, bool setRifle)
{
	string str;

	if (getInventorySize() > 0)
	{
		if (setRickAxe)
			InventoryStorage.InventorySlot[0] = " - RickAxe [A]";

		if (setBomb)
		{
			str = " - Mind Bomb [S] x";

			//------------SET THE COUNT OF BOMBS------------
			str += ((char)InventoryStorage.bombCount + 48);
			InventoryStorage.InventorySlot[1] = str;
		}

		if (setRifle)
		{
			str =  " - Laser Rifle [D] Shots Left: ";
			//------------SET THE COUNT OF AMMO------------
			str += ((char)InventoryStorage.shotsLeft + 48);
			InventoryStorage.InventorySlot[2] = str;
		}
	}
}


//------------GET INVENTORY SIZE------------
int Inventory::getInventorySize()
{
	return 	InventoryStorage.InventorySlot.size();
}


//------------RETURNS BOMB QUANTITY------------
short Inventory::getBombCount()
{
	return InventoryStorage.bombCount;
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