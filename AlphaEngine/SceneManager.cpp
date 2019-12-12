#include "SceneManager.h"
#include "TestScene.h"

SceneManager::SceneManager() : System(), Singleton<SceneManager>(), _currentScene(nullptr)
{
	_currentScene = new TestScene();
}

SceneManager::~SceneManager()
{
	if (_currentScene)
	{
		_currentScene->Release();
		delete _currentScene;
	}
}

void SceneManager::Init()
{
	_currentScene->Init();
}

void SceneManager::Update()
{
	_currentScene->Update();
}

void SceneManager::Render()
{
	_currentScene->Render();
}

void SceneManager::PostRender()
{
	_currentScene->PostRender();
}

void SceneManager::Release()
{
	_currentScene->Release();
}

void SceneManager::SceneChange(int sceneNum)
{
}
