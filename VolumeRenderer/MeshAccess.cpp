#include "MeshAccess.h"
#include "tiny_obj_loader.h"

#include <vector>

MeshAccess::MeshAccess(void)
{
}


MeshAccess::~MeshAccess(void)
{
	for(size_t i = 0 ; i < fArrays.size() ; i++) {
		if(fArrays.at(i) != NULL)
			delete fArrays.at(i);
	}
}

void MeshAccess::Vertices(float * & verts, float * & inds, float * & normals)
{
	size_t nPos = 0;
	size_t nInds = 0;
	size_t nNormals = 0;
	for(size_t i = 0 ; i < fShapes.size() ; i++) {
		tinyobj::shape_t & sp = fShapes[i];
		tinyobj::mesh_t & mh = sp.mesh;

		nPos += mh.positions.size();
		nInds += mh.indices.size();
		nNormals += mh.normals.size();
	}

	float * pos = new float[nPos];
	float * indices = new float [nInds];
	float * norms = new float [nNormals];
	fArrays.push_back(pos);
	fArrays.push_back(indices);
	fArrays.push_back(norms);

	size_t indexPos = 0;
	size_t indexInds = 0;
	size_t indexNorms = 0;
	for(size_t i = 0 ; i < fShapes.size() ; i++) {
		tinyobj::shape_t & sp = fShapes[i];
		tinyobj::mesh_t & mh = sp.mesh;

		for(size_t j = 0 ; j < mh.positions.size() ; j++) {
			pos[indexPos++] = mh.positions[j];
		}

		for(size_t j = 0 ; j < mh.indices.size() ; j++) {
			indices[indexInds++] = mh.indices[j];
		}

		for(size_t j = 0 ; j < mh.normals.size() ; j++) {
			norms[indexNorms++] = mh.normals[j];
		}
	}

	verts = pos;
	inds = indices;
	normals = norms;
}


void MeshAccess::LoadOBJFile(std::string & name, std::string &mtl_basepath)
{
	std::string checkerr = LoadObj(fShapes, name.c_str(), mtl_basepath.c_str());
	if(checkerr.compare("") != 0) {
		throw "obj loading failed.";
	}


}