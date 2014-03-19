#include "MeshAccess.h"
#include "tiny_obj_loader.h"

#include <vector>

MeshAccess::MeshAccess(void)
{
}


MeshAccess::~MeshAccess(void)
{
}

void MeshAccess::Vertex()
{
	return;
}


void MeshAccess::LoadOBJFile(std::string & name, std::string &mtl_basepath)
{
	std::vector<tinyobj::shape_t> shapes;
	std::string checkerr = LoadObj(shapes, name.c_str(), mtl_basepath.c_str());
	if(checkerr.compare("") != 0) {
		throw "obj loading failed.";
	}

}