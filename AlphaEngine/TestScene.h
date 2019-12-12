#pragma once
#include "GameScene.h"
#include "BlockManager.h"
#include "Script.h"
#include "Interpolator.h"

class TestScene : public GameScene
{
	char* _rawScript;

	std::string _errorMsg;
	BlockManager* _blockManager;
	StageData _stageData;

	ObjModel* _sign;
	ObjModel* _logo;
	ObjModel* _clear;
	float _logoScale;
	float _logoY;
	bool _drawLogo;
	
	ObjModel* _player;
	script::Script* _script;

	bool _isPlay = false;
	float _playerGo = 0.0f;
	float _playerTurn = 0.0f;
	std::vector<PlayerAction> _actions;
	std::vector<PlayerAction>::iterator _iter;
	Tween _goTween;
	Tween _turnTween;
	Tween* _curTween;
	bool _nextAction = false;
	Interpolator* _interpolator;
	glm::mat4 _playerMatrix;

	int _playerX;
	int _playerY;

	glm::vec<2, int> _dir;

	int _objectCount;

	static int _stageNumber;

public:
	TestScene();
	virtual ~TestScene();

	void Init() override;
	void Update() override;

	void PreRender() override;
	void Render() override;
	void PostRender() override;

	void Release() override;

	void PlayInit();
	void PlayUpdate();
	void PlayRelease();
};

