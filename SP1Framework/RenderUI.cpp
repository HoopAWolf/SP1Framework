#include "RenderUI.h"
#include "Curse.h"
#include "Inventory.h"
#include "MapGenerator.h"
#include "Entity.h"
#include "HighScore.h"

extern Console g_Console;
extern Inventory inven;
extern Curse curse;
extern MapGenerator mapGen;
extern Entity entityBase;


//----------------RENDER INVENTORY----------------
void renderInventory()
{
	COORD c;

	c.Y = 14;
	c.X = 50;

	g_Console.writeToBuffer(c, "Inventory: ", 0x0A/* green color */);
	c.Y++;

	for (int i = 0; i < inven.getInventorySize(); i++)
	{
		//------------RENDER WORDS RED IF COOLDOWN IS STILL ON------------
		if (i == 1 && inven.getBombCoolDown() > 0 ||
			i == 1 && curse.getActiveCurseShort() == 5 ||
			i == 0 && inven.getRickAxeCoolDown() > 0 ||
			i == 2 && inven.getLaserRifleCoolDown() > 0 ||
			i == 2 && curse.getActiveCurseShort() == 5 ||
			i == 3 && curse.getActiveCurseShort() == 5 || 
			i == 1 && curse.getActiveCurse2Short() == 5 || 
			i == 2 && curse.getActiveCurse2Short() == 5 ||
			i == 3 && curse.getActiveCurse2Short() == 5)
			g_Console.writeToBuffer(c, inven.getInventoryItem(i), 0x0C/* red color */);
		else
			g_Console.writeToBuffer(c, inven.getInventoryItem(i), 0x0A/* green color */);
		c.Y++;
	}
	g_Console.writeToBuffer(c, "<Press [Space] to use Echo Location>", 0x0D/* pink color */);

}

//------------RENDER GUIDE LINE TO HELP PLAYER------------
void renderHelp()
{
	COORD c;

	c.Y = 0;
	c.X = 86;
	g_Console.writeToBuffer(c, "Enemy: ", 0x02/* pink color */);
	c.X += 8;
	g_Console.writeToBuffer(c, mapGen.enemy, 0x0C/* red color */);

	c.Y += 1;
	c.X = 87;
	g_Console.writeToBuffer(c, "Bomb: ", 0x02/* pink color */);
	c.X += 7;
	g_Console.writeToBuffer(c, mapGen.bomb, 0x0C/* red color */);

	c.Y += 1;
	c.X = 82;
	g_Console.writeToBuffer(c, "Objective: ", 0x02/* pink color */);
	c.X += 12;
	g_Console.writeToBuffer(c, mapGen.stair);

	c.Y += 1;
	c.X = 82;
	g_Console.writeToBuffer(c, "Bomb Drop: ", 0x02/* pink color */);
	c.X += 12;
	g_Console.writeToBuffer(c, mapGen.bombDrop, 0x0D/* pink color */);

	c.Y += 1;
	c.X = 76;
	g_Console.writeToBuffer(c, "Rifle Ammo Drop: ", 0x02/* pink color */);
	c.X += 18;
	g_Console.writeToBuffer(c, mapGen.ammoDrop, 0x0D/* pink color */);
}

//----------------RENDER HEALTH BAR----------------
void renderHealthbar()
{
	COORD c;
	std::ostringstream HPcout, HPBar;
	c.Y = 8;
	c.X = 50;

	//---------------------PRINT HEALTH---------------------
	for (int i = 0; i < entityBase.getPlayerMaxHealth(); i++)
	{
		if (i == 0)
			HPBar << "[";

		if (i < entityBase.getPlayerHealth())
			HPBar << mapGen.walls; //------------PRINT BAR FOR HEALTH------------
		else
			HPBar << "-"; //------------PRINT '-' FOR MISSING HEALTH------------

		if (i == entityBase.getPlayerMaxHealth() - 1)
			HPBar << "]";
	}

	g_Console.writeToBuffer(c, HPBar.str(), 0x0C/* red color */);

	HPcout << "Health: " << entityBase.getPlayerHealth() << "/" << entityBase.getPlayerMaxHealth();

	c.X += 4;
	c.Y += 1;
	g_Console.writeToBuffer(c, HPcout.str(), 0x0A/* green color */);

}

//---------------------RENDER FLOOR NUMBER ONTO SCREEN---------------------
void renderFloorNumber()
{
	COORD c;
	std::ostringstream ss, ss2, ss3;
	c.X = (g_Console.getConsoleSize().X / 2) - 3;
	c.Y = 1;
	ss << mapGen.getFloorLevel() << " Floor";
	g_Console.writeToBuffer(c, ss.str());

	c.Y += 5;
	c.X += 10;
	ss2 << "[Curse Active: " << curse.getActiveCurseString() << "]";
	g_Console.writeToBuffer(c, ss2.str(), 0x0E);

	if (mapGen.getFloorLevel() > 25)
	{
		c.Y += 1;
		ss3 << "[Curse Active: " << curse.getActiveCurse2String() << "]";
		g_Console.writeToBuffer(c, ss3.str(), 0x0E);
	}
}

//----------------RENDER GAME OVER----------------
void renderGameOver(int endSelection)
{
	std::ostringstream ss, ss2, ss3;
	COORD c = g_Console.getConsoleSize();

	c.Y = 5;
	c.X = g_Console.getConsoleSize().X / 2 - 32;
	c.Y += 1;
	ss << "You lost on floor " << mapGen.getFloorLevel();
	g_Console.writeToBuffer(c, ss.str(), 0x0A);

	ss2 << "Highscore: Floor " << getHighScore();
	c.Y += 1;
	g_Console.writeToBuffer(c, ss2.str(), 0x0A);

	c.Y += 1;
	g_Console.writeToBuffer(c, "  ________                        ________                     ", 0x0A);
	c.Y += 1;
	g_Console.writeToBuffer(c, " /  _____/_____    _____   ____   \\_____  \\___  __ ___________ ", 0x0B);
	c.Y += 1;
	g_Console.writeToBuffer(c, "/   \\  ___\\__  \\  /     \\_/ __ \\   /   |   \\  \\/ // __ \\_  __ \\", 0x0C);
	c.Y += 1;
	g_Console.writeToBuffer(c, "\\    \\_\\  \\/ __ \\|  Y Y  \\  ___/  /    |    \\   /\\  ___/|  | \\/", 0x0D);
	c.Y += 1;
	g_Console.writeToBuffer(c, " \\______  (____  /__|_|  /\\___  > \\_______  /\\_/  \\___  >__|   ", 0x0E);
	c.Y += 1;
	g_Console.writeToBuffer(c, "        \\/     \\/      \\/     \\/          \\/          \\/       ", 0x0F);
	c.Y += 3;
	ss3 <<  "=========================" << ((entityBase.getPlayerHealth() <= 0) ? "YOU DIED OF HEALTH" : "YOU DIED FROM TIME") << "=========================";
	g_Console.writeToBuffer(c, ss3.str(), 0x0A/* green color */);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 7;
	g_Console.writeToBuffer(c, "Return to main menu", ((endSelection == 0) ? 0x1A : 0x0A));
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2;
	g_Console.writeToBuffer(c, "Quit", ((endSelection == 1) ? 0x1A : 0x0A));

}

//----------------RENDER INSTRUCTION----------------
void renderInstructions()
{
	COORD c;

	c.Y = 0;
	c.X = 5;
	g_Console.writeToBuffer(c, "DEEP INSTRUCTION MANUAL 101", 0x02/* green color */);

	c.Y += 1;
	g_Console.writeToBuffer(c, "[Press ESC or SPACE to return]", 0x02/* green color */);

	c.Y = 0;
	c.X = 86;
	g_Console.writeToBuffer(c, "Enemy: ", 0x02/* pink color */);
	c.X += 8;
	g_Console.writeToBuffer(c, mapGen.enemy, 0x0C/* red color */);

	c.Y += 1;
	c.X = 87;
	g_Console.writeToBuffer(c, "Bomb: ", 0x02/* pink color */);
	c.X += 7;
	g_Console.writeToBuffer(c, mapGen.bomb, 0x0C/* red color */);

	c.Y += 1;
	c.X = 82;
	g_Console.writeToBuffer(c, "Objective: ", 0x02/* pink color */);
	c.X += 12;
	g_Console.writeToBuffer(c, mapGen.stair);

	c.Y += 1;
	c.X = 82;
	g_Console.writeToBuffer(c, "Bomb Drop: ", 0x02/* pink color */);
	c.X += 12;
	g_Console.writeToBuffer(c, mapGen.bombDrop, 0x0D/* pink color */);

	c.Y += 1;
	c.X = 76;
	g_Console.writeToBuffer(c, "Rifle Ammo Drop: ", 0x02/* pink color */);
	c.X += 18;
	g_Console.writeToBuffer(c, mapGen.ammoDrop, 0x0D/* pink color */);

	c.X = (g_Console.getConsoleSize().X / 2) - 23;

	c.Y += 2;
	c.X += 8;
	g_Console.writeToBuffer(c, "Objective is to get to X", 0x0D/* pink color */);
	c.Y += 2;
	c.X += 7;
	g_Console.writeToBuffer(c, "[^] move up", 0x0D/* pink color */);
	c.Y += 1;
	c.X -= 14;
	g_Console.writeToBuffer(c, "move left [<] [V] [>] move right", 0x0D/* pink color */);
	c.Y += 1;
	c.X += 11;
	g_Console.writeToBuffer(c, "move down", 0x0D/* pink color */);

	c.Y += 2;
	c.X = 25;
	g_Console.writeToBuffer(c, "Curse Of Darkness: The vision of player blinks", 0x0E/* yellow color */);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Curse Of Time: Time counts down doubly fast", 0x0E/* yellow color */);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Curse Of Bleed: Health goes down overtime", 0x0E/* yellow color */);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Curse Of Confusion: Movement keys are opposite", 0x0E/* yellow color */);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Curse Of Butter Finger: Deny the player the ability to use some items", 0x0E/* yellow color */);
	c.Y += 1;
	g_Console.writeToBuffer(c, "(The items turn red)", 0x0E/* pink color */);

	c.Y += 1;
	g_Console.writeToBuffer(c, "MORE INTRUCTIONS IN GAME", 0x0E/* yellow color */);

	c.Y += 2;
	c.X += 2;
	g_Console.writeToBuffer(c, "Red hightlight on item means it is on cooldown", 0x0C/* red color */);

	c.Y += 2;
	c.X -= 8;
	g_Console.writeToBuffer(c, "Bomb does damage to player if player is in the radius of the blast", 0x0C/* red color */);
}