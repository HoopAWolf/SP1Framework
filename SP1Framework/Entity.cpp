#include "Entity.h"
#include "MapGenerator.h"

Entity entityBase;

COORD enemyCoord;

//----------------START STATE OF ENTITIES----------------
void Entity::startEntityPlayer()
{
	entityBase.PlayerBase.Playerhealth = 20;

	entityBase.PlayerBase.PlayerMaxHealth = 20;

}

bool Entity::isPlayerDead()
{
	return ((entityBase.PlayerBase.Playerhealth <= 0) ? true : false);
}

int Entity::getPlayerHealth()
{
	return entityBase.PlayerBase.Playerhealth;
}

int Entity::getPlayerMaxHealth()
{
	return entityBase.PlayerBase.PlayerMaxHealth;
}

//----------------DAMAGE PLAYER----------------
void Entity::damagePlayer(short damage)
{
	entityBase.PlayerBase.Playerhealth -= damage;
}

void Entity::increasePlayerMaxHealth(short health)
{
	entityBase.PlayerBase.PlayerMaxHealth += health;
}