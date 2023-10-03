#pragma once
#include <string>
#include <random>
#include <vector>
#include <utility> 

struct MapInfo
{
	int xBorderStart; 
	int yBorderStart; 
	int xBorderEnd;
	int yBorderEnd;
	int screenWidth;
	int screenHeight;
};

class Map
{
public:
	// The location of the ghost's starting container
	int ghostContainerXPos;
	int ghostContainerYPos;

	std::vector<char> GenerateMap(MapInfo mapInfo);

private:
	struct Block
	{
		std::string tile[5];
	};

	Block block1 =
	{
		"............###",
		".#.....#......#",
		".#...#####.....",
		"...#...#......#",
		"...#.......####"

	};

	Block block2 =
	{
		".............##",
		"##............#",
		"....#######....",
		"##..#.....#...#",
		".............##"
	};

	Block block3 =
	{
		"..........##..#",
		"##.....#......#",
		".......#.......",
		"##.#########..#",
		"..............#"
	};

	Block block4 =
	{
		"...........####",
		"#.....###.....#",
		"......###......",
		"#..#..###......",
		"...#.......####"
	};

	Block block5 =
	{
		"............###",
		"#...#.....#....",
		"....#######....",
		"....#.....#...#",
		".##....#....###"
	};

	Block block6 =
	{
		"..#..##....##..",
		"..#..#.....##..",
		".....#.........",
		"...#.##...#....",
		"...#......#...."
	};

	std::vector<Block> allBlocks = { block1,block2,block3,block4,block5,block6 };
};

