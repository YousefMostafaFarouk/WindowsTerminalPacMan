#include "Algorithm.h"

Direction Algorithm::BaseGoTo(int xStart, int yStart, int xTarget, int yTarget) 
{
	int xDir = xTarget - xStart;
	int yDir = yTarget - yStart;

	if (xDir == 0)
	{
		if(yDir>0) 
			return Direction::DOWN;

		return Direction::UP;
	}
	if (yDir == 0)
	{
		if (xDir > 0)
			return Direction::RIGHT;

		return Direction::LEFT;
	}
	if (xDir > 0 && yDir > 0)
	{
		if (map[yStart * width + (xStart + 1)] != '#') 
			return Direction::RIGHT;

		return Direction::DOWN;
	}
	if (xDir > 0 && yDir < 0)
	{
		if (map[yStart * width + (xStart + 1)] != '#')
			return Direction::RIGHT;

		return Direction::UP;
	}
	if (xDir < 0 && yDir > 0)
	{
		if (map[yStart * width + (xStart - 1)] != '#')
			return Direction::LEFT;
		
		return Direction::DOWN;
	}
	if (xDir < 0 && yDir < 0)
	{
		if (map[yStart * width + (xStart - 1)] != '#')
			return Direction::LEFT;
		
		return Direction::UP;
	}

}

Direction Algorithm::EucledianGoTo(int xStart, int yStart, int xTarget, int yTarget)
{
	float upDistance{}, downDistance{}, rightDistance{}, leftDistance{};

	upDistance = sqrt( (yStart - 1 - yTarget) * (yStart - 1 - yTarget) + (xStart - xTarget ) * (xStart - xTarget) );
	downDistance = sqrt( (yStart + 1 - yTarget) * (yStart + 1 - yTarget) + (xStart - xTarget) * (xStart - xTarget) );
	rightDistance = sqrt( (yStart - yTarget) * (yStart - yTarget) + (xStart + 1 - xTarget) * (xStart +1  - xTarget) );
	leftDistance = sqrt( (yStart - yTarget) * (yStart - yTarget) + (xStart - 1 - xTarget) * (xStart - 1 - xTarget) );

	std::vector<std::pair<float, Direction>> distances{ {upDistance,Direction::UP}, {downDistance,Direction::DOWN },
	{rightDistance,Direction::RIGHT}, {leftDistance,Direction::LEFT} };

	std::sort(distances.begin(), distances.end());

	for (int i = 0; i < 4; i ++)
	{
		switch (distances[i].second)
		{
		case Direction::UP:
			if (map[(yStart - 1) * width + xStart] != '#')
				return Direction::UP;
			break;

		case Direction::DOWN:
			if (map[(yStart + 1) * width + xStart] != '#')
				return Direction::DOWN;
			break;

		case Direction::RIGHT:
			if (map[(yStart) * width + xStart + 1] != '#')
				return Direction::RIGHT;
			break;

		case Direction::LEFT:
			if (map[(yStart) * width + xStart - 1] != '#')
				return Direction::LEFT;
			break;
		}
	}
}

Direction Algorithm::BfsGoTo(int xStart, int yStart, int xTarget, int yTarget)
{
	int start = yStart * width + xStart;
	int target = yTarget * width + xTarget;
	std::vector<int> parentOfEachNode(width*height);

	// Creates a vector that has the distance between the node and its parent
	std::vector<int> distanceOfEachNode(width * height, INT_MAX);

	std::queue<int> queue;
	
	queue.push(start);
	parentOfEachNode[start] = { -1 };
	distanceOfEachNode[start] = 0;

	while (!queue.empty()) 
	{
		int front = queue.front();
		queue.pop();

		std::vector<int> adjacentCells = FindAdjacentCells(front);

		// Loops over all the adjacent cells of a node
		for (auto cell : adjacentCells)
		{
			// Updates the parent of a cell only if the new parent is closer to the node than the older parent
			if (map[cell] != '#' && distanceOfEachNode[cell] > distanceOfEachNode[front] + 1)
			{
				distanceOfEachNode[cell] = distanceOfEachNode[front] + 1;
				queue.push(cell);
				parentOfEachNode[cell] = front;
			}
		}
	}

	// This checks the parent of the target node and the backtracks all the way to the start node to find the best next move 
	int nextNode = target;
	while (parentOfEachNode[nextNode] != start && parentOfEachNode[nextNode]!=-1)
		nextNode = parentOfEachNode[nextNode];
	
	if (nextNode == start + 1) 
	{
		return Direction::RIGHT;
	}
	else if (nextNode == start - 1) 
	{
		return Direction::LEFT;
	}
	else if (nextNode == start + width) 
	{
		return Direction::DOWN;
	}
	else if (nextNode == start - width) 
	{
		return Direction::UP;
	}
	else
	{
		return Direction::UP;
	}
}
std::vector<int> Algorithm::FindAdjacentCells(int cell)
{
	std::vector<int> adjacentCells;
	adjacentCells.push_back(cell - 1);
	adjacentCells.push_back(cell + 1);
	adjacentCells.push_back(cell - width);
	adjacentCells.push_back(cell + width);
	return adjacentCells;
}
Algorithm::Algorithm(int screenWidth,int screenHeight, std::vector<char>& generatedMap)
{
	width = screenWidth;
	height = screenHeight;
	map = generatedMap;
}