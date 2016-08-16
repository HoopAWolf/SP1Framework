// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>

double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];
bool renderMapAlready, useEchoLocation = false, countdownStarted = false;
int timer, XlocationX, XlocationY, playerLocationX, playerLocationY, radiusX, radiusY;
char mapArray[45][45] = { '0', };

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
	renderMapAlready = false;
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
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 3.0) // wait for 3 seconds to switch to game mode, else do nothing
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
		if (mapArray[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y - 1] != (char)178)
		{

			g_sChar.m_cLocation.Y--;
			bSomethingHappened = true;
		}
    }
    if (g_abKeyPressed[K_LEFT] && g_sChar.m_cLocation.X > 0)
    {
        //Beep(1440, 30);
		if (mapArray[g_sChar.m_cLocation.X - 1] [g_sChar.m_cLocation.Y] != (char)178)
		{
			g_sChar.m_cLocation.X--;
			bSomethingHappened = true;
		}
    }

    if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
        //Beep(1440, 30);
		if (mapArray[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y + 1] != (char)178)
		{
			g_sChar.m_cLocation.Y++;
			bSomethingHappened = true;
		}
    }
    if (g_abKeyPressed[K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
        //Beep(1440, 30);
		if (mapArray[g_sChar.m_cLocation.X + 1][g_sChar.m_cLocation.Y] != (char)178)
		{
			g_sChar.m_cLocation.X++;
			bSomethingHappened = true;
		}
    }
    if (g_abKeyPressed[K_SPACE] && !useEchoLocation && !countdownStarted)
    {
		useEchoLocation = true;
		timer = (int)g_dElapsedTime;
		playerLocationX = g_sChar.m_cLocation.X;
		playerLocationY = g_sChar.m_cLocation.Y;
		radiusX = 20;
		radiusY = 10;
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
    c.X = c.X / 2 - 9;
    g_Console.writeToBuffer(c, "A game in 3 seconds", 0x03);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 20;
    g_Console.writeToBuffer(c, "Press <Space> to change character colour", 0x09);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 9;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x09);
}

void renderGame()
{
	renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
}

void renderMap()
{
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
	if (!renderMapAlready) {

		bool spawnPoint = false;

		//----------------THIS USES TIME AS A FACTOR IN THE RAND----------------
		srand(time(NULL));

		//----------------THE LOOP FOR THE OUTER RING AND MAKES UP THE SIZE OF THE MAZE----------------
		for (int j = 0; j <= y; j++)
		{
			//----------------THE LOOP INNER MAZE----------------
			for (int i = 0; i <= x; i++)
			{
				if (j == 0 || j == y)
				{
					//----------------THIS COUTS THE OUTER RING AS WELL----------------
					mapArray[i][j] = walls;
				}
				else
				{
					if (i == 0 || i == x)
					{
						//----------------THIS COUTS THE OUTER RING AS WELL----------------
						mapArray[i][j] = walls;
					}
					else
					{
						/*----------------THIS IS WHERE THE JUICY PART OF THE MAZE COMES IN----------------
						----------------USING RAND % 100 MAKES ALL THE NUMBER GENERATED LESS THAN 100----------------*/
						if (rand() % 100 > 65)
						{
							//----------------THIS GIVES THE FACTOR OF RANDOMNESS AND CREATES A MAZE----------------
							if (rand() % 100 < 5 && !spawnPoint)
							{
								//----------------THIS SETS THE SPAWN POINT----------------
								mapArray[i][j] = stair;
								XlocationX = i;
								XlocationY = j;
								spawnPoint = true;
							}
							else
							{
								//----------------THIS SETS THE WALLS----------------
								mapArray[i][j] = walls;
							}
						}
						else
						{
							//----------------THIS SETS THE EMPTY SPACES----------------
							mapArray[i][j] = floors;
						}

					}
				}
			}
		}
		mapArray[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y] = floors;
		renderMapAlready = true;
	}

	for (int j = 0; j <= y; j++)
	{
		c.Y = j;
		for (int i = 0; i <= x; i++)
		{
			c.X = i;
			if(mapArray[i][j] == stair)
				g_Console.writeToBuffer(c, mapArray[i][j], blackColor);
			else
				g_Console.writeToBuffer(c, mapArray[i][j], blackColor);


		}
	}

	//---------------------THE ECHO LOCATION VISION---------------------
	if (useEchoLocation) {

		for (int i = playerLocationY + radiusY; i >= playerLocationY - radiusY; i--)
		{
			if (radiusY >= 9) {
				c.X = XlocationX;
				c.Y = XlocationY;
				g_Console.writeToBuffer(c, mapArray[XlocationX][XlocationY]);
			}

			c.Y = i;
			for (int j = playerLocationX + radiusX; j >= playerLocationX - radiusX; j--)
			{
				c.X = j;

				if (mapArray[j][i] == floors)
					g_Console.writeToBuffer(c, mapArray[j][i], echoedFloor);

			}
		}


		if (g_dElapsedTime > timer + 1)
		{
			timer = (int)g_dElapsedTime;
			radiusX--;
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
		for (int j = g_sChar.m_cLocation.X + 1; j >= g_sChar.m_cLocation.X - 1; j--)
		{
			c.X = j;
			if (mapArray[j][i] == floors)
				g_Console.writeToBuffer(c, mapArray[j][i], floorColor);

			else if (mapArray[j][i] == stair)
				g_Console.writeToBuffer(c, mapArray[j][i]);

			else if (mapArray[j][i] == walls)
				g_Console.writeToBuffer(c, mapArray[j][i], wallColor);
		}
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
