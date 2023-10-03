#include "Ghost.h"

void Ghost::Move()
{
	switch (direction)
	{
	case Direction::UP:
		this->yPos -= this->ySpeed;
		break;

	case Direction::DOWN:
		this->yPos += this->ySpeed;
		break;

	case Direction::LEFT:
		this->xPos -= this->speed;
		break;

	case Direction::RIGHT:
		this->xPos += this->speed;
		break;
	}
}

void Ghost::Reset(int startingXPos, int startingYPos)
{
	this->xPos = startingXPos;
	this->yPos = startingYPos;
	this->currentState = ALIVE;
}

Ghost::Ghost(int startingXPos, int startingYPos, float ghostSpeed, int ghostColor, algoInfo algorithmInformation)
{
	xPos = startingXPos;
	yPos = startingYPos;
	speed = ghostSpeed;
	ySpeed = speed / 2;

	color = ghostColor;

	algorithmInfo = algorithmInformation;
}
