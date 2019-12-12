#include "Grid.h"
#include <vector>
#include "ShaderManager.h"
#include "OpenGLUtil.h"
#include <glm/gtc/type_ptr.hpp>

void get_grid(std::vector<GLfloat>& p, GLfloat w, GLfloat h, int m, int n)
{
	GLfloat x0 = -0.5f * w;
	GLfloat x1 = +0.5f * w;
	GLfloat z0 = -0.5f * h;
	GLfloat z1 = +0.5f * h;
	for (int i = 0; i <= m; ++i)
	{
		GLfloat x = x0 + w * i / m;
		p.push_back(x);
		p.push_back(0);
		p.push_back(z0);
		p.push_back(x);
		p.push_back(0);
		p.push_back(z1);
	}

	for (int i = 0; i <= n; ++i)
	{
		GLfloat z = z0 + h * i / n;
		p.push_back(x0);
		p.push_back(0);
		p.push_back(z);
		p.push_back(x1);
		p.push_back(0);
		p.push_back(z);
	}
}

Grid::Grid() : GameObject("Grid")
{
}

Grid::~Grid()
{
}

void Grid::Init()
{
	_shader = ShaderManager::GetInstance()->GetShader(ShaderManager::Phong);		// phong shader load.
	glGenVertexArrays(1, &_vao);
	{
		glBindVertexArray(_vao);
		glGenBuffers(3, _buffer);

		get_grid(_vertices, 10, 10, 10, 10);
		_colors.resize(_vertices.size());
		for (size_t i = 0; i < _colors.size(); ++i)
			_colors[i] = 1;

		_normals = GLvec(_vertices.size(), 0);

		bind_buffer(_buffer[0], _vertices, _shader, "vPosition", 3);
		bind_buffer(_buffer[1], _colors, _shader, "vColor", 3);
		bind_buffer(_buffer[2], _normals, _shader, "vNormal", 3);
	}

	_camera = Camera::GetInstance();
}

void Grid::Update()
{
	_model = glm::mat4(1.0f);
	_model = glm::scale(_model, glm::vec3(1.0f));
}

void Grid::Render()
{
	glUseProgram(_shader);
	glBindVertexArray(_vao);

	auto V = _camera->GetViewing();
	auto VM = V * _model;
	auto P = _camera->GetOrthographic();
	auto U = transpose(inverse(VM));
	auto LightPos_cs = V * glm::vec4(5, 5, 5, 1);

	glUniform1i(glGetUniformLocation(_shader, "ColorMode"), 1);
	glUniformMatrix4fv(glGetUniformLocation(_shader, "V"), 1, GL_FALSE, value_ptr(V));
	glUniformMatrix4fv(glGetUniformLocation(_shader, "P"), 1, GL_FALSE, value_ptr(P));
	glUniformMatrix4fv(glGetUniformLocation(_shader, "VM"), 1, GL_FALSE, value_ptr(VM));
	glUniformMatrix4fv(glGetUniformLocation(_shader, "U"), 1, GL_FALSE, value_ptr(U));

	glDrawArrays(GL_LINES, 0, _vertices.size() / 3);
}

void Grid::Release()
{
}
