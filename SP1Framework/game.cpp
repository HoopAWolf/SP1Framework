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

double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];
double timerAIAttack = 0.0;


//----------------SET BOOL VARIABLE FOR MAP RENDERING, ECHO LOCATION, COUNTDOWN----------------
bool useEchoLocation = false, countdownStarted = false, startGame = false;

//----------------SET GOAL X AND Y LOCATION, PLAYER X AND Y LOCATION, RADIUS FOR ECHO LOCATION----------------
int timer, playerLocationX, playerLocationY, radiusX, radiusY, facing, timeAttack = 60, danger = 0, timerDanger = 0, timerBomb = 0, timerRickAxe = 0, timerLaserRifle = 0;

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
    g_Console.setConsoleFont(0, 40, L"Consolas");
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

void gameOver()
{
	processUserInputEndScreen();
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
		facing = 0;
        //Beep(1440, 30);
		if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y - 1) != (char)178)
		{
			g_sChar.m_cLocation.Y--;
			bSomethingHappened = true;
		}
    }
    if (g_abKeyPressed[K_LEFT] && g_sChar.m_cLocation.X > 0)
    {
		facing = 2;
        //Beep(1440, 30);
		if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X - 1, g_sChar.m_cLocation.Y) != (char)178)
		{
			g_sChar.m_cLocation.X--;
			bSomethingHappened = true;
		}
    }

    if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
		facing = 1;
        //Beep(1440, 30);
		if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y + 1) != (char)178)
		{
			g_sChar.m_cLocation.Y++;
			bSomethingHappened = true;
		}
    }
    if (g_abKeyPressed[K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
		facing = 3;
        //Beep(1440, 30);
		if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X + 1, g_sChar.m_cLocation.Y) != (char)178)
		{
			g_sChar.m_cLocation.X++;
			bSomethingHappened = true;
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

	if (isKeyPressed('S'))
	{
		//------------CHECK FOR ITEM, IF LOCATION HAS BOMB AND BOMB COOLDOWN------------
		if (inven.getInventoryItem(1) != "-" && mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y) != mapGen.bomb && inven.getBombCoolDown() <= 0)
		{
			//------------IF PLAYER HAVE MORE THAN ONE BOMB------------
			if (inven.getBombCount() > 0){
				//------------REPLACE MAP ELEMENT WITH BOMB------------
				mapGen.replaceMapCharacterXY(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y, mapGen.bomb);
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

	if (isKeyPressed('D'))
	{
		//------------CHECK FOR ITEM, IF LOCATION HAS AMMO AND LASER RIFLE COOLDOWN IS DOWN------------
		if (inven.getInventoryItem(2) != "-" && inven.getLaserRifleCoolDown() <= 0)
		{
			//------------IF PLAYER HAVE MORE THAN ONE AMMO SHOT------------
			if (inven.getLaserRifleAmmoCount() > 0){
				//------------REPLACE MAP ELEMENT WITH LASER------------
				//mapGen.replaceMapCharacterXY(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y, mapGen.bomb);
				//------------RESET LASER RIFLE COOLDOWN------------
				inven.startLaserRifleCoolDown();
				//------------DECREASE LASER RIFLE QUANTITY------------
				inven.decreaseLaserRifleAmmoCount();
				bSomethingHappened = true;
			}
		}
	}

	if (isKeyPressed('F'))
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
        g_dBounceTime = g_dElapsedTime + 0.125; // 125ms should be enough
    }
}
void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_abKeyPressed[K_ESCAPE])
        g_bQuitGame = true;    
}

void processUserInputEndScreen()
{
	// quits the game if player hits the escape key
	if (g_abKeyPressed[K_ESCAPE])
		g_bQuitGame = true;

	if (isKeyPressed('R'))
	{
		g_eGameState = S_SPLASHSCREEN;
		startGame = false;
		mapGen.setBackToDefault(true, true, true, true, true);
		useEchoLocation = false;
	}
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
	g_Console.writeToBuffer(c, " ______                        ______ _                    ", 0x03);
	c.Y += 1;
	if (rand() % 100 < 50)
	g_Console.writeToBuffer(c, "(______)                      / _____) |                   ", 0x03);
	c.Y += 1;
	if (rand() % 100 < 50)
	g_Console.writeToBuffer(c, " _     _ _____ _____ ____    ( (____ | | _____ _____ ____  ", 0x03);
	c.Y += 1;
	if (rand() % 100 < 50)
	g_Console.writeToBuffer(c, "| |   | | ___ | ___ |  _ \\    \\____ \\| || ___ | ___ |  _ \\ ", 0x03);
	c.Y += 1;
	if (rand() % 100 < 50)
	g_Console.writeToBuffer(c, "| |__/ /| ____| ____| |_| |   _____) ) || ____| ____| |_| )", 0x03);
	c.Y += 1;
	if (rand() % 100 < 50)
	g_Console.writeToBuffer(c, "|_____/ |_____)_____)  __/   (______/ \\_)_____)_____)  __/ ", 0x03);
	c.Y += 1;
	if (rand() % 100 < 50)
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
	renderHelp();
}

void renderMap()
{

	//----------------SETTING ITEMS(TEMPORARY HERE FOR NOW)----------------
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

	//----------------THIS IS TO SET THE SIZE OF MAZE----------------
	int mazeSizeX = 43, mazeSizeY = 23;

	mapGen.generateMap(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y, mazeSizeX, mazeSizeY, g_dElapsedTime);

	//---------------------THE ECHO LOCATION VISION---------------------
	if (useEchoLocation) {

		for (int i = playerLocationY + radiusY; i >= playerLocationY - radiusY; i--)
		{
			if (radiusY >= 7) {
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
			if (mapGen.getArrayCharacter(j, i) == mapGen.floors)
				g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i), mapGen.floorColor);

			else if (mapGen.getArrayCharacter(j, i) == mapGen.stair)
				g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i));

			else if (mapGen.getArrayCharacter(j, i) == mapGen.walls)
				g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i), mapGen.wallColor);

			else if (mapGen.getArrayCharacter(j, i) == mapGen.enemy)
				g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i), mapGen.enemyColor);

			else if (mapGen.getArrayCharacter(j, i) == mapGen.bombDrop || mapGen.getArrayCharacter(j, i) == mapGen.ammoDrop)
				g_Console.writeToBuffer(c, mapGen.getArrayCharacter(j, i), mapGen.itemDropColor);
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

	//--------------------------------IF PLAYER PICK UP ITEM--------------------------------------
	if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y) == mapGen.bombDrop)
	{
		mapGen.replaceMapCharacterXY(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y, mapGen.floors);
		inven.setInventoryBombCount(inven.getBombCount() + 1);
	}

	if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y) == mapGen.ammoDrop)
	{
		mapGen.replaceMapCharacterXY(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y, mapGen.floors);
		inven.setInventoryLaserRifleAmmountCount(inven.getLaserRifleAmmoCount() + 1);
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
		g_dElapsedTime = 0;
		g_dBounceTime = 0;
		timerDanger = 0;
		timerBomb = 0;
		timerRickAxe = 0;
		timerLaserRifle = 0;
		timerAIAttack = 0;
	}

	//---------------------DAMAGE PLAYER IF THE ENEMY TOUCHES THE PLAYER---------------------
	if (mapGen.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y) == mapGen.enemy)
	{
		if(timerAIAttack == 0)
			timerAIAttack = g_dElapsedTime;

		if (g_dElapsedTime > timerAIAttack + 0.5)
		{
			entityBase.damagePlayer(1);
			timerAIAttack = g_dElapsedTime;
		}
	}

	//---------------------SHOW GAME OVER SCREEN WHEN TIME RUNS OUT OR HEALTH IS ZERO---------------------
	if (entityBase.getPlayerHealth() <= 0 || isTimeOver())
		g_eGameState = S_GAMEOVER;
}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0A;

	g_Console.writeToBuffer(g_sChar.m_cLocation, mapGen.player, charColor); //CHARACTER STUFF
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
    ss << ((timeAttack - (int)g_dElapsedTime)) - ((curse.getActiveCurseShort() == 1) ? 1 : 0) << " secs left!";
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
	else
	{
		color = 0x0F;
	}

    g_Console.writeToBuffer(c, ss.str(), color);
}



//----------------RENDER GAME OVER----------------
void renderGameOver()
{
	COORD c = g_Console.getConsoleSize();
	c.Y /= 3;
	c.X = g_Console.getConsoleSize().X / 2 - 32;

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
	g_Console.writeToBuffer(c, "==========================YOU DIEDED!=========================", 0x0A/* green color */);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 13;
	g_Console.writeToBuffer(c, "Press <R> to start game", 0x0A);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 11;
	g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x0A);

}

//---------------------RENDER FLOOR NUMBER ONTO SCREEN---------------------
void renderFloorNumber()
{
	COORD c;
	std::ostringstream ss, ss2;
	c.X = (g_Console.getConsoleSize().X / 2) - 3;
	c.Y = 1;
	ss << mapGen.getFloorLevel() << " Floor";
	g_Console.writeToBuffer(c, ss.str());

	c.Y += 5;
	c.X += 10;
	ss2 << "[Curse Active: " << curse.getActiveCurseString() << "]";
	g_Console.writeToBuffer(c, ss2.str(), 0x0E);
}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

//----------------RENDER HEALTH BAR----------------
void renderHealthbar()
{

	std::ostringstream HPcout, HPBar;
	c.Y = 8;
	c.X = 50;

	//---------------------PRINT HEALTH---------------------
	for (int i = 0; i < 20; i++)
	{
		if (i == 0)
			HPBar << "["; 

		if (i < entityBase.getPlayerHealth())
			HPBar << mapGen.walls; //------------PRINT BAR FOR HEALTH------------
		else
			HPBar << "-"; //------------PRINT '-' FOR MISSING HEALTH------------

		if (i == 19)
			HPBar << "]";
	}

	g_Console.writeToBuffer(c, HPBar.str(), 0x0C/* red color */);

	HPcout << "Health: " << entityBase.getPlayerHealth() << "/20";

	c.X += 4;
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

	for (int i = 0; i < inven.getInventorySize(); i++)
	{
		//------------RENDER WORDS RED IF COOLDOWN IS STILL ON------------
		if (i == 1 && inven.getBombCoolDown() > 0 || i == 0 && inven.getRickAxeCoolDown() > 0 || i == 2 && inven.getLaserRifleCoolDown() > 0)
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
	c.Y = 0;
	c.X = 79;
	g_Console.writeToBuffer(c, "Enemy: ", 0x0D/* pink color */);
	c.X += 8;
	g_Console.writeToBuffer(c, mapGen.enemy, 0x0C/* red color */);

	c.Y += 1;
	c.X = 80;
	g_Console.writeToBuffer(c, "Bomb: ", 0x0D/* pink color */);
	c.X += 7;
	g_Console.writeToBuffer(c, mapGen.bomb, 0x0C/* red color */);

	c.Y += 1;
	c.X = 75;
	g_Console.writeToBuffer(c, "Objective: ", 0x0D/* pink color */);
	c.X += 12;
	g_Console.writeToBuffer(c, mapGen.stair);
}

//---------------------CHECK IF TIME LEFT IS 0---------------------
bool isTimeOver()
{
	return (((timeAttack - ((int)g_dElapsedTime)) - ((curse.getActiveCurseShort() == 1) ? 1 : 0) <= 0) ? true : false);
}