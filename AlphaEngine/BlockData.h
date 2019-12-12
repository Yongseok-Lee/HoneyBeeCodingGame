#pragma once
#include <vector>

enum BlockType
{
	grass,
	honey,
	pond,
	pondLeaf,
	frog,
	piranhaPlant,
	decoBlock,
	BlockTypeNum
};

enum StageBlockType
{
	Null,
	Pond,
	PondFrog,
	PiranhaPlant,
	Default,
	Object,
	Start
};

struct StageData
{
	int stageNumber;
	int xSize, ySize;
	std::vector<std::vector<StageBlockType> > mapData;

	int GetObjectCount()
	{
		int count = 0;
		for (int i = 0; i < ySize; ++i)
		{
			for (int j = 0; j < xSize; ++j)
			{
				if (mapData[i][j] == Object) count++;
			}
		}
		return count;
	}
};
