#pragma once

class System
{
public:
	System() = default;
	virtual ~System() = default;

	virtual void Init() {}
	virtual void Update() {}
	virtual void PreRender() {}
	virtual void Render() {}
	virtual void PostRender() {}
	virtual void Release() {}
};
