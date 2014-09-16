#pragma once
#include "Shader.h"

#include <vector>
#include <glm.hpp>

class Batch
{
public:
	Batch(int vertexVBO, std::vector<glm::vec3> & inVertices,
		int indexVBO, std::vector<unsigned int> & inIndices,
		int normalVBO, std::vector<glm::vec3> & normals,
		int glTexID, int UVVBO, std::vector<glm::vec2> & UVs);
	~Batch(void);

	int fVertexVBO;
	int fIndexVBO;
	int fNormalVBO;
	int fUVVBO;
	std::vector<glm::vec3> fVertices;
	std::vector<unsigned int> fIndices;
	std::vector<glm::vec3> fNormals;
	std::vector<glm::vec2> fUVs;

	unsigned int fGLTexID;
};

