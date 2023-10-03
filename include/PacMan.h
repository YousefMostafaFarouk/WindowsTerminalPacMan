#pragma once
#include "Direction.h"

class PacMan
{

public:
	enum pacManChar
	{
		OPEN,
		CLOSE
	};

	enum pacManState
	{
		ALIVE,
		DEAD
	};

	int numberOfLives{ 3 };

	float xPos, yPos;
	float speed ;
	float ySpeed;
	char charLookUpTable[2] {'C','O'};
	pacManChar currentChar = OPEN;

	pacManState currentState = ALIVE;

	Direction direction = Direction::RIGHT;

	
	void Move();

	void Reset(float startingXPos, float startingYPos);

	PacMan(float startingXPos, float startingYPos, float pacManSpeed);
};

