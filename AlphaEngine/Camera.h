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

	Camera();
	virtual ~Camera() = default;

	glm::mat4 GetViewing() const;
	glm::mat4 GetOrthographic() const;
	glm::mat4 GetPerspective() const;
};
