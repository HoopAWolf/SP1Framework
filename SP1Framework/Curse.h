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
		short activeCurse = 0;
	} CurseBase;


public:
	//---------------------RETURNS ACTIVE CURSE NUMBER---------------------
	short getActiveCurseShort();
	//---------------------RETURNS ACTIVE CURSE STRING---------------------
	string getActiveCurseString();

	//---------------------START RANDOM CURSE---------------------
	void startARandomCurse();
	//---------------------RESET CURSE---------------------
	void resetCurse();
};

#endif