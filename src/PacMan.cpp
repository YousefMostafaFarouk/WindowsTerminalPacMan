#include "pacman.h"

// This function takes a direction and moves pacman in that direction according to pacman's speed
void PacMan::Move() 
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

void PacMan::Reset(float startingXPos, float startingYPos) {
	this->xPos = startingXPos;
	this->yPos = startingYPos;
	this->currentState = ALIVE;
	this->direction = Direction::RIGHT;
}

PacMan::PacMan(float startingXPos, float startingYPos, float pacManSpeed)
{
	xPos = startingXPos;
	yPos = startingYPos;
	speed = pacManSpeed;
	ySpeed = speed / 2;
}