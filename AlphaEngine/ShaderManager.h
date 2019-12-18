#pragma once
#include "Singleton.h"
#include <vector>

class ShaderManager : public Singleton<ShaderManager>
{
	std::vector<int> _shaders;
	
public:
	enum Shader
	{
		Phong = 0,
		Gouraud = 1,
		Flat = 2,
		Sprite = 3,
		ShaderNum
	};

	ShaderManager();
	virtual ~ShaderManager();

	void SetShader(Shader shader);
	int GetShader();
	int GetShader(Shader shader);
private:
	int _currentShader;
};

