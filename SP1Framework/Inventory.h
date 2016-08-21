#include "game.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

class Inventory {

private:
	struct InventoryStorage {

		vector<string> InventorySlot;

		short rickaxeTimer, bombCount, 
			  bombRadius = 2, pickAxeLength = 1;


	} InventoryStorage;

public: 

	//----------------SET BOMB COUNT----------------
	void setInventoryBombCount(short i);

	//----------------MINE WALL WITH RICKAXE----------------
	void mineWall(short i, short charPosX, short charPosY);

	//----------------SET EXPLOSION----------------
	void useExplosion(short charPosX, short charPosY);

	//----------------CLEAR ALL INVENTORY----------------
	void clearInventory();

	//----------------SET INVENTORY ITEM----------------
	void setInventory(bool setRickAxe, bool setBomb);

	//----------------GET SPECIFIC ITEM: 1 - RICKAXE, 2 - BOMB, 3 - NOTHING YET, 4 - NOTHING YET----------------
	string getInventoryItem(int i);

	//----------------GET INVEONTORY SIZE----------------
	int getInventorySize();

};
