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

void MeshAccess::Vertices(std::vector<glm::vec3> & vertices, std::vector<unsigned short> & indices, std::vector<glm::vec3> & normals)
{
	for(size_t i = 0 ; i < fShapes.size() ; i++) {
		tinyobj::shape_t & sp = fShapes[i];
		tinyobj::mesh_t & mh = sp.mesh;

		for(size_t j = 0 ; j < mh.positions.size() ; j+=3) {
			vertices.push_back(glm::vec3(mh.positions[j], mh.positions[j+1], mh.positions[j+2]));
		}

		for(size_t j = 0 ; j < mh.indices.size() ; j++) {
			indices.push_back(mh.indices[j]);
		}

		for(size_t j = 0 ; j < mh.normals.size() ; j+=3) {
			normals.push_back(glm::vec3(mh.normals[j], mh.normals[j+1], mh.normals[j+2]));
		}
	}
}

void MeshAccess::Colors(size_t nColors, std::vector<float> & colors)
{
	srand((unsigned int)time(NULL));

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