#pragma once

#include <vector>
#include "tiny_obj_loader.h"
#include <glm.hpp>


struct Mesh
{
	std::vector<glm::vec3> fVertices;
	std::vector<unsigned short> fIndices;
	std::vector<glm::vec3> fNormals;
	std::vector<glm::vec2> fUVs;
};

class IMeshAccess
{
public:

	virtual void LoadOBJFile(std::string & name, std::string &mtl_basepath) = 0;
	virtual void Vertices(std::vector<Mesh> & outMeshes) = 0;
};

class MeshAccess : public IMeshAccess
{
public:
	MeshAccess(void);
	~MeshAccess(void);

	void LoadOBJFile(std::string & name, std::string &mtl_basepath);

	virtual void Vertices(std::vector<Mesh> & outMeshes);

	void Colors(size_t nColors, std::vector<float> & colors);

protected:

	std::vector<tinyobj::shape_t> fShapes;
	std::vector<float *> fArrays;
};

