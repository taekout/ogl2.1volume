#include "MeshAccess.h"
#include "tiny_obj_loader.h"

#include <vector>
#include <time.h>
#include <stdio.h>
#include "gtc/type_ptr.hpp"

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

void MeshAccess::GetMeshData(std::vector<Mesh> & outMeshes)
{
	for(size_t i = 0 ; i < fShapes.size() ; i++) {
		tinyobj::shape_t & sp = fShapes[i];
		tinyobj::mesh_t & mh = sp.mesh;

		Mesh outM;

		Material & mat = outM.fMat;

		/*
		std::string name;

		float ambient[3];
		float diffuse[3];
		float specular[3];
		float transmittance[3];
		float emission[3];
		float shininess;
		float ior;                // index of refraction

		std::string ambient_texname;
		std::string diffuse_texname;
		std::string specular_texname;
		std::string normal_texname;
		*/
		tinyobj::material_t & refMat = sp.material;
		mat = Material(refMat.name, glm::make_vec3(refMat.ambient), glm::make_vec3(refMat.diffuse), glm::make_vec3(refMat.specular),
			glm::make_vec3(refMat.transmittance), glm::make_vec3(refMat.emission), refMat.shininess, refMat.ior,
			refMat.ambient_texname, refMat.diffuse_texname, refMat.specular_texname, refMat.normal_texname);

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

		std::vector<glm::vec2> & UVs = outM.fUVs;
		for(size_t j = 0 ; j < mh.texcoords.size() ; j+=2) {
			UVs.push_back(glm::vec2(mh.texcoords[j], mh.texcoords[j+1]));
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