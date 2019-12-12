#pragma once
#include "Singleton.h"
class SceneManager;
class Interpolator;
class SoundManager;
struct GLFWwindow;

class GameApp : public Singleton<GameApp>
{
	SceneManager* _sceneManager;
	Interpolator* _interpolator;
	SoundManager* _soundManager;
	GLFWwindow* _window;

	int _width;
	int _height;

	bool _isExit;

public:
	GameApp();
	~GameApp();

	int GameLoop();

	void Init();
	void ProcessKey(int key, int scancode, int action, int mods);
	void Update();
	void Render();
	void Release();
};

