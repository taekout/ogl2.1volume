#pragma once

#include <vector>
#include "tiny_obj_loader.h"



class IMeshAccess
{
public:

	virtual void LoadOBJFile(std::string & name, std::string &mtl_basepath) = 0;
	virtual void Vertices(float * & verts, float * & inds, float * & normals) = 0;
};

class MeshAccess : public IMeshAccess
{
public:
	MeshAccess(void);
	~MeshAccess(void);

	void LoadOBJFile(std::string & name, std::string &mtl_basepath);

	virtual void Vertices(float * & verts, float * & inds, float * & normals);

protected:

	std::vector<tinyobj::shape_t> fShapes;
	std::vector<float *> fArrays;
};

