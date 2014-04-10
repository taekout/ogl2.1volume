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

void MeshAccess::Vertices(std::vector<Mesh> & outMeshes)
{
	for(size_t i = 0 ; i < fShapes.size() ; i++) {
		tinyobj::shape_t & sp = fShapes[i];
		tinyobj::mesh_t & mh = sp.mesh;

		Mesh outM;

		std::vector<glm::vec3> & verts = outM.fVertices;
		for(size_t j = 0 ; j < mh.positions.size() ; j+=3) {
			verts.push_back(glm::vec3(mh.positions[j], mh.positions[j+1], mh.positions[j+2]));
		}

		std::vector<unsigned short> & inds = outM.fIndices;
		for(size_t j = 0 ; j < mh.indices.size() ; j++) {
			inds.push_back(mh.indices[j]);
		}

		std::vector<glm::vec3> & norms = outM.fNormals;
		for(size_t j = 0 ; j < mh.normals.size() ; j+=3) {
			norms.push_back(glm::vec3(mh.normals[j], mh.normals[j+1], mh.normals[j+2]));
		}

		outMeshes.push_back(outM);
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