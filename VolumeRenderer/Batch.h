#pragma once
#include <vector>
#include <glm.hpp>

class Batch
{
public:
	Batch(int ID, const std::vector<glm::vec3> & vertices, const std::vector<unsigned int> & indices, std::vector<glm::vec3> & normals, std::vector<glm::vec2> & UVs);
	~Batch(void);

private:
	unsigned fID;
	std::vector<glm::vec3> fVertices;
	std::vector<unsigned int> fIndices;
	std::vector<glm::vec3> fNormals;
	std::vector<glm::vec2> fUVs;

};

