#include "Batch.h"


Batch::Batch(int vertexVBO, std::vector<glm::vec3> & inVertices,
	  int indexVBO, std::vector<unsigned int> & inIndices,
	  int normalVBO, std::vector<glm::vec3> & normals,
	  int UVVBO, std::vector<glm::vec2> & UVs)
{
	fVertexVBO = vertexVBO;
	fIndexVBO = indexVBO;
	fNormalVBO = normalVBO;

	fUVVBO = UVVBO;

	fVertices = inVertices;
	fIndices = inIndices;
	fNormals = normals;
	fUVs = UVs;
}


Batch::~Batch(void)
{
}
