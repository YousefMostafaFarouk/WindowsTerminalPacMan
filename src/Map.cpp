#include "Map.h"
#include <random>

std::vector<char> Map::GenerateMap(MapInfo mapInfo)
{
	// Create a random number generator engine
	std::random_device rd;
	std::mt19937 gen(rd()); 

	// Define a range for the random number
	int min = 1;
	int max = 100;

	// Create a distribution for the random number within the specified range
	std::uniform_int_distribution<int> distribution(min, max);

	//The vector where the generated map is stored
	std::vector<char> generatedMap( mapInfo.screenWidth * mapInfo.screenHeight );

	//Places 1 in the parts where no tiles are supposed to be generated
	for (int i = 0;i < 2000;++i) generatedMap[i] = '1';


	//Loops over a block of the screen and places hashtags
	for (int x = mapInfo.xBorderStart - 1; x < mapInfo.xBorderEnd + 1; x++)
	{
		for (int y = mapInfo.yBorderStart - 1 ; y < mapInfo.yBorderEnd + 1 ; y++)
		{
			generatedMap[mapInfo.screenWidth * y + x] = '#';
		}
	}

	int blockWidth = 15;
	int blockHeight = 5;

	int screenWidthWithoutBorder = mapInfo.xBorderEnd - mapInfo.xBorderStart;
	int screenHeightWithoutBorder = mapInfo.yBorderEnd - mapInfo.yBorderStart;

	int prev_rand_num = -1;
	int randNumber = -1;

	//This for loop first splits the map into sections where we have (game width/block width) horizontal sections and then we have (game height / block height) vertical sections
	//it then places a random block in each section
	for (int i = 0; i < (screenWidthWithoutBorder / blockWidth)/2; i++)
	{
		for (int j = 0; j < (screenHeightWithoutBorder / blockHeight); j++)
		{
			//This is to prevent duplicate blocks
			while (randNumber % Map::allBlocks.size() == prev_rand_num % Map::allBlocks.size())
				randNumber = distribution(gen);
			prev_rand_num = randNumber;

			for (int y = 0; y < blockHeight; y++)
			{
				for (int x = 0; x < blockWidth; x++)
				{
					//Places the character of each block and then mirrors that character on the y axis
					char blockChar = Map::allBlocks[randNumber % Map::allBlocks.size()].tile[y][x];
					generatedMap[mapInfo.screenWidth * (mapInfo.yBorderStart + (blockHeight * j) + y) + (mapInfo.xBorderStart + (blockWidth * i) + x)] = blockChar;
					generatedMap[mapInfo.screenWidth * (mapInfo.yBorderStart + (blockHeight * j) + y) + (mapInfo.xBorderStart + (mapInfo.xBorderEnd - mapInfo.xBorderStart) -1
					- (blockWidth * i) - x)] = blockChar;
				}
			}
		}
	}

	int mapWidth = mapInfo.xBorderEnd - mapInfo.xBorderStart;
	int mapHeight = mapInfo.yBorderEnd - mapInfo.yBorderStart;

	int containerWidth = 4;
	int containerHeight = 2;

	int containerYStartPos = mapInfo.yBorderStart + mapHeight / 2 - containerHeight / 2;
	int containerXStartPos = mapInfo.xBorderStart + mapWidth / 2 - containerWidth / 2;

	//clears the middle part of the map for the ghost starting container to be placed
	for (int y = containerYStartPos - 1; y <= containerYStartPos + containerHeight + 2; y++)
	{
		for (int x = containerXStartPos - 2; x <= containerXStartPos + containerWidth + 2; x++)
		{
			generatedMap[mapInfo.screenWidth * y + x] = '.';
		}
	}

	//This loop places the ghosts' starting container	
	for (int y = containerYStartPos; y < containerYStartPos + containerHeight; y++)
	{
		for (int x = containerXStartPos; x<= containerXStartPos + containerWidth; x++)
		{
			if ( !(x == containerXStartPos || x == containerXStartPos + containerWidth) && y == containerYStartPos )
				generatedMap[mapInfo.screenWidth * y + x] = '.';
			else
				generatedMap[mapInfo.screenWidth * y + x] = '#';
		}
	}

	ghostContainerXPos = containerXStartPos;
	ghostContainerYPos = containerYStartPos;

	return generatedMap;
}