#include "Batch.h"


Batch::Batch(unsigned int ID, std::vector<glm::vec3> & inVertices, std::vector<unsigned int> & inIndices, std::vector<glm::vec3> & normals, unsigned int glTexID, std::vector<glm::vec2> & UVs, Shader::EShaderKind kind)
{
	fID = ID;
	fVertices =inVertices;
	fIndices = inIndices;
	fNormals = normals;
	fUVs = UVs;
	fProgram = kind;
	fGLTexID = glTexID;
}


Batch::~Batch(void)
{
}
