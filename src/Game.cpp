#include "Game.h"


void Game::startGame()
{
	ShowStartScreen();
	while (true)
	{
		// initialize the map information and generates a random map
		Map map;
		MapInfo mapInfo = { xBorderStart, yBorderStart, xBorderEnd, yBorderEnd, Game::SCREENWIDTH, Game::SCREENHEIGHT };
		std::vector<char> generatedMap = map.GenerateMap(mapInfo);
		PlaceObjectInMapRandomly('o', generatedMap);
		PlaceObjectInMapRandomly('*', generatedMap);

		PacMan pacMan(xBorderStart, yBorderStart, 1.0f);

		ghostSpawnXPos = map.ghostContainerXPos + 1;
		ghostSpawnYPos = map.ghostContainerYPos;

		Ghost::algoInfo ghost1AlgoInfo = { Ghost::EUCLEDIAN, -1, 0 };
		Ghost ghost1(ghostSpawnXPos, ghostSpawnYPos, 0.5f, FOREGROUND_RED, ghost1AlgoInfo);

		Ghost::algoInfo ghost2AlgoInfo = { Ghost::BASE, +1, 0 };
		Ghost ghost2(ghostSpawnXPos, ghostSpawnYPos, 0.5f, FOREGROUND_GREEN, ghost2AlgoInfo);

		Ghost::algoInfo ghost3AlgoInfo = { Ghost::BFS, 0, 0 };
		Ghost ghost3(ghostSpawnXPos, ghostSpawnYPos, 0.5f, FOREGROUND_BLUE | FOREGROUND_RED, ghost3AlgoInfo);

		Ghost::algoInfo ghost4AlgoInfo = { Ghost::EUCLEDIAN, +2, +1 };
		Ghost ghost4(ghostSpawnXPos, ghostSpawnYPos, 0.5f, FOREGROUND_BLUE | FOREGROUND_INTENSITY, ghost4AlgoInfo);

		std::vector<Ghost*> ghosts{&ghost1, &ghost2, &ghost3, &ghost4};

		Algorithm gameAlgorithm(SCREENWIDTH, SCREENHEIGHT, generatedMap);

		bool cherryEaten = false;
		bool powerUpEaten = false;

		std::chrono::system_clock::time_point cherryTimer = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point powerUpTimer = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point ghostRunningTimer = std::chrono::system_clock::now();

		WaitToReady(pacMan, generatedMap, yBorderStart, ghosts);
		while (!gameEnd) {
			ClearScreen();

			//Gets the direction pressed by the player
			GetPacManDirection(pacMan.direction);

			//Moves pacman in the direction pressed
			pacMan.Move();

			//checks if pacman collided with a wall and then reverts its position so that it isn't inside a wall
			CheckWallCollision(pacMan.xPos, pacMan.yPos, pacMan.speed, pacMan.direction, generatedMap);

			UpdateGhostState(ghosts, pacMan, ghostRunningTimer);

			//moves the ghosts based on each ghost's algorithm and state
			MoveGhostsUsingAlgorithm(ghosts, gameAlgorithm, pacMan);

			//checks if a ghost collided with a wall and then reverts its position so that it isn't inside a wall
			for (auto ghost : ghosts)
				CheckWallCollision(ghost->xPos, ghost->yPos, ghost->speed, ghost->direction, generatedMap);

			UpdateGhostState(ghosts, pacMan, ghostRunningTimer);

			// decrease number of pacman's lives if ghost eat pacman
			if (pacMan.currentState == PacMan::DEAD)
			{
				pacMan.numberOfLives--;
				pacMan.Reset(xBorderStart, yBorderStart);
				for (auto& ghost : ghosts)
				{
					ghost->Reset(ghostSpawnXPos, ghostSpawnYPos);
				}
				if (!pacMan.numberOfLives)
				{
					gameEnd = true;
				}
				else
					WaitToReady(pacMan, generatedMap, yBorderStart, ghosts);
			}

			if (gameEnd)
				break;

			// Checks if a power up has been eaten and how much time has passed since it was eaten
			// If enough time has passed it then spawns a power up in a random position 
			if (powerUpEaten && std::chrono::system_clock::now() - powerUpTimer >= std::chrono::milliseconds(15000))
			{
				PlaceObjectInMapRandomly('o', generatedMap);
				powerUpEaten = false;
			}

			// Same logic that was done to the power up is done here to the cherry
			if (cherryEaten && std::chrono::system_clock::now() - cherryTimer >= std::chrono::milliseconds(8000))
			{
				PlaceObjectInMapRandomly('*', generatedMap);
				cherryEaten = false;
			}

			// If pacman eats a power up then updates the states of the ghosts and sets a power up spawn timer and sets powerUpEaten to true
			// Also starts the timer of how long the ghosts are going to be running for 
			if (generatedMap[(int)pacMan.yPos * SCREENWIDTH + (int)pacMan.xPos] == 'o')
			{
				generatedMap[(int)pacMan.yPos * SCREENWIDTH + (int)pacMan.xPos] = ' ';

				for (auto ghost : ghosts)
				{
					ghost->currentState = ghost->RUNNING;
				}

				powerUpEaten = true;
				ghostRunningTimer = std::chrono::system_clock::now();
				powerUpTimer = std::chrono::system_clock::now();
			}

			// If pacman eats a cherry then adds the score points and sets a cherry spawn timer and sets the cherryEaten to true
			if (generatedMap[(int)pacMan.yPos * SCREENWIDTH + (int)pacMan.xPos] == '*')
			{
				generatedMap[(int)pacMan.yPos * SCREENWIDTH + (int)pacMan.xPos] = ' ';
				score += 50; // This is the value that determines how much the score should be increased by if pacman eats a power up

				cherryEaten = true;
				cherryTimer = std::chrono::system_clock::now();
			}

			// make pacman eat dots in map
			if (generatedMap[(int)pacMan.yPos * SCREENWIDTH + (int)pacMan.xPos] == '.')
			{
				generatedMap[(int)pacMan.yPos * SCREENWIDTH + (int)pacMan.xPos] = ' ';
				score++;
			}

			PlaceIntoScreenBuffer(pacMan, generatedMap, ghosts);

			ShowScore(score, highScore);

			ShowNumberOfLives(pacMan.numberOfLives);
			WriteConsoleOutput(hConsole, screenBuffer, dwBufferSize, { 0,0 }, &windowSize);
			
			//Checks if all points have been eaten and then breaks
			if (allPointsEaten)
				break;

		}
		GameOver();
		while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);
	}
}

void Game::GetPacManDirection(Direction& inputDirection)
{
	auto time1 = std::chrono::system_clock::now();
	while ( (std::chrono::system_clock::now() - time1) < std::chrono::milliseconds(100) )
	{
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			inputDirection = Direction::LEFT;

		else if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			inputDirection = Direction::RIGHT;

		else if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
			inputDirection = Direction::UP;

		else if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
			inputDirection = Direction::DOWN;
	}
}

Game::Game(int screenWidth, int screenHeight)
{
	SCREENWIDTH = screenWidth;
	SCREENHEIGHT = screenHeight;

	highScore = GetHighScore(0);

	xBorderStart = 45;
	xBorderEnd = 75;
	yBorderStart = 10;
	yBorderEnd = 20;

	screenBuffer = new _CHAR_INFO[SCREENHEIGHT * SCREENWIDTH];

	consoleWindow = GetConsoleWindow();
	dwBufferSize.X = SCREENWIDTH;
	dwBufferSize.Y = SCREENHEIGHT;

	style = GetWindowLong(consoleWindow, GWL_STYLE);
	style &= ~WS_MAXIMIZEBOX; // Disable maximize box
	style &= ~WS_SIZEBOX;     // Disable resize box

	SetWindowLong(consoleWindow, GWL_STYLE, style);
	windowSize = { 0, 0, static_cast<short>(SCREENWIDTH), static_cast<short>(SCREENHEIGHT) };
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	dwBytesWritten = 0;
};

void Game::ClearScreen()
{
	for (int i = 0; i < SCREENHEIGHT * SCREENWIDTH; i++)
	{
		screenBuffer[i].Char.UnicodeChar = ' ';
		screenBuffer[i].Attributes = FOREGROUND_BLUE;
	}

}

void Game::PlaceIntoScreenBuffer(const PacMan& pacman, std::vector<char>& generatedMap,std::vector<Ghost*>& ghosts)
{
	int numberOfPoints{};

	// Places the map into the screen buffer
	for (int i = 0; i < Game::SCREENHEIGHT * Game::SCREENWIDTH; i++)
	{
		// This if statment is for coloring the dots and also for checking if all dots have been eaten
		if (generatedMap[i] == '.')
		{
			numberOfPoints++;
			screenBuffer[i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		}

		// The if statment is so that it doesn't add characters that are outside the border
		if (generatedMap[i] != '1')
			screenBuffer[i].Char.UnicodeChar = generatedMap[i];
	

		// This if statment is for coloring power ups
		if (generatedMap[i] == 'o')
			screenBuffer[i].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

		// This if statment is for coloring cherries
		if (generatedMap[i] == '*')
			screenBuffer[i].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
	}

	//Places PacMan into the screen buffer
	screenBuffer[ (int)(pacman.yPos) * SCREENWIDTH + (int)(pacman.xPos) ].Char.UnicodeChar = pacman.charLookUpTable[pacman.currentChar];
	screenBuffer[ (int)(pacman.yPos) * SCREENWIDTH + (int)(pacman.xPos) ].Attributes = FOREGROUND_RED | FOREGROUND_GREEN ;

	//Places the ghosts into the screen buffer
	for (int i = 0; i < ghosts.size(); i++)
	{
		screenBuffer[ (int)(ghosts[i]->yPos) * SCREENWIDTH + (int)(ghosts[i]->xPos) ].Char.UnicodeChar = ghosts[i]->charLookUpTable[ghosts[i]->currentState];
		screenBuffer[ (int)(ghosts[i]->yPos) * SCREENWIDTH + (int)(ghosts[i]->xPos) ].Attributes = ghosts[i]->color;
	}

	// Checks if all points have been eaten and sets the allPointsEaten variable to true
	if (numberOfPoints == 0)
		allPointsEaten = true;
}

void Game::CheckWallCollision(float& xEntityPos, float& yEntityPos,float entitySpeed , Direction direction, const std::vector<char>& generatedMap)
{
	if (generatedMap[SCREENWIDTH * (int)yEntityPos + (int)xEntityPos] == '#' ||
	yEntityPos > yBorderEnd || yEntityPos < yBorderStart
	|| xEntityPos > xBorderEnd || xEntityPos < xBorderStart)
	{
		switch (direction)
		{
		case Direction::UP:
			yEntityPos += entitySpeed;
			break;

		case Direction::DOWN:
			yEntityPos -= entitySpeed;
			break;

		case Direction::LEFT:
			xEntityPos += entitySpeed;
			break;

		case Direction::RIGHT:
			xEntityPos -= entitySpeed;
			break;

		default:
			break;
		}
	}
}

void Game::ShowScore(int score, int highScore)
{
	std::string number = std::to_string(score);
	std::string scoreString = "Score = " + number;
	int displayOffset = SCREENWIDTH / 2 - scoreString.size() / 2;

	//For loop to display the score
	for (int i = 0; i < scoreString.size(); i++)
	{
		screenBuffer[i + displayOffset].Char.UnicodeChar = scoreString[i];
		screenBuffer[i + displayOffset].Attributes = FOREGROUND_RED | FOREGROUND_GREEN ;
	}

	std::string highScoreNumber = std::to_string(highScore);
	std::string highScoreString = "High Score = " + highScoreNumber;

	//For loop to display the high score
	displayOffset = 1;
	for (int i = 0; i < highScoreString.size(); i++)
	{
		screenBuffer[i + displayOffset].Char.UnicodeChar = highScoreString[i];
		screenBuffer[i + displayOffset].Attributes = FOREGROUND_RED | FOREGROUND_GREEN ;
	}

	
}

void Game::ShowNumberOfLives(int numberOfLives)
{
	std::string numberOfLivesString = "NUMBER OF LIVES = " + std::to_string(numberOfLives);
	PrintString(numberOfLivesString, 0, SCREENHEIGHT - 1, FOREGROUND_RED | FOREGROUND_GREEN);
}

int Game::GetHighScore(int score)
{
	std::fstream file;
	std::string line;
	int highScore{};
	file.open("HighScore.txt", std::ios::in);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			highScore = stoi(line);

		}
		file.close();

		if (score > highScore)
		{
			file.open("HighScore.txt", std::ios::out);

			file << score;

			file.close();

			return score;

		}

		return highScore;
	}
	else
	{
		file.close();  // Close the failed open attempt
		file.open("HighScore.txt", std::ios::out);  // Create the file
		file << score;  // Write "0" to the empty file
		file.close();
	}
}

void Game::MoveGhostsUsingAlgorithm(std::vector<Ghost*>& ghosts, Algorithm& gameAlgorithm, const PacMan& pacMan) {
	for (Ghost* ghost : ghosts) 
	{
		if (ghost->currentState == Ghost::ALIVE) 
		{
			switch (ghost->algorithmInfo.algorithmType) 
			{
			case Ghost::BFS:
				ghost->direction = gameAlgorithm.BfsGoTo(ghost->xPos, ghost->yPos, pacMan.xPos + ghost->algorithmInfo.xTargetOffset, pacMan.yPos + ghost->algorithmInfo.yTargetOffset);
				break;
			case Ghost::BASE:
				ghost->direction = gameAlgorithm.BaseGoTo(ghost->xPos, ghost->yPos, pacMan.xPos + ghost->algorithmInfo.xTargetOffset, pacMan.yPos + ghost->algorithmInfo.yTargetOffset);
				break;
			case Ghost::EUCLEDIAN:
				ghost->direction = gameAlgorithm.EucledianGoTo(ghost->xPos, ghost->yPos, pacMan.xPos + ghost->algorithmInfo.xTargetOffset, pacMan.yPos + ghost->algorithmInfo.yTargetOffset);
				break;
			default:
				break;
			}
		}
		//running needs to be modified
		else if (ghost->currentState == Ghost::RUNNING)
		{
			ghost->direction = gameAlgorithm.BaseGoTo(pacMan.xPos - 2, pacMan.yPos, ghost->xPos, ghost->yPos);
		}
		else 
		{
			ghost->direction = gameAlgorithm.BfsGoTo(ghost->xPos, ghost->yPos, ghostSpawnXPos, ghostSpawnYPos);
		}

		ghost->Move();
	}
}

void Game::UpdateGhostState(std::vector<Ghost*>& ghosts, PacMan& pacMan, std::chrono::system_clock::time_point ghostRunningTimer) 
{
	for (auto ghost : ghosts) 
	{
		if ( (int)(ghost->xPos) == (int)(pacMan.xPos) && (int)(ghost->yPos) == (int)(pacMan.yPos) ) 
		{
			if (ghost->currentState == Ghost::ALIVE) 
			{
				pacMan.currentState = PacMan::DEAD;
			}

			if (ghost->currentState == Ghost::RUNNING) 
			{
				score += 50;
				ghost->currentState = Ghost::DEAD;
			}
		}

		// If enough time passes after the play eats the power up return the ghosts back to their normal state
		if (ghost->currentState == Ghost::RUNNING && ( std::chrono::system_clock::now() - ghostRunningTimer >= std::chrono::milliseconds(8000) ) )
			ghost->currentState = Ghost::ALIVE;

		if (ghost->currentState == Ghost::DEAD && (int)ghost->xPos == ghostSpawnXPos && (int)ghost->yPos == ghostSpawnYPos) 
			ghost->currentState = Ghost::ALIVE;
	}
}

void Game::PlaceObjectInMapRandomly(char objectChar, std::vector<char>& generatedMap)
{
	// Create a random number generator engine
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define a range for the random number
	int min = 1;
	int max = 5000;

	// Create a distribution for the random number within the specified range
	std::uniform_int_distribution<int> distribution(min, max);

	int randomNumber = distribution(gen) % ( ( xBorderEnd - xBorderStart ) * ( yBorderEnd - yBorderStart ) ) * 0.75;

	for ( int x = xBorderStart; x < xBorderEnd; x++ )
	{
		for (int y = yBorderStart; y < yBorderEnd; y++)
		{
			if (randomNumber <= 0 && generatedMap[SCREENWIDTH * y + x] != '#')
			{
				generatedMap[SCREENWIDTH * y + x] = objectChar;
				return;
			}
			randomNumber--;
		}
	}
}

void Game::GameOver()
{
	ClearScreen();

	std::string playAgainString = "PRESS SPACE TO PLAY AGAIN ";

	int yDisplayOffset = SCREENHEIGHT / 2 - 3;
	if (!allPointsEaten)
	{
		// Show gameover String
		std::string gameOverString = "GAME OVER";
		PrintString(gameOverString, SCREENWIDTH / 2 - playAgainString.size() / 2, yDisplayOffset, FOREGROUND_RED | FOREGROUND_GREEN);
		yDisplayOffset++;
	}

	else
	{
		std::string youWonString = "YOU WON!";
		PrintString(youWonString, SCREENWIDTH / 2 - playAgainString.size() / 2, yDisplayOffset, FOREGROUND_RED | FOREGROUND_GREEN);
		yDisplayOffset++;
	}

	// show your score in this game
	std::string number = std::to_string(score);
	std::string scoreString = "Score = " + number;
	PrintString(scoreString, SCREENWIDTH / 2 - playAgainString.size() / 2, yDisplayOffset, FOREGROUND_RED | FOREGROUND_GREEN);
	yDisplayOffset++;

	// update high score and print new high score;
	if (score > highScore)
	{
		GetHighScore(score);
		std::string highScoreString = "NEW HIGH SCORE = " + number + " !!";

		PrintString(highScoreString, SCREENWIDTH / 2 - playAgainString.size() / 2, yDisplayOffset, FOREGROUND_RED | FOREGROUND_GREEN);
		yDisplayOffset++;
	}

	PrintString(playAgainString, SCREENWIDTH / 2 - playAgainString.size() / 2, yDisplayOffset, FOREGROUND_RED | FOREGROUND_GREEN);


	WriteConsoleOutput(hConsole, screenBuffer, dwBufferSize, { 0,0 }, &windowSize);
	
	// Reset the value of score
	gameEnd = false;

	if (!allPointsEaten)
		score = 0;
	else
		allPointsEaten = false;

}

void Game::WaitToReady(const PacMan& pacman, std::vector<char>& generatedMap, int yBorderStart, std::vector<Ghost*>& ghosts)
{
	ClearScreen();
	PlaceIntoScreenBuffer(pacman, generatedMap, ghosts);

	std::string readyString = "READY";
	int xDisplayOffset = SCREENWIDTH / 2 - readyString.size() / 2;
	int yDisplayOffset = yBorderStart - 2;

	// Makes the "Ready" text blink
	auto time1 = std::chrono::system_clock::now();
	while ((std::chrono::system_clock::now() - time1) < std::chrono::milliseconds(3000))
	{
		auto time2 = std::chrono::system_clock::now();
		PrintString(readyString, xDisplayOffset, yDisplayOffset, FOREGROUND_RED | FOREGROUND_GREEN);
		WriteConsoleOutput(hConsole, screenBuffer, dwBufferSize, { 0,0 }, &windowSize);
		while ((std::chrono::system_clock::now() - time2) < std::chrono::milliseconds(500));

		time2 = std::chrono::system_clock::now();
		PrintString(readyString, xDisplayOffset, yDisplayOffset, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		WriteConsoleOutput(hConsole, screenBuffer, dwBufferSize, { 0,0 }, &windowSize);
		while ((std::chrono::system_clock::now() - time2) < std::chrono::milliseconds(500));
	}
}

void Game::PrintString(std::string word, int xDisplayOffset, int yDisplayOffset,int color)
{
	for (int i = 0; i < word.size(); i++)
	{
		screenBuffer[i + xDisplayOffset + SCREENWIDTH * yDisplayOffset].Char.UnicodeChar = word[i];
		screenBuffer[i + xDisplayOffset + SCREENWIDTH * yDisplayOffset].Attributes = color;
	}
}

void Game::ShowStartScreen()
{
	ClearScreen();
	for (int y = 0; y < SCREENHEIGHT / 2; y++)
	{
		for (int x = 0; x < SCREENWIDTH  ; x++)
		{
			screenBuffer[y * SCREENWIDTH + x].Char.UnicodeChar = ' ';
			screenBuffer[y * SCREENWIDTH + x].Attributes = BACKGROUND_GREEN | BACKGROUND_RED;

			screenBuffer[ (SCREENHEIGHT-y-1) * SCREENWIDTH + (SCREENWIDTH-x-1)].Char.UnicodeChar = ' ';
			screenBuffer[ (SCREENHEIGHT - y-1) * SCREENWIDTH + (SCREENWIDTH - x-1)].Attributes = BACKGROUND_GREEN | BACKGROUND_RED ;

			WriteConsoleOutput(hConsole, screenBuffer, dwBufferSize, { 0,0 }, &windowSize);
			
		}
		Sleep(100);
	}

	std::string byWhoString = "Windows Terminal Pacman by YousefMostafaFarouk and AhmedElsayed2004 and noor";
	PrintString(byWhoString, SCREENWIDTH / 2 - byWhoString.size() / 2, SCREENHEIGHT / 2 - 1, BACKGROUND_GREEN | BACKGROUND_RED);

	std::string pressSpaceToPlayString = "PRESS SPACE TO START PLAYING";
	PrintString(pressSpaceToPlayString, SCREENWIDTH / 2 - pressSpaceToPlayString.size() / 2, SCREENHEIGHT / 2 - 2, BACKGROUND_GREEN | BACKGROUND_RED );

	WriteConsoleOutput(hConsole, screenBuffer, dwBufferSize, { 0,0 }, &windowSize);
	while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);
}