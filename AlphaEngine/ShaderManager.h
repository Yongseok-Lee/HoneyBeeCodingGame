#pragma once
#include "Singleton.h"
#include <vector>

class ShaderManager : public Singleton<ShaderManager>
{
	std::vector<int> _shaders;

public:
	enum Shader
	{
		Sprite,
		Phong,
		ShaderNum
	};

	ShaderManager();
	virtual ~ShaderManager();

	int GetShader(Shader shader);
};

