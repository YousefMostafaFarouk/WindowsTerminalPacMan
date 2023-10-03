#pragma once
#include"Direction.h";

class Ghost
{
public:
	enum ghostState
	{
		ALIVE,
		RUNNING,
		DEAD
	};

	enum algorithms
	{
		BFS,
		BASE,
		EUCLEDIAN
	};

	// Information needed for functions updating the ghost state
	struct algoInfo
	{
		algorithms algorithmType;
		int xTargetOffset;
		int yTargetOffset;
	};

	char charLookUpTable[3]{ 'A','R','X'};
	ghostState currentState = ALIVE;


	int color;

	float xPos, yPos;
	int algorithmXTargetPos, algorithmYTargetPos;
	float speed;
	float ySpeed;
	algoInfo algorithmInfo;
	Direction direction = Direction::UP;

	void Move();

	void Reset(int startingXPos, int startingYPos);

	Ghost( int startingXPos, int startingYPos, float ghostSpeed, int ghostColor, algoInfo algorithmInformation);

};
