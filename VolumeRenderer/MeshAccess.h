#pragma once

#include <vector>
#include "tiny_obj_loader.h"



class IMeshAccess
{
public:

	virtual void LoadOBJFile(std::string & name, std::string &mtl_basepath) = 0;
	virtual void Vertex() = 0;
};

class MeshAccess : public IMeshAccess
{
public:
	MeshAccess(void);
	~MeshAccess(void);

	void LoadOBJFile(std::string & name, std::string &mtl_basepath);

	virtual void Vertex();

private:

	std::vector<tinyobj::shape_t *> fShapes;
};

