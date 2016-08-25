#ifndef _CURSE_H_
#define _CURSE_H_

#include "game.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

using std::string;
using std::vector;

class Curse {

private: 
	struct CurseBase
	{
		//---------------------STORES CURSES---------------------
		vector<string> allCurse;
		//---------------------CURRENT ACTIVE CURSE---------------------
		short activeCurse = 0, activeCurse2;
	} CurseBase;


public:
	//---------------------RETURNS ACTIVE CURSE NUMBER---------------------
	short getActiveCurseShort();
	//---------------------RETURN ACTIVE CURSE 2  NUMBER---------------------
	short getActiveCurse2Short();
	//---------------------RETURNS ACTIVE CURSE STRING---------------------
	string getActiveCurseString();
	//---------------------RETURNS ACTIVE CURSE STRING---------------------
	string getActiveCurse2String();

	//---------------------START RANDOM CURSE---------------------
	void startARandomCurse();
	//---------------------RESET CURSE---------------------
	void resetCurse();
	//---------------------BLEED CURSE---------------------
	void bleedCurse();
};

#endif