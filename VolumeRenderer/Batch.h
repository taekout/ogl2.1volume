#pragma once
#include "Shader.h"

#include <vector>
#include <glm.hpp>

class Batch
{
public:
	Batch(unsigned int ID, std::vector<glm::vec3> & inVertices, std::vector<unsigned int> & inIndices, std::vector<glm::vec3> & normals, unsigned int glTexID, std::vector<glm::vec2> & UVs, Shader::EShaderKind kind);
	~Batch(void);

	unsigned int fID;
	std::vector<glm::vec3> fVertices;
	std::vector<unsigned int> fIndices;
	std::vector<glm::vec3> fNormals;
	std::vector<glm::vec2> fUVs;

	unsigned int fGLTexID;
	Shader::EShaderKind fProgram;
};

