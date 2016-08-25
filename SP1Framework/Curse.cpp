#include "Curse.h"
#include "Entity.h"
#include "MapGenerator.h"

Curse curse;
extern Entity entityBase;
extern MapGenerator mapGen;
string curseArray[] =  { "No Curse", "Curse Of Time", "Curse Of Bleed", "Curse Of Darkness", "Curse Of Confusion", "Curse Of Butter Finger" };

//---------------------RETURN ACTIVE CURSE NUMBER---------------------
short Curse::getActiveCurseShort()
{
	return CurseBase.activeCurse;
}

//---------------------RETURN ACTIVE CURSE NUMBER---------------------
short Curse::getActiveCurse2Short()
{
	return CurseBase.activeCurse2;
}

//---------------------RETURN ACTIVE CURSE STRING---------------------
string Curse::getActiveCurseString()
{
	if (CurseBase.allCurse.size() <= 0)
	{
		for (int i = 0; i < 7; i++)
			CurseBase.allCurse.push_back(curseArray[i]);
	}

	return CurseBase.allCurse[CurseBase.activeCurse];
}

//---------------------RETURN ACTIVE CURSE2 STRING---------------------
string Curse::getActiveCurse2String()
{
	if (CurseBase.allCurse.size() <= 0)
	{
		for (int i = 0; i < 7; i++)
			CurseBase.allCurse.push_back(curseArray[i]);
	}

	return CurseBase.allCurse[CurseBase.activeCurse2];
}

//---------------------START RANDOM CURSE---------------------
void Curse::startARandomCurse()
{
	srand(time(NULL));
	short i = rand() % 6;

	CurseBase.activeCurse = i;
	if (mapGen.getFloorLevel() > 25)
	{
		i = rand() % 6;
		CurseBase.activeCurse2 = i;
	}

}

//---------------------RESET TO NO CURSE---------------------
void Curse::resetCurse()
{
	CurseBase.activeCurse = 0;
	CurseBase.activeCurse2 = 0;
}

//---------------------BLEED CURSE---------------------
void Curse::bleedCurse()
{
	entityBase.damagePlayer(1);
}