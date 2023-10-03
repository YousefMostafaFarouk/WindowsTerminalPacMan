#pragma once
#include "Direction.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>

class Algorithm
{
public:
	int width, height;
	std::vector<char> map;

	Algorithm(int screenWidth, int screenHeight, std::vector<char>& generatedMap);


	// algorithms that takes a starting position and a target position and returns
	// the direction to go to based on the algorithm

	// Uses basic vector direction to determine the next place to go to
	Direction BaseGoTo(int xStart, int yStart, int xTarget, int yTarget);

	// Loops over the 4 direction that the ghost can take and then chooses the one which minimizes the distance between the starting position and the target
	Direction EucledianGoTo(int xStart, int yStart, int xTarget, int yTarget);

	// Uses the Breadth First Search algorithm to find the best route from the starting position to the target
	Direction BfsGoTo(int xStart, int yStart, int xTarget, int yTarget);

private:
	std::vector<int> FindAdjacentCells(int cell);

};

