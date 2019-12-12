#pragma once
#include "BlockData.h"
#include "GameObject.h"
#include "ObjModel.h"

class BlockManager : public GameObject
{
	ObjModel* _models[BlockTypeNum];
	StageData _stageData;

	float _blockScale;

	int _startX;
	int _startY;
	
public:
	BlockManager();
	virtual ~BlockManager();

	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;

	void SetStageData(StageData data);
	int GetStartX() const;
	int GetStartY() const;

	StageBlockType GetBlock(int x, int y);
	void SetBlock(int x, int y, StageBlockType type);
};

