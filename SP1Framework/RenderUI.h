#ifndef _RENDERUI_H_
#define _RENDERUI_H_

#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>


//----------------RENDER INVENTORY----------------
void renderInventory();
//------------RENDER GUIDE LINE TO HELP PLAYER------------
void renderHelp();
//----------------RENDER HEALTH BAR----------------
void renderHealthbar();
//---------------------RENDER FLOOR NUMBER ONTO SCREEN---------------------
void renderFloorNumber();
//----------------RENDER GAME OVER----------------
void renderGameOver();
//----------------RENDER INSTRUCTION----------------
void renderInstructions();

#endif