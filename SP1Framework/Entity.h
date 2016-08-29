#ifndef _ENTITY_H
#define _ENTITY_H

#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

class Entity 
{

private:

	struct PlayerBase
	{
		//----------------PLAYER HEALTH----------------
		int Playerhealth = 20;
		//----------------PLAYER SPEED----------------
		int Playerspeed = 1;
		//----------------PLAYER MAX HEALTH----------------
		int PlayerMaxHealth = 20;

	} PlayerBase;

public:

	//----------------START ENEMY----------------
	void startEntityEnemy();
	//----------------START PLAYER----------------
	void startEntityPlayer();

	//----------------CHECK ENEMY IS DEAD----------------
	bool isEnemyDead();
	//----------------CHECK PLAYER IS DEAD----------------
	bool isPlayerDead();

	//----------------GET ENEMY HEALTH----------------
	int getEnemyHealth();
	//----------------GET PLAYER HEALTH----------------
	int getPlayerHealth();
	//----------------GET PLAYER MAX HEALTH----------------
	int getPlayerMaxHealth();
	//----------------INCREASE PLAYER MAX HEALTH----------------
	void increasePlayerMaxHealth(short health);

	//----------------DAMAGE PLAYER----------------
	void damagePlayer(short i);

};


#endif