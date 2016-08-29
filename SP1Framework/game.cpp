// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "MapGenerator.h"
#include "Entity.h"
#include "Inventory.h"
#include "Curse.h"
#include <iostream>
#include <fstream>
#include "HighScore.h"
#include "RenderUI.h"

#include <windows.h>
#include "MMSystem.h"
#include "Windows.h"

//----------------THIS IS TO SET THE SIZE OF MAZE----------------
#define MAZEX 43
#define MAZEY 23


double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];
double timerAIAttack = 0.0, timerSelection = 0.0, timerHealEffect = 5.0;


//----------------SET BOOL VARIABLE FOR MAP RENDERING, ECHO LOCATION, COUNTDOWN----------------
bool useEchoLocation = false, countdownStarted = false, startGame = false;

//----------------SET GOAL X AND Y LOCATION, PLAYER X AND Y LOCATION, RADIUS FOR ECHO LOCATION----------------
int timer, playerLocationX, playerLocationY, radiusX, radiusY, facing,
timeAttack = 60, danger = 0,
timerDanger = 0, timerBomb = 0, timerRickAxe = 0, timerLaserRifle = 0, timerHealthEffect = 0,
selection = 0, endSelection = 0;

// MAP GENERATOR OBJECT
extern MapGenerator mapGen;
//ENTITY OPJECT
extern Entity entityBase;
//INVENTORY OPJECT
extern Inventory inven;
//CURSE OPJECT
extern Curse curse;

COORD c;
WORD color = 0x0F;

// Game specific variables here
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once

// Console object
Console g_Console(100, 24, "Deep Sleep - By Team Insomnia");



//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;
    g_dBounceTime = 0.0;

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;
    g_sChar.m_cLocation.X = g_Console.getConsoleSize().X - 60;
    g_sChar.m_cLocation.Y = (g_Console.getConsoleSize().Y / 2) + 5;
    g_sChar.m_bActive = true;
	useEchoLocation = false;
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 40, L"Consolas");

	PlaySound(TEXT("titleScreen.wav"), NULL, SND_SYNC | SND_LOOP | SND_ASYNC);
	waveOutSetVolume(NULL, 0x5555);

}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Getting all the key press states
//            This function checks if any key had been pressed since the last time we checked
//            If a key is pressed, the value for that particular key will be true
//
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{    
    g_abKeyPressed[K_UP]     = isKeyPressed(VK_UP);
    g_abKeyPressed[K_DOWN]   = isKeyPressed(VK_DOWN);
    g_abKeyPressed[K_LEFT]   = isKeyPressed(VK_LEFT);
    g_abKeyPressed[K_RIGHT]  = isKeyPressed(VK_RIGHT);
    g_abKeyPressed[K_SPACE]  = isKeyPressed(VK_SPACE);
    g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
	g_abKeyPressed[K_ENTER] = isKeyPressed(VK_RETURN);
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
		case S_INSTRUCTIONSCREEN: instructionScreen(); // game logic for the splash screen
			break;
        case S_GAME: gameplay(); // gameplay logic when we are in the game
            break;
		case S_GAMEOVER: gameOver(); // -----renders the game over stuff-----
			break;
    }
}
//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
        case S_SPLASHSCREEN: renderSplashScreen();
            break;
		case S_INSTRUCTIONSCREEN: renderInstructions();
			break;
        case S_GAME: renderGame();
            break;
		case S_GAMEOVER: renderGameOver(endSelection);
			break;
    }

    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{

	processUserInputSplashScreen();

	if (g_dElapsedTime > timerSelection + 0.01)
	{
		if (g_abKeyPressed[K_ENTER] && selection == 0)
		{ // PRESS ENTER TO START GAME
			g_eGameState = S_GAME;
			mapGen.hellMode = false;
			timerHealEffect = 5;

			PlaySound(TEXT("backgroundMusic.wav"), NULL, SND_SYNC | SND_LOOP | SND_ASYNC);
			waveOutSetVolume(NULL, 0x5555);
			timerSelection = g_dElapsedTime;
		}

		else if (g_abKeyPressed[K_ENTER] && selection == 1)
		{ // PRESS ENTER TO START GAME
			g_eGameState = S_GAME;
			mapGen.hellMode = true;
			timeAttack = 20;
			timerHealEffect = 6;

			PlaySound(TEXT("backgroundMusic.wav"), NULL, SND_SYNC | SND_LOOP | SND_ASYNC);
			waveOutSetVolume(NULL, 0x5555);
			timerSelection = g_dElapsedTime;
		}

		else if (g_abKeyPressed[K_ENTER] && selection == 2)
		{
			g_eGameState = S_INSTRUCTIONSCREEN;
			timerSelection = g_dElapsedTime;
		}

		else if (g_abKeyPressed[K_ENTER] && selection == 3)
		{
			g_bQuitGame = true;
			timerSelection = g_dElapsedTime;
		}

		
	}
}

void gameplay()            // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
}

void gameOver()
{
	processUserInputEndScreen();
}

void moveCharacter()
{
    bool bSomethingHappened = false;
	if (curse.getActiveCurseShort() == 4 || curse.getActiveCurse2Short() == 4)
	{
		if (g_abKeyPressed[K_UP])
			facing = 1;
		if (g_abKeyPressed[K_DOWN])
			facing = 0;
		if (g_abKeyPressed[K_LEFT])
			facing = 3;
		if (g_abKeyPressed[K_RIGHT])
			facing = 2;
	}
	else
	{
		if (g_abKeyPressed[K_UP])
			facing = 0;
		if (g_abKeyPressed[K_DOWN])
			facing = 1;
		if (g_abKeyPressed[K_LEFT])
			facing = 2;
		if (g_abKeyPressed[K_RIGHT])
			facing = 3;
	}

    if (g_dBounceTime + 0.127 > g_dElapsedTime)
        return;

    // Updating the location of the character based on the key press
    // providing a beep sound whenver we shift the character

	//------------------- 0 - UP, 1 - DOWN, 2 - LEFT, 3 - RIGHT -------------------
    if (g_abKeyPressed[K_UP] && g_sChar.m_cLocation.Y > 0)
    {
        //Beep(1440, 30);
		//---------------------CHECK FOR CURSE---------------------
		if (curse.getActiveCurseShort() == 4 || curse.getActiveCurse2Short() == 4)
		{
			//---------------------GO DOWN IF CURSE IS ACTIVE---------------------
			facing = 1;
			if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y + 1) != mapGen.walls)
			{
				g_sChar.m_cLocation.Y++;
				bSomethingHappened = true;
			}
		}
		else
		{
			//---------------------GO UP IF NO CURSE---------------------
			facing = 0;
			if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y - 1) != mapGen.walls)
			{
				g_sChar.m_cLocation.Y--;
				bSomethingHappened = true;
			}
		}
    }

    if (g_abKeyPressed[K_LEFT] && g_sChar.m_cLocation.X > 0)
    {
        //Beep(1440, 30);
		if (curse.getActiveCurseShort() == 4 || curse.getActiveCurse2Short() == 4)
		{
			//---------------------GO RIGHT IF NO CURSE---------------------
			facing = 3;
			if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X + 1, g_sChar.m_cLocation.Y) != mapGen.walls)
			{
				g_sChar.m_cLocation.X++;
				bSomethingHappened = true;
			}
		}
		else
		{
			//---------------------GO LEFT IF NO CURSE---------------------
			facing = 2;
			if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X - 1, g_sChar.m_cLocation.Y) != mapGen.walls)
			{
				g_sChar.m_cLocation.X--;
				bSomethingHappened = true;
			}
		}
    }

    if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {

		if (curse.getActiveCurseShort() == 4 || curse.getActiveCurse2Short() == 4)
		{
			//---------------------GO UP IF NO CURSE---------------------
			facing = 0;
			//Beep(1440, 30);
			if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y - 1) != mapGen.walls)
			{
				g_sChar.m_cLocation.Y--;
				bSomethingHappened = true;
			}
		}
		else
		{
			//---------------------GO DOWN IF NO CURSE---------------------
			facing = 1;
			//Beep(1440, 30);
			if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y + 1) != mapGen.walls)
			{
				g_sChar.m_cLocation.Y++;
				bSomethingHappened = true;
			}
		}
    }

    if (g_abKeyPressed[K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
		if (curse.getActiveCurseShort() == 4 || curse.getActiveCurse2Short() == 4)
		{
			//---------------------GO LEFT IF NO CURSE---------------------
			facing = 2;
			//Beep(1440, 30);
			if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X - 1, g_sChar.m_cLocation.Y) != mapGen.walls)
			{
				g_sChar.m_cLocation.X--;
				bSomethingHappened = true;
			}
		}
		else
		{
			//---------------------GO RIGHT IF NO CURSE---------------------
			facing = 3;
			//Beep(1440, 30);
			if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X + 1, g_sChar.m_cLocation.Y) != mapGen.walls)
			{
				g_sChar.m_cLocation.X++;
				bSomethingHappened = true;
			}
		}
    }
    if (g_abKeyPressed[K_SPACE] && !useEchoLocation && !countdownStarted)
    {
		//----------------STARTING ECHO LOCATION----------------
		useEchoLocation = true;

		//----------------SET TIMER WITH ELAPSED TIME----------------
		timer = (int)g_dElapsedTime;

		//----------------SET PLAYER LOCATION----------------
		playerLocationX = g_sChar.m_cLocation.X;
		playerLocationY = g_sChar.m_cLocation.Y;

		//----------------SETTING RADIUS FOR ECHO LOCATION----------------
		radiusX = 10;
		radiusY = ((mapGen.hellMode) ? 10 : 5);
        bSomethingHappened = true;
    }

	if (isKeyPressed('Q'))
	{
		//------------CHECK FOR ITEM AND RICKAXE COOLDOWN------------
		if (inven.getInventoryItem(0) != "-" && inven.getRickAxeCoolDown() <= 0)
		{
			//------------MINE WALL------------
			inven.mineWall(facing, g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y);
			//------------RESET RICKAXE COOLDOWN------------
			inven.startRickAxeCoolDown();
			bSomethingHappened = true;
		}
	}

	if (isKeyPressed('W') && curse.getActiveCurseShort() != 5 && curse.getActiveCurse2Short() != 5)
	{
		//------------CHECK FOR ITEM, IF LOCATION HAS BOMB AND BOMB COOLDOWN------------
		if (inven.getInventoryItem(1) != "-" && mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y) != mapGen.bomb && inven.getBombCoolDown() <= 0)
		{
			//------------IF PLAYER HAVE MORE THAN ONE BOMB------------
			if (inven.getBombCount() > 0){
				//------------REPLACE MAP ELEMENT WITH BOMB------------
				mapGen.replaceMapCharacterXY(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y, mapGen.bomb);
				mapGen.bombCoord.X = g_sChar.m_cLocation.X;
				mapGen.bombCoord.Y = g_sChar.m_cLocation.Y;
				//------------RESET BOMB COUNTDOWN------------
				inven.resetBombCountDown();
				//------------RESET BOMB COOLDOWN------------
				inven.startBombCoolDown();
				//------------DECREASE BOMB QUANTITY------------
				inven.decreaseBombCount();
				bSomethingHappened = true;
			}
		}
	}

	if (isKeyPressed('E') && curse.getActiveCurseShort() != 5 && curse.getActiveCurse2Short() != 5)
	{
		//------------CHECK FOR ITEM, IF LOCATION HAS AMMO AND LASER RIFLE COOLDOWN IS DOWN------------
		if (inven.getInventoryItem(2) != "-" && inven.getLaserRifleCoolDown() <= 0)
		{
			//------------IF PLAYER HAVE MORE THAN ONE AMMO SHOT------------
			if (inven.getLaserRifleAmmoCount() > 0){
				//------------REPLACE MAP ELEMENT WITH LASER------------
				mapGen.spawnLazer(facing, g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y, g_dElapsedTime);
				//------------RESET LASER RIFLE COOLDOWN------------
				inven.startLaserRifleCoolDown();
				//------------DECREASE LASER RIFLE QUANTITY------------
				inven.decreaseLaserRifleAmmoCount();
				bSomethingHappened = true;
			}
		}
	}

	if (isKeyPressed('R') && curse.getActiveCurseShort() != 5 && curse.getActiveCurse2Short() != 5)
	{
		//------------CHECK FOR ITEM, IF LOCATION HAS BOMB OR HAS TORCH------------
		if (inven.getInventoryItem(3) != "-" && mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y) != mapGen.bomb && mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y) != mapGen.torch)
		{
			//------------IF PLAYER HAVE MORE THAN ONE TORCH------------
			if (inven.getTorchCount() > 0){
				//------------REPLACE MAP ELEMENT WITH TORCH------------
				mapGen.replaceMapCharacterXY(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y, mapGen.torch);
				//------------DECREASE TORCH QUANTITY------------
				inven.decreaseTorchCount();
				bSomethingHappened = true;
			}
		}
	}
	
    if (bSomethingHappened)
    {
        // set the bounce time to some time in the future to prevent accidental triggers
        g_dBounceTime = g_dElapsedTime + 0.033; // 125ms should be enough
    }
}

void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_abKeyPressed[K_ESCAPE])
        g_bQuitGame = true;    
}

void processUserInputSplashScreen()
{
	// SELECTION PHASE
	if (timerSelection == 0.0)
		timerSelection = g_dElapsedTime;

	if (g_dElapsedTime > timerSelection + 0.1)
	{
		if (g_abKeyPressed[K_UP])
		{
			if (selection == 0)
				selection = 3;
			else
				selection--;

			timerSelection = g_dElapsedTime;
		}
		else if (g_abKeyPressed[K_DOWN])
		{
			if (selection == 3)
				selection = 0;
			else
				selection++;

			timerSelection = g_dElapsedTime;
		}
	}
}

void processUserInputEndScreen()
{
	if (g_dElapsedTime > timerSelection + 0.1)
	{
		if (g_abKeyPressed[K_UP])
		{
			if (endSelection == 0)
				endSelection = 1;
			else
				endSelection--;

			timerSelection = g_dElapsedTime;
		}
		else if (g_abKeyPressed[K_DOWN])
		{
			if (endSelection == 1)
				endSelection = 0;
			else
				endSelection++;

			timerSelection = g_dElapsedTime;
		}
	}

		// quits the game if player hits the escape key
		if (g_abKeyPressed[K_ENTER] && endSelection == 1)
		{
			g_bQuitGame = true;
		}

		if (g_abKeyPressed[K_ENTER] && endSelection == 0)
		{
			g_eGameState = S_SPLASHSCREEN;
			startGame = false;
			mapGen.setBackToDefault(true, true, true, true, true);
			useEchoLocation = false;
			mapGen.setRenderMapAlready(false);
			curse.resetCurse();
			inven.resetBombCoolDown();
			inven.resetRickAxeCoolDown();
			inven.resetLaserRifleCoolDown();
			inven.resetInventory();
			timerDanger = 0;
			timerBomb = 0;
			timerRickAxe = 0;
			timerLaserRifle = 0;
			timeAttack = 60;
			timerAIAttack = 0;
			timerHealthEffect = 0;
			timerSelection = 0;
			g_dElapsedTime = 0;
		}
}

void instructionScreen()
{
	if (g_abKeyPressed[K_ESCAPE] || g_abKeyPressed[K_SPACE])
		g_eGameState = S_SPLASHSCREEN;
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
	g_Console.clearBuffer(0x00);
}

void renderSplashScreen()  // renders the splash screen
{
	srand(time(NULL));
	COORD c = g_Console.getConsoleSize();
	c.Y /= 3;
	c.X = g_Console.getConsoleSize().X / 2 - 30;

	if (rand() % 100 < 50)
	g_Console.writeToBuffer(c, " ______                        ______ _                    ", ((selection == 1) ? 0x0C : 0x03));
	c.Y += 1;
	if (rand() % 100 < 50)
	g_Console.writeToBuffer(c, "(______)                      / _____) |                   ", ((selection == 1) ? 0x0C : 0x03));
	c.Y += 1;
	if (rand() % 100 < 50)
	g_Console.writeToBuffer(c, " _     _ _____ _____ ____    ( (____ | | _____ _____ ____  ", ((selection == 1) ? 0x0C : 0x03));
	c.Y += 1;
	if (rand() % 100 < 50)
	g_Console.writeToBuffer(c, "| |   | | ___ | ___ |  _ \\    \\____ \\| || ___ | ___ |  _ \\ ", ((selection == 1) ? 0x0C : 0x03));
	c.Y += 1;
	if (rand() % 100 < 50)
	g_Console.writeToBuffer(c, "| |__/ /| ____| ____| |_| |   _____) ) || ____| ____| |_| )", ((selection == 1) ? 0x0C : 0x03));
	c.Y += 1;
	if (rand() % 100 < 50)
	g_Console.writeToBuffer(c, "|_____/ |_____)_____)  __/   (______/ \\_)_____)_____)  __/ ", ((selection == 1) ? 0x0C : 0x03));
	c.Y += 1;
	if (rand() % 100 < 50)
	g_Console.writeToBuffer(c, "                    |_|                             |_|    ", ((selection == 1) ? 0x0C : 0x03));


	c.Y += 2;
	c.X = g_Console.getConsoleSize().X / 2 - 10;
	g_Console.writeToBuffer(c, "Welcome to Deep Sleep!", ((selection == 1) ? 0x0C : 0x03));
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 5;
	g_Console.writeToBuffer(c, "Start Game", ((selection == 0) ? 0x19 : 0x09));
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 5;
	g_Console.writeToBuffer(c, "Hell  Mode", ((selection == 1) ? 0x1C : 0x0C));
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 6;
	g_Console.writeToBuffer(c, "Instructions", ((selection == 2) ? 0x19 : 0x09));
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 4;
	g_Console.writeToBuffer(c, "Quit game", ((selection == 3) ? 0x19 : 0x09));
}


void renderGame()
{
	renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
	renderFramerate();
	renderFloorNumber();
	renderHealthbar();
	renderInventory();
	renderHelp();
}

void renderMap()
{

	if (!startGame)
	{
		g_dElapsedTime = 0;
		g_dBounceTime = 0;
		entityBase.startEntityPlayer();
		startGame = true;
	}

	//----------------STARTS COUNTDOWN WHEN COUNTDOWNSTARTED == TRUE----------------
	if (countdownStarted)
	{
		if (g_dElapsedTime > timer) 
		{
			countdownStarted = false;
		}
	}

	COORD c;


	mapGen.generateMap(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y, MAZEX, MAZEY, g_dElapsedTime);

	//---------------------THE ECHO LOCATION VISION---------------------
	if (useEchoLocation) {

		for (int i = playerLocationY + radiusY; i >= playerLocationY - radiusY; i--)
		{
			if (radiusY >= 4) {
				//----------------MAKE END POINT VISIBLE FOR 2 SEC----------------
				c.X = mapGen.getStairLocationX();
				c.Y = mapGen.getStairLocationY();
				g_Console.writeToBuffer(c, mapGen.getArrayCharacter(mapGen.getStairLocationX(), mapGen.getStairLocationY()));
			}

			c.Y = i;
			for (int j = playerLocationX + radiusX; j >= playerLocationX - radiusX; j--)
			{
				c.X = j;
				//----------------MAKING FLOORS AND ENEMIES VISIBLE----------------

				if (j < 43/* MAKING THE ECHO NOT DETECT OUTSIDE THE BOUNDARY */){
					if (mapGen.getArrayCharacter(j, i) == mapGen.floors)
						g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i), mapGen.echoedFloor);

					else if(mapGen.getArrayCharacter(j, i) == mapGen.ammoDrop || mapGen.getArrayCharacter(j, i) == mapGen.bombDrop)
						g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i), mapGen.itemDropColor);
					
				}
			}
		}

		//----------------MAKE ECHO LOCATION RADIUS SHRINK GRADUALLY----------------
		if (g_dElapsedTime > timer + 1)
		{
			timer = (int)g_dElapsedTime;
			radiusX -= 2;
			radiusY--;
		}


		if (radiusY <= -1)
		{
			useEchoLocation = false;
			countdownStarted = true;
		}
	}

	mapGen.playerVision(g_dElapsedTime, g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y);
	mapGen.torchView();
	mapGen.lazerView();

	//---------------------GET BLOOD CURSE---------------------
	if (curse.getActiveCurseShort() == 2 || curse.getActiveCurse2Short() == 2)
	{
		//---------------------MINUS HP EVERY 3 SEC---------------------
		if (timerHealthEffect == 0)
			timerHealthEffect = g_dElapsedTime;

		if (g_dElapsedTime > timerHealthEffect + ((mapGen.hellMode) ? 8 : 3))
		{
			curse.bleedCurse();
			timerHealthEffect = g_dElapsedTime;
		}
	} 
	else
	{
		if (entityBase.getPlayerHealth() < entityBase.getPlayerMaxHealth())
		{
			if ((timerHealEffect - g_dElapsedTime) <= 0)
			{
				entityBase.damagePlayer(-1);
				timerHealEffect = g_dElapsedTime + ((mapGen.hellMode) ? 5 : 6);
			}
		}
	}

	//----------------COOLDOWN OF THE BOMB----------------
	if (inven.getBombCoolDown() > 0)
	{

		if (g_dElapsedTime > timerBomb)
		{
			//----------------DECREASE COOLDOWN UNTIL REACH ZERO----------------
			inven.decreaseBombCoolDown();
		}
		timerBomb = g_dElapsedTime + 1;
	}

	//----------------COOLDOWN OF THE RICKAXE----------------
	if (inven.getRickAxeCoolDown() > 0)
	{
		if (timerRickAxe == 0)
			timerRickAxe = g_dElapsedTime + 1;

		if (g_dElapsedTime > timerRickAxe)
		{
			//----------------DECREASE COOLDOWN UNTIL REACH ZERO----------------
			inven.decreaseRickAxeCoolDown();
		}
		timerRickAxe = g_dElapsedTime + 1;
	}

	//----------------COOLDOWN OF THE LAZER RIFLE----------------
	if (inven.getLaserRifleCoolDown() > 0)
	{
		if (timerLaserRifle == 0)
			timerLaserRifle = g_dElapsedTime + 1;

		if (g_dElapsedTime > timerLaserRifle)
		{
			//----------------DECREASE COOLDOWN UNTIL REACH ZERO----------------
			inven.decreaseLaserRifleCoolDown();
		}
		timerLaserRifle = g_dElapsedTime + 1;
	}

	//---------------------GENERATE A NEW MAP BUT KEEPS THE PLAYER CURRENT LOCATION WHEN REACH GOAL (AKA CREATES NEXT LEVEL)---------------------
	if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y) == mapGen.stair || isKeyPressed('C'))
	{
		mapGen.setBackToDefault(false, true, false, true, false);
		mapGen.increaseDifficulty();
		useEchoLocation = false;
		mapGen.setRenderMapAlready(false);
		inven.resetBombCoolDown();
		inven.resetRickAxeCoolDown();
		inven.resetLaserRifleCoolDown();
		mapGen.resetLazer();
		timerHealEffect = timerHealEffect - g_dElapsedTime;

		g_dElapsedTime = 0;
		g_dBounceTime = 0;
		timerDanger = 0;
		timerBomb = 0;
		timerRickAxe = 0;
		timerLaserRifle = 0;

		if(!mapGen.hellMode)
			timeAttack = 60;
		else
			timeAttack += 20;

		timerAIAttack = 0;
		timerHealthEffect = 0;


		if (mapGen.getFloorLevel() % 6 == 0 && entityBase.getPlayerMaxHealth() < 30 && mapGen.hellMode)
		{
			entityBase.damagePlayer(-1);
			entityBase.increasePlayerMaxHealth(1);
		}
	}

	//---------------------DAMAGE PLAYER IF THE ENEMY TOUCHES THE PLAYER---------------------
	if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y) == mapGen.enemy)
	{
		if(timerAIAttack == 0)
			timerAIAttack = g_dElapsedTime;

		if (g_dElapsedTime > timerAIAttack + ((mapGen.hellMode) ? 0.477 : 0.3))
		{
			entityBase.damagePlayer(1);
			timerAIAttack = g_dElapsedTime;
		}
	}

	//---------------------SHOW GAME OVER SCREEN WHEN TIME RUNS OUT OR HEALTH IS ZERO---------------------
	if (entityBase.getPlayerHealth() <= 0 || isTimeOver())
	{
		setHighScore(mapGen.getFloorLevel());
		g_eGameState = S_GAMEOVER;
		g_dElapsedTime = 0;
		timerSelection = 0;
	}
}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor;
	char player;

	if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y) == mapGen.enemy)
		charColor = 0x0C;
	else
		charColor = 0x0A;

	switch (facing)
	{
	case 0: 
		player = '^';
		break;
	case 1:
		player = 'V';
		break;
	case 2:
		player = '<';
		break;
	case 3:
		player = '>';
		break;
	default:
		player = '^';
		break;
	}

	g_Console.writeToBuffer(g_sChar.m_cLocation, player, charColor); //CHARACTER STUFF
}

void renderFramerate()
{
	COORD c;
	// displays the framerate
	std::ostringstream ss, ss2;
	//ss << std::fixed << std::setprecision(3);
	//ss << 1.0 / g_dDeltaTime << "fps";
	//c.X = g_Console.getConsoleSize().X - 9;
	//c.Y = 0;
	//g_Console.writeToBuffer(c, ss.str());

	// displays the elapsed time
	ss.str("");
	ss << timeAttack << " secs left!";
	c.X = (g_Console.getConsoleSize().X / 2) - 5;
	c.Y = 0;

	if (timerDanger == 0)
		timerDanger = (int)g_dElapsedTime;

		if (g_dElapsedTime > timerDanger + 1)
		{

			if (curse.getActiveCurseShort() == 1 || curse.getActiveCurse2Short() == 1)
				timeAttack -= 2;
			else
				timeAttack -= 1;

			//---------------------FLASH RED WHEN LESS THAN 10 SECONDS LEFT---------------------
			if (timeAttack < 10)
			{
				

				switch (danger)
				{
				case 0:
					danger = 1;
					//---------------------SET RED---------------------
					color = 0x0C;
					break;
				case 1:
					danger = 0;
					//---------------------SET WHITE---------------------
					color = 0x0F;
					break;
				default:
					break;
				}
			}

		else
		{
			color = 0x0F;
		}

			timerDanger = (int)g_dElapsedTime;
	}
	g_Console.writeToBuffer(c, ss.str(), color);

	ss2 << "Facing: " << ((facing > 1) ? ((facing == 2) ? "LEFT" : "RIGHT") : ((facing == 0) ? "UP" : "DOWN"));
	c.Y += 2;
	g_Console.writeToBuffer(c, ss2.str());
}


void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}


//---------------------CHECK IF TIME LEFT IS 0---------------------
bool isTimeOver()
{
	return ((timeAttack <= 0) ? true : false);
}