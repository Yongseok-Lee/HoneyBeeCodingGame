#pragma once
#include "System.h"
#include "GameScene.h"
#include "Singleton.h"

class SceneManager final : public System, public Singleton<SceneManager>
{
	GameScene* _currentScene;

public:
	SceneManager();
	virtual ~SceneManager();

	void Init() override;
	void Update() override;
	void Render() override;
	void PostRender() override;
	void Release() override;

	void SceneChange(int sceneNum);
};

