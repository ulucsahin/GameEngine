#pragma once

#include <vector>
#include <glm/glm.hpp>

class ModelLoader
{
public:
	static bool LoadObj(const char* filepath,
		std::vector <glm::vec3>& out_vertices,
		std::vector <glm::vec2>& out_uvs,
		std::vector <glm::vec3>& out_normals,
		std::vector <unsigned int>& out_indices);

	static bool LoadObj2(const char* filepath,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_texture_coords,
		std::vector<unsigned int>& out_faces);
		
	static void LoadModel3(const char* filepath,
		std::vector<float>& outVertices,
		std::vector<int>& outFaceIndexes,
		std::vector<float>& outUV);

};