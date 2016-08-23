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
		vector<string> allCurse;
		short activeCurse = 0;
	} CurseBase;


public:
	short getActiveCurseShort();
	string getActiveCurseString();
	void startARandomCurse();
	
};

#endif