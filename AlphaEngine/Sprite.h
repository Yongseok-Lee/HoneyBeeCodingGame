#pragma once
#include <string>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <glm/matrix.hpp>
#include "Camera.h"

struct Sprite
{
	static std::string path;
	std::string _modelPath;

	GLuint _vao;
	GLuint _buffer[2];
	GLuint _shader;
	GLuint _texture;
	glm::mat4 _modelMatrix;
	glm::mat4 _projMatrix;

	std::vector<GLfloat> _vertices;
	std::vector<GLfloat> _colors;
	std::vector<GLfloat> _texcoords;

	Camera* _camera;

	Sprite(const char* spriteName);
	virtual ~Sprite();

	void Init();
	void Update(glm::mat4 modelMatrix);
	void Render();
	void Release();

	void SetColor(glm::vec3 color);
};

