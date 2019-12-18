#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GameApp.h"

Camera::Camera(): eye(0, 0, 5), center(0, 0, 0), up(0, 1, 0)
{
	// isometric camera.
	glm::vec4 front(1, 0, 0, 1);
	glm::mat4 isoMatrix(1.0f);
	isoMatrix = glm::rotate(isoMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	front = front * isoMatrix;
	isoMatrix = glm::rotate(isoMatrix, glm::radians(35.264f), glm::vec3(front));
	eye = glm::vec3(glm::vec4(eye, 1) * isoMatrix);
}

void Camera::Init()
{
	eye = glm::vec3(0, 0, 5);
	center = glm::vec3(0, 0, 0);
	up = glm::vec3(0, 1, 0);
	glm::vec4 front(1, 0, 0, 1);
	glm::mat4 isoMatrix(1.0f);
	isoMatrix = glm::rotate(isoMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	front = front * isoMatrix;
	isoMatrix = glm::rotate(isoMatrix, glm::radians(35.264f), glm::vec3(front));
	eye = glm::vec3(glm::vec4(eye, 1) * isoMatrix);

	auto app = GameApp::GetInstance();
	_windowX = app->_width;
	_windowY = app->_height;
}

glm::mat4 Camera::GetSpriteProj() const
{
	return glm::ortho(0.0f, (float)_windowX, (float)_windowY, 0.0f, -1.0f, 1.0f);
}

glm::mat4 Camera::GetViewing() const
{
	return lookAt(eye, center, up);
}

glm::mat4 Camera::GetOrthographic() const
{
	return glm::ortho(-1.5f, +1.5f, -1.0f, +1.0f, 0.05f, 200.0f);
}

glm::mat4 Camera::GetPerspective() const
{
	return glm::frustum(-1.5f, +1.5f, -1.0f, +1.0f, 0.05f, 200.0f);
}
