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

		short rickaxeTimer, bombCount = 0,
			bombRadius = 2, pickAxeLength = 1, bombCountDown = 0, bombCoolDown = 0, rickAxeCoolDown = 0,
			shotsLeft = 0;


	} InventoryStorage;

public: 

	//----------------SET BOMB COUNT----------------
	void setInventoryBombCount(short i);
	//------------RETURNS BOMB QUANTITY------------
	short getBombCount();


	//----------------MINE WALL WITH RICKAXE----------------
	void mineWall(short i, short charPosX, short charPosY);

	//----------------SET EXPLOSION----------------
	void useExplosion(short bombPosX, short bombPosY);

	//----------------CLEAR ALL INVENTORY----------------
	void clearInventory();

	//----------------SET INVENTORY ITEM----------------
	void setInventory(bool setRickAxe, bool setBomb, bool setRifle);

	//----------------GET SPECIFIC ITEM: 1 - RICKAXE, 2 - BOMB, 3 - NOTHING YET, 4 - NOTHING YET----------------
	string getInventoryItem(int i);

	//----------------GET INVEONTORY SIZE----------------
	int getInventorySize();


	//------------DECREASE BOMB QUANTITY BY 1------------
	void decreaseBombCount();


	//-----------RESET EXPLOSION COUNTDOWN TO 3------------
	void resetBombCountDown();
	//------------RETURN EXPLOSION COUNT DOWN TIME------------
	short getBombCountDown();
	//------------DECREASE EXPLOSION COUNTDOWN BY 1------------
	void decreaseBombCountDown();


	//------------DECREASE BOMB COOLDOWN------------
	void decreaseBombCoolDown();
	//------------RETURNS BOMB COOLDOWN------------
	short getBombCoolDown();


	//------------DECREASE RICKAXE COOLDOWN------------
	void decreaseRickAxeCoolDown();
	//------------RETURNS RICKAXE COOLDOWN------------
	short getRickAxeCoolDown();


	//------------START BOMB COOLDOWN------------
	void startBombCoolDown();
	//------------START RICKAXE COOLDOWN------------
	void startRickAxeCoolDown();

	//------------RESET BOMB COOLDOWN------------
	void resetBombCoolDown();
	//------------RESET RICKAXE COOLDOWN------------
	void resetRickAxeCoolDown();
};
