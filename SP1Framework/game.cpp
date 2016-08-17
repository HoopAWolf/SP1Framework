// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "MapGenerator.h"

double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];

//----------------SET BOOL VARIABLE FOR MAP RENDERING, ECHO LOCATION, COUNTDOWN----------------
bool useEchoLocation = false, countdownStarted = false;

//----------------SET GOAL X AND Y LOCATION, PLAYER X AND Y LOCATION, RADIUS FOR ECHO LOCATION----------------
int timer, playerLocationX, playerLocationY, radiusX, radiusY;

// MAP GENERATOR OBJECT
MapGenerator mapgenerator;

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
		}
    }
    if (g_abKeyPressed[K_LEFT] && g_sChar.m_cLocation.X > 0)
    {
        //Beep(1440, 30);
		if (mapgenerator.getArrayCharacter(g_sChar.m_cLocation.X - 1, g_sChar.m_cLocation.Y) != (char)178)
		{
			g_sChar.m_cLocation.X--;
			bSomethingHappened = true;
		}
    }

    if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
        //Beep(1440, 30);
		if (mapgenerator.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y + 1) != (char)178)
		{
			g_sChar.m_cLocation.Y++;
			bSomethingHappened = true;
		}
    }
    if (g_abKeyPressed[K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
        //Beep(1440, 30);
		if (mapgenerator.getArrayCharacter(g_sChar.m_cLocation.X + 1, g_sChar.m_cLocation.Y) != (char)178)
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
    g_Console.clearBuffer(0x1F);
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
}

void renderMap()
{
	//----------------STARTS COUNTDOWN WHEN COUNTDOWNSTARTED == TRUE----------------
	if (countdownStarted)
	{
		if (g_dElapsedTime > timer) 
		{
			countdownStarted = false;
		}
	}

	COORD c;
	//----------------THIS IS THE COLOUR CODES----------------
	WORD floorColor = 0x0A, wallColor = 0x0C, blackColor = 0x0000, echoedFloor = 0x08;

	//----------------THIS IS THE WALL SYMBOL FROM ASCII----------------
	char walls = 178, floors = 176, stair = 'X';

	//----------------THIS IS TO SET THE SIZE OF MAZE----------------
	int x = 43, y = 23;

	mapgenerator.generateMap(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y);

	//----------------WRITING THE MAP FROM ARRAY INTO THE CONSOLE BUFFER----------------
	for (int j = 0; j < y; j++)
	{
		c.Y = j;
		for (int i = 0; i < x; i++)
		{
			c.X = i;
			//----------------SETTING MAP TO TOTAL DARKNESSSSSS----------------
			g_Console.writeToBuffer(c, mapgenerator.getArrayCharacter(x, y), blackColor);
		}
	}


	//----------------WRITING THE MAP FROM ARRAY INTO THE CONSOLE BUFFER----------------
	for (int j = 0; j <= y; j++)
	{
		c.Y = j;
		for (int i = 0; i <= x; i++)
		{
			c.X = i;
			//----------------SETTING MAP TO TOTAL DARKNESSSSSS----------------
			g_Console.writeToBuffer(c, mapgenerator.getArrayCharacter(x, y), blackColor);


		}
	}

	//---------------------THE ECHO LOCATION VISION---------------------
	if (useEchoLocation) {

		for (int i = playerLocationY + radiusY; i >= playerLocationY - radiusY; i--)
		{
			if (radiusY >= 6) {
				//----------------MAKE END POINT VISIBLE FOR 2 SEC----------------
				c.X = mapgenerator.getStairLocationX();
				c.Y = mapgenerator.getStairLocationY();
				g_Console.writeToBuffer(c, mapgenerator.getArrayCharacter(mapgenerator.getStairLocationX(), mapgenerator.getStairLocationY()));
			}

			c.Y = i;
			for (int j = playerLocationX + radiusX; j >= playerLocationX - radiusX; j--)
			{
				c.X = j;
				//----------------MAKING FLOORS VISIBLE----------------
				if (mapgenerator.getArrayCharacter(j, i) == floors)
					g_Console.writeToBuffer(c, mapgenerator.getArrayCharacter(j, i), echoedFloor);

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
			//----------------DISPLAYING FLOORS, END GOAL(STAIR), WALLS----------------
			if (mapgenerator.getArrayCharacter(j, i) == floors)
				g_Console.writeToBuffer(c, mapgenerator.getArrayCharacter(j, i), floorColor);

			else if (mapgenerator.getArrayCharacter(j, i) == stair)
				g_Console.writeToBuffer(c, mapgenerator.getArrayCharacter(j, i));

			else if (mapgenerator.getArrayCharacter(j, i) == walls)
				g_Console.writeToBuffer(c, mapgenerator.getArrayCharacter(j, i), wallColor);
		}
	}

	//---------------------GENERATE A NEW MAP BUT KEEPS THE PLAYER CURRENT LOCATION WHEN REACH GOAL---------------------
	if (mapgenerator.getArrayCharacter(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y) == stair)
	{
		g_dBounceTime = g_dElapsedTime + 0.125;
		mapgenerator.setBackToDefault(false, true);
		mapgenerator.increaseDifficulty();
		mapgenerator.setRenderMapAlready(false);
	}
}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0C;
    if (g_sChar.m_bActive)
    {
        charColor = 0x0A;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)1, charColor);
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x59);
}
void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}
