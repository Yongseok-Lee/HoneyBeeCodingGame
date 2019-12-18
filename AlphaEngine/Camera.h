#pragma once
#include "System.h"
#include "Singleton.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera : public Singleton<Camera>
{
public:
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;

	int _windowX;
	int _windowY;

	Camera();
	virtual ~Camera() = default;

	void Init();

	glm::mat4 GetSpriteProj() const;
	glm::mat4 GetViewing() const;
	glm::mat4 GetOrthographic() const;
	glm::mat4 GetPerspective() const;
};
