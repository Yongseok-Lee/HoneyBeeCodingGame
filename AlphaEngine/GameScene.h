#pragma once
#include "string"
#include "vector"
#include "GameObject.h"
#include "System.h"

class GameScene : public System
{
protected:
	std::vector<GameObject*> _objectList;

public:
	GameScene();
	virtual ~GameScene();

	void Init() override;
	void Update() override;

	void PreRender() override;
	void Render() override;
	void PostRender() override;

	void Release() override;

	GameObject* FindGameObject(std::string name);
	void AddGameObject(GameObject* object);
	void RemoveGameObject(std::string name);
};

