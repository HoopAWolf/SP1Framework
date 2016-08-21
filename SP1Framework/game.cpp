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

double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];


//----------------SET BOOL VARIABLE FOR MAP RENDERING, ECHO LOCATION, COUNTDOWN----------------
bool useEchoLocation = false, countdownStarted = false, clearItemsAlready = false;

//----------------SET GOAL X AND Y LOCATION, PLAYER X AND Y LOCATION, RADIUS FOR ECHO LOCATION----------------
int timer, playerLocationX, playerLocationY, radiusX, radiusY, facing, timeAttack = 60, danger = 0, timerDanger = 0;

// MAP GENERATOR OBJECT
MapGenerator mapgenerator;
//ENTITY OPJECT
Entity entity;
//INVENTORY OPJECT
Inventory inventory;

COORD c;
WORD color = 0x0F;

// Game specific variables here
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once

// Console object
Console g_Console(100, 25, "Deep Sleep - By Team Insomnia");



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
    g_Console.setConsoleFont(0, 16, L"Consolas");
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
	//g_abKeyPressed[K_0] = isKeyPressed(VK_NUMPAD0);
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
        case S_GAME: gameplay(); // gameplay logic when we are in the game
            break;
		case S_GAMEOVER: renderGameOver(); // -----renders the game over stuff-----
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
        case S_GAME: renderGame();
            break;
		case S_GAMEOVER: renderGameOver();
			break;
    }
   // renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
	if (g_abKeyPressed[K_ENTER]) // PRESS ENTER TO START GAME
        g_eGameState = S_GAME;
}

void gameplay()            // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
}

void moveCharacter()
{
    bool bSomethingHappened = false;
    if (g_dBounceTime > g_dElapsedTime)
        return;

    // Updating the location of the character based on the key press
    // providing a beep sound whenver we shift the character
    if (g_abKeyPressed[K_UP] && g_sChar.m_cLocation.Y > 0)
    {
        //Beep(1440, 30);
		if (mapgenerator.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y - 1) != (char)178)
		{
			g_sChar.m_cLocation.Y--;
			bSomethingHappened = true;
			facing = 0;
		}
    }
    if (g_abKeyPressed[K_LEFT] && g_sChar.m_cLocation.X > 0)
    {
        //Beep(1440, 30);
		if (mapgenerator.getArrayCharacter(g_sChar.m_cLocation.X - 1, g_sChar.m_cLocation.Y) != (char)178)
		{
			g_sChar.m_cLocation.X--;
			bSomethingHappened = true;
			facing = 2;
		}
    }

    if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
        //Beep(1440, 30);
		if (mapgenerator.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y + 1) != (char)178)
		{
			g_sChar.m_cLocation.Y++;
			bSomethingHappened = true;
			facing = 1;
		}
    }
    if (g_abKeyPressed[K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
        //Beep(1440, 30);
		if (mapgenerator.getArrayCharacter(g_sChar.m_cLocation.X + 1, g_sChar.m_cLocation.Y) != (char)178)
		{
			g_sChar.m_cLocation.X++;
			bSomethingHappened = true;
			facing = 3;
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
		radiusX = 16;
		radiusY = 8;
        bSomethingHappened = true;
    }

	if (isKeyPressed('A'))
	{
		if (inventory.getInventoryItem(0) != "-")
		{
			inventory.mineWall(facing, g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y);
			bSomethingHappened = true;
		}
	}

	if (isKeyPressed('S'))
	{
		if (inventory.getInventoryItem(1) != "-")
		{
			inventory.useExplosion(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y);
			bSomethingHappened = true;
		}
	}

    if (bSomethingHappened)
    {
        // set the bounce time to some time in the future to prevent accidental triggers
        g_dBounceTime = g_dElapsedTime + 0.125; // 125ms should be enough
    }
}
void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_abKeyPressed[K_ESCAPE])
        g_bQuitGame = true;    
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
	g_Console.clearBuffer(0x00);
}

void renderSplashScreen()  // renders the splash screen
{
	COORD c = g_Console.getConsoleSize();
	c.Y /= 3;
	c.X = g_Console.getConsoleSize().X / 2 - 30;

	g_Console.writeToBuffer(c, " ______                        ______ _                    ", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "(______)                      / _____) |                   ", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, " _     _ _____ _____ ____    ( (____ | | _____ _____ ____  ", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "| |   | | ___ | ___ |  _ \\    \\____ \\| || ___ | ___ |  _ \\ ", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "| |__/ /| ____| ____| |_| |   _____) ) || ____| ____| |_| )", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "|_____/ |_____)_____)  __/   (______/ \\_)_____)_____)  __/ ", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "                    |_|                             |_|    ", 0x03);


	c.Y += 2;
	c.X = g_Console.getConsoleSize().X / 2 - 10;
	g_Console.writeToBuffer(c, "Welcome to Deep Sleep!", 0x03);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 13;
	g_Console.writeToBuffer(c, "Press <Enter> to start game", 0x09);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 15;
	g_Console.writeToBuffer(c, "Press 'Esc' to quit once in-game", 0x09);
}

void renderGame()
{
	renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
	renderFramerate();
	renderFloorNumber();
	renderHealthbar();
	renderInventory();
}

void renderMap()
{

	//----------------SETTING ITEMS(TEMPORARY HERE FOR NOW)----------------
	if (!clearItemsAlready)
	{
		inventory.clearInventory();
		inventory.setInventory(true, true);
		clearItemsAlready = true;
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

	//----------------THIS IS TO SET THE SIZE OF MAZE----------------
	int mazeSizeX = 43, mazeSizeY = 23;

	mapgenerator.generateMap(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y, mazeSizeX, mazeSizeY, g_dElapsedTime);

	//---------------------THE ECHO LOCATION VISION---------------------
	if (useEchoLocation) {

		for (int i = playerLocationY + radiusY; i >= playerLocationY - radiusY; i--)
		{
			if (radiusY >= 7) {
				//----------------MAKE END POINT VISIBLE FOR 2 SEC----------------
				c.X = mapgenerator.getStairLocationX();
				c.Y = mapgenerator.getStairLocationY();
				g_Console.writeToBuffer(c, mapgenerator.getArrayCharacter(mapgenerator.getStairLocationX(), mapgenerator.getStairLocationY()));
			}

			c.Y = i;
			for (int j = playerLocationX + radiusX; j >= playerLocationX - radiusX; j--)
			{
				c.X = j;
				//----------------MAKING FLOORS AND ENEMIES VISIBLE----------------

				if (j < 43/* MAKING THE ECHO NOT DETECT OUTSIDE THE BOUNDARY */){
					if (mapgenerator.getArrayCharacter(j, i) == mapgenerator.floors)
						g_Console.writeToBuffer(c, mapgenerator.getArrayCharacter(j, i), mapgenerator.echoedFloor);
					
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

	//---------------------THE 3 X 3 VISION---------------------
	for (int i = g_sChar.m_cLocation.Y + 1; i >= g_sChar.m_cLocation.Y - 1; i--)
	{
		c.Y = i;
		for (int j = g_sChar.m_cLocation.X + 2; j >= g_sChar.m_cLocation.X - 2; j--)
		{
			c.X = j;
			//----------------DISPLAYING FLOORS, END GOAL(STAIR), WALLS, ENEMIES----------------
			if (mapgenerator.getArrayCharacter(j, i) == mapgenerator.floors)
				g_Console.writeToBuffer(c, mapgenerator.getArrayCharacter(j, i), mapgenerator.floorColor);

			else if (mapgenerator.getArrayCharacter(j, i) == mapgenerator.stair)
				g_Console.writeToBuffer(c, mapgenerator.getArrayCharacter(j, i));

			else if (mapgenerator.getArrayCharacter(j, i) == mapgenerator.walls)
				g_Console.writeToBuffer(c, mapgenerator.getArrayCharacter(j, i), mapgenerator.wallColor);

			else if (mapgenerator.getArrayCharacter(j, i) == mapgenerator.enemy)
			{
				g_Console.writeToBuffer(c, mapgenerator.getArrayCharacter(j, i), mapgenerator.enemyColor);
			}
		}
	}

	//---------------------GENERATE A NEW MAP BUT KEEPS THE PLAYER CURRENT LOCATION WHEN REACH GOAL (AKA CREATES NEXT LEVEL)---------------------
	if (mapgenerator.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y) == mapgenerator.stair || isKeyPressed('C'))
	{
		mapgenerator.setBackToDefault(false, true, false, true);
		mapgenerator.increaseDifficulty();
		useEchoLocation = false;
		mapgenerator.setRenderMapAlready(false);
		g_dElapsedTime = 0;
		g_dBounceTime = 0;
	}

	//---------------------DAMAGE PLAYER IF THE ENEMY TOUCHES THE PLAYER---------------------
	if (mapgenerator.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y) == mapgenerator.enemy)
		entity.damagePlayer(1);

	//---------------------SHOW GAME OVER SCREEN WHEN TIME RUNS OUT OR HEALTH IS ZERO---------------------
	if (entity.getPlayerHealth() <= 0 || isTimeOver())
		g_eGameState = S_GAMEOVER;
}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0A;

	g_Console.writeToBuffer(g_sChar.m_cLocation, mapgenerator.player, charColor); //CHARACTER STUFF
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    //ss << std::fixed << std::setprecision(3);
    //ss << 1.0 / g_dDeltaTime << "fps";
    //c.X = g_Console.getConsoleSize().X - 9;
    //c.Y = 0;
    //g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << (timeAttack - (int)g_dElapsedTime) << " secs left!";
	c.X = (g_Console.getConsoleSize().X / 2) - 5;
    c.Y = 0;

	//---------------------FLASH RED WHEN LESS THAN 10 SECONDS LEFT---------------------
	if ((timeAttack - (int)g_dElapsedTime) < 10)
	{
		if (timerDanger == 0)
			timerDanger = (int)g_dElapsedTime;

		if (g_dElapsedTime > timerDanger + 1)
		{
			switch (danger)
			{
			case 0:
				danger = 1;
				//---------------------SET RED---------------------
				color = 0x0C;
				timerDanger = (int)g_dElapsedTime;
				break;
			case 1:
				danger = 0;
				//---------------------SET WHITE---------------------
				color = 0x0F;
				timerDanger = (int)g_dElapsedTime;
				break;
			default:
				break;
			}
		}
	}

    g_Console.writeToBuffer(c, ss.str(), color);
}

//----------------RENDER GAME OVER----------------
void renderGameOver()
{
	g_Console.writeToBuffer(0, 0, "deaded", 0x0A/* green color */);

}

//---------------------RENDER FLOOR NUMBER ONTO SCREEN---------------------
void renderFloorNumber()
{
	COORD c;
	std::ostringstream ss;
	c.X = (g_Console.getConsoleSize().X / 2) - 3;
	c.Y = 1;
	ss << mapgenerator.getFloorLevel() << " Floor";
	g_Console.writeToBuffer(c, ss.str());
}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

//----------------RENDER HEALTH BAR----------------
void renderHealthbar()
{

	std::ostringstream HPcout;
	string HPBar;
	c.Y = 8;
	c.X = 50;

	//---------------------PRINT HEALTH---------------------
	for (int i = 0; i < 20; i++)
	{
		if (i == 0)
			HPBar += "[";

		if (i < entity.getPlayerHealth())
			HPBar += mapgenerator.walls;
		else
			HPBar += "-";

		if (i == 19)
			HPBar += "]";
	}

	g_Console.writeToBuffer(c, HPBar, 0x0C/* red color */);

	HPcout << "Health: " << entity.getPlayerHealth() << "/20";

	c.Y += 1;
	g_Console.writeToBuffer(c, HPcout.str(), 0x0A/* green color */);

}

//----------------RENDER INVENTORY----------------
void renderInventory()
{

	c.Y = 11;
	c.X = 50;

	g_Console.writeToBuffer(c, "Inventory: ", 0x0A/* green color */);
	c.Y++;

	for (int i = 0; i < inventory.getInventorySize(); i++)
	{
		g_Console.writeToBuffer(c, inventory.getInventoryItem(i), 0x0A/* green color */);
		c.Y++;
	}

}

//---------------------CHECK IF TIME LEFT IS 0---------------------
bool isTimeOver()
{
	return (((timeAttack - ((int)g_dElapsedTime)) <= 0) ? true : false);
}