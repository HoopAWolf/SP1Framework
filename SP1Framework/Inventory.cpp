#include "Inventory.h"
#include "MapGenerator.h"

MapGenerator mapGen;


string Inventory::getInventoryItem(int i)
{
	return InventoryStorage.InventorySlot[i];
}


void Inventory::setInventoryBombCount(short i)
{
	InventoryStorage.bombCount = i;
}

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
		break;
	}
}

void Inventory::useExplosion(short charPosX, short charPosY)
{
	for (int i = charPosY - InventoryStorage.bombRadius; i <= charPosY + InventoryStorage.bombRadius; i++)
	{
		for (int j = (charPosX - InventoryStorage.bombRadius) - 2; j <= (charPosX + InventoryStorage.bombRadius) + 2; j++)
		{
			if (i != 0 && i != 23 && j != 0 && j != 43)
			{
				if (mapGen.getArrayCharacter(j, i) == mapGen.walls)
					mapGen.replaceMapCharacterXY(j, i, mapGen.floors);
			}
		}
	}

}

void Inventory::clearInventory()
{
	for(int i = 0; i < 4; i ++)
		InventoryStorage.InventorySlot.push_back("-");
}

void Inventory::setInventory(bool setRickAxe, bool setBomb)
{
	if(setRickAxe)
		InventoryStorage.InventorySlot[0] = " - RickAxe [A]";

	if (setBomb)
		InventoryStorage.InventorySlot[1] = " - MindBomb [S]";

}

int Inventory::getInventorySize()
{
	return 	InventoryStorage.InventorySlot.size();
}