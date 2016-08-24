#ifndef _INVENTORY_H_
#define _INVENTORY_H_

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

		//---------------------CONTAINS COOLDOWNS, BOMB RADIUS, AMMOUNT OF ITEMS---------------------
		short rickaxeTimer, bombCount = 0,
			bombRadius = 2, pickAxeLength = 1, bombCountDown = 0, 
			bombCoolDown = 0, rickAxeCoolDown = 0, laserRifleCoolDown = 0,
			shotsLeft = 0, torchCount = 0;


	} InventoryStorage;

public: 


	//---------------------CONTAINS TORCH LOCATION---------------------
	vector<COORD> torchLocation;

	//----------------MINE WALL WITH RICKAXE----------------
	void mineWall(short i, short charPosX, short charPosY);

	//----------------SET EXPLOSION----------------
	void useExplosion(short bombPosX, short bombPosY);

	//----------------CLEAR ALL INVENTORY----------------
	void clearInventory();

	//----------------SET INVENTORY ITEM----------------
	void setInventory(bool setRickAxe, bool setBomb, bool setRifle, bool setTorch);

	//----------------GET SPECIFIC ITEM: 1 - RICKAXE, 2 - BOMB, 3 - NOTHING YET, 4 - NOTHING YET----------------
	string getInventoryItem(int i);

	//----------------GET INVEONTORY SIZE----------------
	int getInventorySize();

	

	//------------RETURNS BOMB QUANTITY------------
	short getBombCount();
	//------------DECREASE BOMB QUANTITY BY 1------------
	void decreaseBombCount();


	//-----------RESET EXPLOSION COUNTDOWN TO 3------------
	void resetBombCountDown();
	//------------RETURN EXPLOSION COUNT DOWN TIME------------
	short getBombCountDown();
	//------------DECREASE EXPLOSION COUNTDOWN BY 1------------
	void decreaseBombCountDown();


	//------------DECREASE RICKAXE COOLDOWN------------
	void decreaseRickAxeCoolDown();
	//------------DECREASE LASER RIFLE COOLDOWN------------
	void decreaseLaserRifleCoolDown();
	//------------DECREASE BOMB COOLDOWN------------
	void decreaseBombCoolDown();


	//------------RETURNS LASER RIFLE COOLDOWN------------
	short getLaserRifleCoolDown();
	//------------RETURNS RICKAXE COOLDOWN------------
	short getRickAxeCoolDown();
	//------------RETURNS BOMB COOLDOWN------------
	short getBombCoolDown();


	//------------START BOMB COOLDOWN------------
	void startBombCoolDown();
	//------------START RICKAXE COOLDOWN------------
	void startRickAxeCoolDown();
	//------------START LASER RIFLE COOLDOWN------------
	void startLaserRifleCoolDown();



	//------------RETURNS TORCH QUANTITY------------
	short getTorchCount();
	//------------RETURNS AMMO QUANTITY------------
	short getLaserRifleAmmoCount();


	//------------RESET BOMB COOLDOWN------------
	void resetBombCoolDown();
	//------------RESET RICKAXE COOLDOWN------------
	void resetRickAxeCoolDown();
	//------------RESET LAZER RIFLE COOLDOWN------------
	void resetLaserRifleCoolDown();



	//------------SET LAZER RIFLE AMMO------------
	void setInventoryLaserRifleAmmountCount(short ammount);
	//------------SET TORCH AMMO------------
	void setInventoryTorchCount(short ammount);
	//----------------SET BOMB COUNT----------------
	void setInventoryBombCount(short i);



	//---------------------DECREASE TORCH COUNT---------------------
	void decreaseTorchCount();
	//---------------------DECREASE LASER RIFLE AMMO---------------------
	void decreaseLaserRifleAmmoCount();
};

#endif