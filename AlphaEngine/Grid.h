#pragma once
#include "GameObject.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include "Camera.h"

typedef std::vector<GLfloat> GLvec;

class Grid : public GameObject
{
	GLuint _vao;
	GLuint _buffer[3];
	GLvec _vertices;
	GLvec _normals;
	GLvec _colors;
	GLuint _shader;

	Camera* _camera;

	glm::mat4 _model;
public:
	Grid();
	virtual ~Grid();
	void Init() override;
	void Update() override;
	void Render() override;
	void Release() override;
};

