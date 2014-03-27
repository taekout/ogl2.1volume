#include "MeshAccess.h"
#include "tiny_obj_loader.h"

#include <vector>
#include <time.h>
#include <stdio.h>

MeshAccess::MeshAccess(void)
{
}


MeshAccess::~MeshAccess(void)
{
	for(size_t i = 0 ; i < fArrays.size() ; i++) {
		if(fArrays.at(i) != NULL)
			delete [] fArrays.at(i);
	}
}

void MeshAccess::Vertices(std::vector<float> & vertices, std::vector<unsigned int> & indices, std::vector<float> & normals)
{
	for(size_t i = 0 ; i < fShapes.size() ; i++) {
		tinyobj::shape_t & sp = fShapes[i];
		tinyobj::mesh_t & mh = sp.mesh;

		for(size_t j = 0 ; j < mh.positions.size() ; j++) {
			vertices.push_back(mh.positions[j]);
		}

		for(size_t j = 0 ; j < mh.indices.size() ; j++) {
			indices.push_back(mh.indices[j]);
		}

		for(size_t j = 0 ; j < mh.normals.size() ; j++) {
			normals.push_back(mh.normals[j]);
		}
	}
}

void MeshAccess::Colors(size_t nColors, std::vector<float> & colors)
{
	srand(time(NULL));

	for(size_t i = 0 ; i < nColors ; i++) {
		colors.push_back( (float)rand() / RAND_MAX );
	}

}


void MeshAccess::LoadOBJFile(std::string & name, std::string &mtl_basepath)
{
	std::string checkerr = LoadObj(fShapes, name.c_str(), mtl_basepath.c_str());
	if(checkerr.compare("") != 0) {
		throw "obj loading failed.";
	}


}