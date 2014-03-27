#pragma once

#include <vector>
#include "tiny_obj_loader.h"



class IMeshAccess
{
public:

	virtual void LoadOBJFile(std::string & name, std::string &mtl_basepath) = 0;
	virtual void Vertices(std::vector<float> & vertices, std::vector<unsigned int> & indices, std::vector<float> & normals) = 0;
};

class MeshAccess : public IMeshAccess
{
public:
	MeshAccess(void);
	~MeshAccess(void);

	void LoadOBJFile(std::string & name, std::string &mtl_basepath);

	void Vertices(std::vector<float> & vertices, std::vector<unsigned int> & indices, std::vector<float> & normals);

	void Colors(size_t nColors, std::vector<float> & colors);

protected:

	std::vector<tinyobj::shape_t> fShapes;
	std::vector<float *> fArrays;
};

