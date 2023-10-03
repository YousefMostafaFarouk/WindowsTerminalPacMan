#pragma once
#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <deque>
#include <thread>
#include <chrono>
#include <Windows.h>
#include "pacman.h"
#include "Map.h"
#include "Ghost.h"
#include "Algorithm.h"
#include <fstream>


class Game
{
public:
	int SCREENWIDTH;
	int SCREENHEIGHT;

	Game(int screenWidth, int screenHeight);

	// Starts the game main loop
	void startGame();

private:
	int score{ 0 };
	int highScore;

	bool gameEnd = false;
	bool allPointsEaten = false;
	bool soundPlaying = false;
	bool soundStopped = false;

	// The border where the actual game is anything around it is reserved for score /other things
	int xBorderStart;
	int xBorderEnd;
	int yBorderStart;
	int yBorderEnd;

	int ghostSpawnXPos;
	int ghostSpawnYPos;

	// All of these are things needed for the windows terminal
	_CHAR_INFO* screenBuffer;
	COORD dwBufferSize;
	DWORD style;
	SMALL_RECT windowSize;
	HANDLE hConsole;
	DWORD dwBytesWritten;
	HWND consoleWindow;

	void ClearScreen(); 

	void PlaceIntoScreenBuffer(const PacMan& pacman, std::vector<char>& generatedMap, std::vector<Ghost*>& ghosts);

	// Checks if Pacman or a Ghost are inside of wall then reverts their position
	void CheckWallCollision(float& xEntityPos, float& yEntityPos, float entitySpeed, Direction direction, const std::vector<char>& generatedMap);

	void ShowScore(int score, int highScore);

	void ShowNumberOfLives(int numberOfLives);

	// Gets pacman's direction through user input
	void GetPacManDirection(Direction& direction);

	int GetHighScore(int score);

	// Moves the ghost using its specific algorithm and also according to a ghost's state
	void MoveGhostsUsingAlgorithm(std::vector<Ghost*>& ghosts, Algorithm& gameAlgorithm, const PacMan& pacMan);

	void UpdateGhostState(std::vector<Ghost*>& ghosts, PacMan& pacMan, std::chrono::system_clock::time_point ghostRunningTimer);

	void PlaceObjectInMapRandomly(char objectChar, std::vector<char>& generatedMap);

	void GameOver();

	void WaitToReady(const PacMan& pacman, std::vector<char>& generatedMap, int yBorderStart, std::vector<Ghost*>& ghosts);

	void PrintString(std::string word, int xDisplayOffset, int yDisplayOffset,int color);

	void ShowStartScreen();
};
