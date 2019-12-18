#include "Sprite.h"
#include "LoadTex.h"
#include "ShaderManager.h"
#include "OpenGLUtil.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#ifdef _DEBUG
std::string Sprite::path = "../Assets/Sprite/";
#else
std::string Sprite::path = "../Assets/Sprite/";
#endif

Sprite::Sprite(const char* modelName) : _modelPath(path + modelName)
{
}

Sprite::~Sprite()
{
}

void Sprite::Init()
{
	glGenVertexArrays(1, &_vao);
	{
		static ShaderManager* sm = ShaderManager::GetInstance();
		_shader = sm->GetShader(ShaderManager::Sprite);
		glUseProgram(_shader);
		glBindVertexArray(_vao);
		glGenBuffers(2, _buffer);

		glActiveTexture(GL_TEXTURE0);
		_texture = generate_tex(_modelPath.c_str(), GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);

		_vertices.clear();
		_vertices = {
			// Pos      // Tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		_texcoords = {
			0.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		};
		_colors.resize(18);
		for (size_t i = 0; i < _colors.size(); ++i)
			_colors[i] = 1.0f;

		glUseProgram(_shader);
		glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*_vertices.size(), _vertices.data(), GL_STATIC_DRAW);

		glBindVertexArray(this->_vao);
		glEnableVertexAttribArray(glGetAttribLocation(_shader, "vPosTex"));
		glVertexAttribPointer(glGetAttribLocation(_shader, "vPosTex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		bind_buffer(_buffer[1], _colors, _shader, "vColor", 3);
	}

	_camera = Camera::GetInstance();
	//_projMatrix = _camera->GetSpriteProj();
	_projMatrix = glm::ortho(0.0f, (float)1200, (float)800, 0.0f, -200.0f, 200.0f);
}

void Sprite::Update(glm::mat4 modelMatrix)
{
	_modelMatrix = modelMatrix;
}

void Sprite::Render()
{
	static ShaderManager* sm = ShaderManager::GetInstance();
	_shader = sm->GetShader(ShaderManager::Sprite);

	glUseProgram(_shader);
	glm::mat4 T = _projMatrix * _modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(_shader, "T"), 1, GL_FALSE, value_ptr(T));

	glUniform1i(glGetUniformLocation(_shader, "ColorMode"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glUniform1i(glGetUniformLocation(_shader, "sampler"), 0);
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(sm->GetShader());
}

void Sprite::Release()
{
	
}

void Sprite::SetColor(glm::vec3 color)
{
	for (size_t i = 0; i < _colors.size(); i+=3)
	{
		_colors[0] = color.r;
		_colors[1] = color.g;
		_colors[2] = color.b;
	}

	glUseProgram(_shader);
	bind_buffer(_buffer[1], _colors, _shader, "vColor", 3);
}
