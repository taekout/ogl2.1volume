#include "Batch.h"


Batch::Batch(int ID, const std::vector<glm::vec3> & vertices, const std::vector<unsigned int> & indices, std::vector<glm::vec3> & normals, std::vector<glm::vec2> & UVs)
{
	fID = ID;
	fVertices = vertices;
	fIndices = indices;
	fNormals = normals;
	fUVs = UVs;
}


Batch::~Batch(void)
{
}
