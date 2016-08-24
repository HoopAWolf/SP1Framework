#include "Curse.h"

Curse curse;
string curseArray[] =  { "No Curse", "Curse Of Time", "Curse Of Bleed", "Curse Of Darkness" };

//---------------------RETURN ACTIVE CURSE NUMBER---------------------
short Curse::getActiveCurseShort()
{
	return CurseBase.activeCurse;
}

//---------------------RETURN ACTIVE CURSE STRING---------------------
string Curse::getActiveCurseString()
{
	if (CurseBase.allCurse.size() <= 0)
	{
		for (int i = 0; i < 5; i++)
			CurseBase.allCurse.push_back(curseArray[i]);
	}

	return CurseBase.allCurse[CurseBase.activeCurse];
}

//---------------------START RANDOM CURSE---------------------
void Curse::startARandomCurse()
{
	srand(time(NULL));
	short i = rand() % 4;

	CurseBase.activeCurse = i;
}

void Curse::resetCurse()
{
	CurseBase.activeCurse = 0;
}

