#pragma once
#include <string>
#include <map>
#include <vector>
#include "tiny_obj_loader.h"
#include <GL/glew.h>
#include <glm/matrix.hpp>
#include "Camera.h"

struct ObjModel
{
	static std::string path;
	std::string _modelPath;

	GLuint _vao;
	GLuint _buffer[4];
	GLuint _shader;
	glm::mat4 _modelMatrix;

	std::vector<tinyobj::real_t> _vertices;
	std::vector<tinyobj::real_t> _normals;
	std::vector<tinyobj::real_t> _colors;
	std::vector<size_t> _vertex_map;
	std::vector<tinyobj::shape_t> _shapes;
	std::vector<tinyobj::material_t> _materials;
	std::vector<tinyobj::real_t> _texcoords;
	std::map<std::string, size_t> _texmap;

	Camera* _camera;

	bool _isOrtho = true;

	ObjModel(const char* modelName);
	~ObjModel();

	void Init();
	void Update(glm::mat4 modelMatrix);
	void Render();
	void Release();
};

