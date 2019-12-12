#include "BlockManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>

/*
grass,
	honey,
	pond,
	pondLeaf,
	frog,
	piranhaPlant,
	decoBlock,
 */

static const char* BlockName[BlockTypeNum] =
{
	"grass.obj",
	"honeypot.obj",
	"Pond.obj",
	"LotusLeafInPond.obj",
	"Frog.obj",
	"PiranhaPlant.obj",
	"flower_ground.obj"
};

BlockManager::BlockManager() : GameObject("BlockManager")
{
	_blockScale = 0.2f;
	for (int i = 0; i < BlockTypeNum; ++i)
	{
		_models[i] = new ObjModel(BlockName[i]);
	}
}

BlockManager::~BlockManager()
{
	for (int i = 0; i < BlockTypeNum; ++i)
	{
		delete _models[i];
	}
}

void BlockManager::Init()
{
	for (int i = 0; i < BlockTypeNum; ++i)
	{
		_models[i]->Init();
	}

	bool find = false;

	for (int i = 0; i < _stageData.ySize; ++i)
	{	
		for (int j = 0; j < _stageData.xSize; ++j)
		{
			StageBlockType type = _stageData.mapData[i][j];
			if (type == Start)
			{
				_startX = j;
				_startY = i;
				find = true;
				break;
			}
		}
		if (find) break;
	}
}

void BlockManager::Update()
{
	
}

void BlockManager::Render()
{
	glm::mat4 offset(1.0f);
	glm::mat4 Z(1.0);
	
	offset = glm::translate(offset, glm::vec3(-(_startX * _blockScale), 0.0f, -(_startY * _blockScale)));
	for (int i = 0; i < _stageData.ySize; ++i)
	{
		glm::mat4 X(1.0f);
		for (int j = 0; j < _stageData.xSize; ++j)
		{
			glm::mat4 S(1.0f);
			S = glm::scale(S, glm::vec3(_blockScale));
			//T*S;
			StageBlockType type = _stageData.mapData[i][j];
			if (type != Null)
			{
				// draw block.
				switch (type)
				{
				case Pond:
					_models[pond]->Update(Z*X*offset*S);
					_models[pond]->Render();
					break;
				case PondFrog:
					_models[pondLeaf]->Update(Z*X*offset*S);
					_models[pondLeaf]->Render();
					{
						glm::mat4 Y(1.0f);
						Y = glm::translate(Y, glm::vec3(0, _blockScale, 0));
						_models[frog]->Update(Y*Z*X*offset*S);
						_models[frog]->Render();
					}
					break;
				case PiranhaPlant:
					_models[piranhaPlant]->Render();
					{
						glm::mat4 Y(1.0f);
						Y = glm::translate(Y, glm::vec3(0, _blockScale, 0));
						_models[piranhaPlant]->Update(Y*Z*X*offset*S);
						_models[piranhaPlant]->Render();
					}
					_models[grass]->Update(Z*X*offset*S);
					_models[grass]->Render();
					break;
				
				case Object: // draw object.
				{
					glm::mat4 Y(1.0f);
					Y = glm::translate(Y, glm::vec3(0, _blockScale, 0));
					_models[honey]->Update(Y*Z*X*offset*S);
					_models[honey]->Render();
				}
				case Start:
				case Default:
					_models[grass]->Update(Z*X*offset*S);
					_models[grass]->Render();
					break;
				}
			}
			X = glm::translate(X, glm::vec3(_blockScale, 0.0f, 0.0f));
		}
		Z = glm::translate(Z, glm::vec3(0.0f, 0.0f, _blockScale));
	}
}

void BlockManager::Release()
{
	for (int i = 0; i < BlockTypeNum; ++i)
	{
		_models[i]->Release();
	}
}

void BlockManager::SetStageData(StageData data)
{
	_stageData = data;
}

int BlockManager::GetStartX() const
{
	return _startX;
}

int BlockManager::GetStartY() const
{
	return _startY;
}

StageBlockType BlockManager::GetBlock(int x, int y)
{
	if (x >= _stageData.xSize || x < 0)
		return Null;
	if (y >= _stageData.ySize || y < 0)
		return Null;
	
	return _stageData.mapData[y][x];
}

void BlockManager::SetBlock(int x, int y, StageBlockType type)
{
	if (x >= _stageData.xSize || x < 0)
		return;
	if (y >= _stageData.ySize || y < 0)
		return;

	_stageData.mapData[y][x] = type;
}

