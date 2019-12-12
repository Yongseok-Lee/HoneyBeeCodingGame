#pragma once

#include <GL/glew.h>
#include <vector>
#include <map>
#include "tiny_obj_loader.h"

bool load_tex(const char* basedir, // base directory where texture 
									// files are located 
	std::vector<tinyobj::real_t>& texcoords_out, // output texture coordinates 
	std::map<std::string, size_t>& texmap_out, // output map from texture file name 
												// to its OpenGL texture id 
	const std::vector<tinyobj::real_t>& texcoords, // input texture coordinates 
	const std::vector<tinyobj::shape_t>& shapes, // a list of shapes 
	const std::vector<tinyobj::material_t>& materials, // a list of materials 
	GLint min_filter = GL_LINEAR_MIPMAP_LINEAR, // type of the min filter 
	GLint mag_filter = GL_LINEAR_MIPMAP_LINEAR); // type of the mag filter

GLuint generate_tex(const char* tex_file_path, GLint min_filter, GLint mag_filter);
