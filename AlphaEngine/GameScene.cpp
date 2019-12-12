#include "GameScene.h"
#include "algorithm"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	std::for_each(_objectList.begin(), _objectList.end(), [](GameObject* gameObject)
	{
		gameObject->Init();
	});
}

void GameScene::Update()
{
	std::for_each(_objectList.begin(), _objectList.end(), [](GameObject* gameObject)
	{
		if (gameObject->GetActive())
			gameObject->Update();
	});
}

void GameScene::PreRender()
{
	
}

void GameScene::Render()
{
	std::for_each(_objectList.begin(), _objectList.end(), [](GameObject* gameObject)
	{
		if (gameObject->GetActive())
			gameObject->Render();
	});
}

void GameScene::PostRender()
{
	
}

void GameScene::Release()
{
	std::for_each(_objectList.begin(), _objectList.end(), [](GameObject* gameObject)
	{
		gameObject->Release();
	});
	_objectList.clear();
}

GameObject* GameScene::FindGameObject(std::string name)
{
	auto gameObject = std::find_if(_objectList.begin(), _objectList.end(), [&name](GameObject* gameObject)
	{
		return gameObject->GetName() == name;
	});
	if (gameObject != _objectList.end())
		return *gameObject;
	return nullptr;
}

void GameScene::AddGameObject(GameObject* object)
{
	_objectList.push_back(object);
}

void GameScene::RemoveGameObject(std::string name)
{
	std::for_each(_objectList.begin(), _objectList.end(), [&name](GameObject* gameObject)
	{
		if (gameObject->GetName() == name)
			gameObject->SetActive(false);
	});
	/*std::remove_if(_objectList.begin(), _objectList.end(), [&name](GameObject* gameObject)
	{
		return gameObject->GetName() == name;
	});*/
}
