#include "Entity.h"
#include "MapGenerator.h"

Entity entityBase;

COORD enemyCoord;

//----------------START STATE OF ENTITIES----------------
void Entity::startEntityEnemy()
{
	entityBase.EnemyBase.Enemyhealth = 3;
	entityBase.EnemyBase.Enemyspeed = 1;

}

void Entity::startEntityPlayer()
{
	entityBase.PlayerBase.Playerhealth = 3;
	entityBase.PlayerBase.Playerspeed = 1;

}

//----------------GET STATE OF ENTITIES----------------
bool Entity::isEnemyDead()
{
	return ((entityBase.EnemyBase.Enemyhealth == 0) ? true : false);
}

bool Entity::isPlayerDead()
{
	return ((entityBase.PlayerBase.Playerhealth == 0) ? true : false);
}


//----------------GET HEALTH OF ENTITIES----------------
int Entity::getEnemyHealth()
{
	return entityBase.EnemyBase.Enemyhealth;
}

int Entity::getPlayerHealth()
{
	return entityBase.PlayerBase.Playerhealth;
}


//----------------GET SPEED OF ENTITIES----------------
int Entity::getEnemySpeed()
{
	return entityBase.EnemyBase.Enemyspeed;
}

int Entity::getPlayerSpeed()
{
	return entityBase.PlayerBase.Playerspeed;
}


//----------------DAMAGE PLAYER----------------
void Entity::damagePlayer(short damage)
{
	entityBase.PlayerBase.Playerhealth -= damage;
}