#pragma once

#include <vector>
#include "tiny_obj_loader.h"
#include <glm.hpp>



class IMeshAccess
{
public:

	virtual void LoadOBJFile(std::string & name, std::string &mtl_basepath) = 0;
	virtual void Vertices(std::vector<glm::vec3> & vertices, std::vector<unsigned short> & indices, std::vector<glm::vec3> & normals) = 0;
};

class MeshAccess : public IMeshAccess
{
public:
	MeshAccess(void);
	~MeshAccess(void);

	void LoadOBJFile(std::string & name, std::string &mtl_basepath);

	void Vertices(std::vector<glm::vec3> & vertices, std::vector<unsigned short> & indices, std::vector<glm::vec3> & normals);

	void Colors(size_t nColors, std::vector<float> & colors);

protected:

	std::vector<tinyobj::shape_t> fShapes;
	std::vector<float *> fArrays;
};

