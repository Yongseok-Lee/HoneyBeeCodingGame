#include "LoadTex.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

#define MAP_FIND(map_obj, item) ((map_obj).find(item) != (map_obj).end())
#define IS_MIPMAP(flag) ((flag) == GL_LINEAR_MIPMAP_LINEAR ||  (flag) == GL_LINEAR_MIPMAP_NEAREST ||  (flag) == GL_NEAREST_MIPMAP_LINEAR ||  (flag) == GL_NEAREST_MIPMAP_NEAREST)

static bool has_file(const char* filepath)
{
	FILE *fp; if (fopen_s(&fp, filepath, "rb") == 0)
	{
		fclose(fp); 
		return true;
	} 
	return false;
}

bool load_tex(const char* basedir, std::vector<tinyobj::real_t>& texcoords_out,
	std::map<std::string, size_t>& texmap_out, const std::vector<tinyobj::real_t>& texcoords,
	const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials, GLint min_filter,
	GLint mag_filter)
{
	using namespace std; using namespace tinyobj;
	size_t total_num_of_vertices = 0; size_t num_of_shapes = shapes.size();
	for (size_t s = 0; s < num_of_shapes; ++s)
	{
		total_num_of_vertices += shapes[s].mesh.indices.size();
	}

	texcoords_out.resize(total_num_of_vertices * 2);
	real_t* texcoords_dst_ptr = texcoords_out.data();
	const real_t* texcoords_src_ptr = texcoords.size() > 0 ? texcoords.data() : NULL;

	for (size_t s = 0; s < num_of_shapes; ++s) {

		const mesh_t& mesh = shapes[s].mesh; size_t num_of_faces = mesh.indices.size() / 3;

		for (size_t f = 0; f < num_of_faces; ++f) { // Get indices to the three vertices of a triangle. 
			int idx[3] = { mesh.indices[3 * f + 0].texcoord_index, mesh.indices[3 * f + 1].texcoord_index, mesh.indices[3 * f + 2].texcoord_index };

			// Compute and copy valid texture coordinates. ......
			real_t tc[3][2]; 
			if (texcoords_src_ptr != NULL)
			{
				if (idx[0] < 0 || idx[1] < 0 || idx[2] < 0)
				{
					fprintf(stderr, "Invalid texture coordinate index\n"); return false;
				}
				for (size_t i = 0; i < 3; ++i) {
					memcpy(tc[i], texcoords_src_ptr + idx[i] * 2, sizeof(real_t) * 2);
					tc[i][1] = 1.0f - tc[i][1];// flip the t coordinate. 
					}
			}
			else
			{
				tc[0][0] = tc[0][1] = tc[0][2] = 0; 
				tc[1][0] = tc[1][1] = tc[1][2] = 0; 
				tc[2][0] = tc[2][1] = tc[2][2] = 0;
			}
			memcpy(texcoords_dst_ptr, tc, sizeof(real_t) * 6); texcoords_dst_ptr += 6;
		}
	}

	GLuint texture_id; size_t num_of_materials = materials.size();
	for (size_t m = 0; m < num_of_materials; ++m) {
		const material_t& mat = materials[m];
		const string& texname = mat.diffuse_texname;
		if (texname.empty()) continue; if (MAP_FIND(texmap_out, texname)) continue;
		string full_texpath = texname;
		if (!has_file(full_texpath.c_str())) { 
			full_texpath = basedir + texname; 
			if (!has_file(full_texpath.c_str()))
			{
				fprintf(stderr, "Failed to find %s\n", texname.c_str()); return false;
			}
		}
		// Generate a texture object. 
		texture_id = generate_tex(full_texpath.c_str(), min_filter, mag_filter);
		if (texture_id < 0) return false;
		// Register the texture id. 
		texmap_out[texname] = texture_id;
	}

}

GLuint generate_tex(const char* tex_file_path, GLint min_filter, GLint mag_filter)
{
	int width, height, num_of_components; 
	unsigned char* image = stbi_load(tex_file_path, &width, &height, &num_of_components, STBI_default);
	if (!image)
	{
		fprintf(stderr, "Failed to open %s\n", tex_file_path); return false;
	} 
	// Generate a texture object and set its parameters. 
	GLuint texture_id; 
	glGenTextures(1, &texture_id); 
	glBindTexture(GL_TEXTURE_2D, texture_id); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	bool is_supported = true; 
	switch (num_of_components)
	{
	case 1:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image);
		break;
	case 3: 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image); 
		break; 
	case 4: 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image); 
		break; 
		default: 
		is_supported = false; 
		break;
	}
	if (IS_MIPMAP(min_filter) || IS_MIPMAP(mag_filter)) 
		glGenerateMipmap(GL_TEXTURE_2D);

	// Release the loaded image data. 
	stbi_image_free(image); 
	glBindTexture(GL_TEXTURE_2D, 0);
	if (!is_supported)
	{
		fprintf(stderr, "Unsupported image format: %d components\n", num_of_components); 
		glDeleteTextures(1, &texture_id); 
		texture_id = -1;
	}

	return texture_id;
}

