#pragma once
#include <string>
#include "GL/glew.h"

class GameObject
{
protected:
	std::string _name;
	bool _active;

public:
	GameObject(std::string name)
	{
		_name = name;
		_active = true;
	}
	virtual ~GameObject() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;

	void SetName(std::string name)
	{
		_name = name;
	}

	std::string GetName() const
	{
		return _name;
	}

	void SetActive(bool active)
	{
		_active = active;
	}

	bool GetActive() const
	{
		return _active;
	}
};
