#include "ObjModel.h"
#include "LoadObj.h"
#include "LoadTex.h"
#include "ShaderManager.h"
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLUtil.h"

#ifdef _DEBUG
std::string ObjModel::path = "../Assets/Model/";
#else
std::string ObjModel::path = "../Assets/Model/";
#endif

ObjModel::ObjModel(const char* modelName) : _modelPath(path+modelName)
{
}

ObjModel::~ObjModel()
{
}

void ObjModel::Init()
{
	glGenVertexArrays(1, &_vao);
	{
		glBindVertexArray(_vao);
		glGenBuffers(4, _buffer);
		tinyobj::attrib_t attrib;
		load_obj(_modelPath.c_str(), path.c_str(), _vertices, _normals, _vertex_map, attrib, _shapes, _materials, 1);
		glActiveTexture(GL_TEXTURE0);
		load_tex(path.c_str(), _texcoords, _texmap, attrib.texcoords, _shapes, _materials, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);

		_colors.resize(_vertices.size());
		for (size_t i = 0; i < _colors.size(); ++i)
			_colors[i] = 1.0f;

		static ShaderManager* sm = ShaderManager::GetInstance();
		
		_shader = sm->GetShader(ShaderManager::Phong);
		glUseProgram(_shader);
		glBindVertexArray(_vao);
		bind_buffer(_buffer[0], _vertices, _shader, "vPosition", 3);
		bind_buffer(_buffer[1], _colors, _shader, "vColor", 3);
		bind_buffer(_buffer[2], _texcoords, _shader, "vTexcoord", 2);
		bind_buffer(_buffer[3], _normals, _shader, "vNormal", 3);

		for (int i = 1; i < ShaderManager::Sprite; ++i)
		{
			_shader = sm->GetShader(static_cast<ShaderManager::Shader>(i));		// phong shader load.
			glUseProgram(_shader);
			glBindVertexArray(_vao);

			bind_buffer(_buffer[0], _shader, "vPosition", 3);
			bind_buffer(_buffer[1], _shader, "vColor", 3);
			bind_buffer(_buffer[2], _shader, "vTexcoord", 2);
			bind_buffer(_buffer[3], _shader, "vNormal", 3);
		}
	}

	_camera = Camera::GetInstance();
}

void ObjModel::Update(glm::mat4 modelMatrix)
{
	_modelMatrix = modelMatrix;
}

void ObjModel::Render()
{
	static ShaderManager* sm = ShaderManager::GetInstance();
	_shader = sm->GetShader();
	glUseProgram(_shader);
	glBindVertexArray(_vao);

	bind_buffer(_buffer[0], _shader, "vPosition", 3);
	bind_buffer(_buffer[1], _shader, "vColor", 3);
	bind_buffer(_buffer[2], _shader, "vTexcoord", 2);
	bind_buffer(_buffer[3], _shader, "vNormal", 3);
	
	auto V = _camera->GetViewing();
	auto VM = V * _modelMatrix;
	auto P = _camera->GetOrthographic();
	auto U = transpose(inverse(VM));
	auto LightPos_cs = V * glm::vec4(4, 4, -4, 1);

	glUniformMatrix4fv(glGetUniformLocation(_shader, "V"), 1, GL_FALSE, value_ptr(V));
	glUniformMatrix4fv(glGetUniformLocation(_shader, "P"), 1, GL_FALSE, value_ptr(P));
	glUniformMatrix4fv(glGetUniformLocation(_shader, "VM"), 1, GL_FALSE, value_ptr(VM));
	glUniformMatrix4fv(glGetUniformLocation(_shader, "U"), 1, GL_FALSE, value_ptr(U));

	glUniform1i(glGetUniformLocation(_shader, "ColorMode"), 0);
	glUniform3fv(glGetUniformLocation(_shader, "LightPos_cs"), 1, value_ptr(LightPos_cs));
	glUniform3f(glGetUniformLocation(_shader, "Il"), 6, 6, 6);
	glUniform3f(glGetUniformLocation(_shader, "Ia"), 2, 2, 2);
	glUniform3f(glGetUniformLocation(_shader, "c"), 0.1, 0.1, 0.1);

	for (size_t i = 0; i < 1/*shapes.size()*/; ++i) {
		int m_id = _shapes[i].mesh.material_ids[0];
		if (m_id < 0) {
			glUniform1f(glGetUniformLocation(_shader, "n"), 10.0f);
			glUniform3f(glGetUniformLocation(_shader, "Ka"), 0.3f, 0.3f, 0.3f);
			glUniform3f(glGetUniformLocation(_shader, "Kd"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(_shader, "Ks"), 0.8f, 0.8f, 0.8f);
		}
		else {
			glUniform1f(glGetUniformLocation(_shader, "n"), _materials[m_id].shininess);

			/// TODO : ¾Æ´Ï ¸ðµ¨¸µÆÄÀÏ¿¡ ka, kd, ks ¾È³ÖÀº³ð ´ëÃ¼ ¹¹³Ä..
			/*glUniform3fv(glGetUniformLocation(_shader, "Ka"), 1, _materials[m_id].ambient);
			glUniform3fv(glGetUniformLocation(_shader, "Kd"), 1, _materials[m_id].diffuse);
			glUniform3fv(glGetUniformLocation(_shader, "Ks"), 1, _materials[m_id].specular);*/

			glUniform3f(glGetUniformLocation(_shader, "Ka"), 0.2f, 0.2f, 0.2f);
			glUniform3f(glGetUniformLocation(_shader, "Kd"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(_shader, "Ks"), 0.0f, 0.0f, 0.0f);

			auto texitem = _texmap.find(_materials[m_id].diffuse_texname);
			if (texitem != _texmap.end())
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texitem->second);
				glUniform1i(glGetUniformLocation(_shader, "sampler"), 0);
			}
		}

		glDrawArrays(GL_TRIANGLES, _vertex_map[i], _vertex_map[i + 1] - _vertex_map[i]);
	}
}

void ObjModel::Release()
{
}
