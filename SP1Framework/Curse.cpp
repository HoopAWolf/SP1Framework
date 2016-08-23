#include "Curse.h"

Curse curse;
string curseArray[] =  { "No Curse", "Curse Of Time", "Curse Of Bleed", "Curse Of Darkness" };

short Curse::getActiveCurseShort()
{
	return CurseBase.activeCurse;
}

string Curse::getActiveCurseString()
{
	if (CurseBase.allCurse.size() <= 0)
	{
		for (int i = 0; i < 5; i++)
			CurseBase.allCurse.push_back(curseArray[i]);
	}

	return CurseBase.allCurse[CurseBase.activeCurse];
}

void Curse::startARandomCurse()
{
	srand(time(NULL));
	short i = rand() % 4;

	CurseBase.activeCurse = i;
}

