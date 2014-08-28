#include "Batch.h"


Batch::Batch(unsigned int ID, const std::vector<glm::vec3> & vertices, const std::vector<unsigned int> & indices, std::vector<glm::vec3> & normals, unsigned int glTexID, std::vector<glm::vec2> & UVs, Shader::EShaderKind kind)
{
	fID = ID;
	fVertices = vertices;
	fIndices = indices;
	fNormals = normals;
	fUVs = UVs;
	fProgram = kind;
	fGLTexID = glTexID;
}


Batch::~Batch(void)
{
}
