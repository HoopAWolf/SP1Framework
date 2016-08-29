#include "Entity.h"
#include "MapGenerator.h"

Entity entityBase;

COORD enemyCoord;

//----------------START STATE OF ENTITIES----------------
void Entity::startEntityEnemy()
{
	entityBase.EnemyBase.Enemyhealth = 3;

}

void Entity::startEntityPlayer()
{
	entityBase.PlayerBase.Playerhealth = 20;

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

//----------------DAMAGE PLAYER----------------
void Entity::damagePlayer(short damage)
{
	//FEED BACK
	entityBase.PlayerBase.Playerhealth -= damage;
}