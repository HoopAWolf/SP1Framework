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
	struct EnemyBase
	{
		//----------------ENEMY HEALTH----------------
		int Enemyhealth;
		//----------------ENEMY SPEED----------------
		int Enemyspeed;

	} EnemyBase;

	struct PlayerBase
	{
		//----------------PLAYER HEALTH----------------
		int Playerhealth = 20;
		//----------------PLAYER SPEED----------------
		int Playerspeed = 1;

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

	//----------------GET ENEMY SPEED----------------
	int getEnemySpeed();
	//----------------GET PLAYER SPEED----------------
	int getPlayerSpeed();

	//----------------DAMAGE PLAYER----------------
	void damagePlayer(short i);
};


#endif