#pragma once

#include <vector>
#include "tiny_obj_loader.h"
#include <glm.hpp>

struct Material
{
	Material() : fDiffuse(1.0, 1.0, 1.0), fSpecular(1.0, 1.0, 1.0), fTransmittance(1.0, 1.0, 1.0), fEmission(1.0, 1.0, 1.0), fShininess(0.0f), fIndexRefract(0.f) {}
	
	Material(const std::string &ID, const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular, const glm::vec3 &transmit,
			const glm::vec3 & emission, float shininess, float indexOfRefract, const std::string ambientTex, const std::string diffuseTex,
			const std::string specularTex, const std::string normalTex)
			: fID(ID), fAmbient(ambient), fDiffuse(diffuse), fSpecular(specular), fTransmittance(transmit), fEmission(emission),
			fShininess(shininess), fIndexRefract(indexOfRefract), fAmbientTexName(ambientTex), fDiffuseTexName(diffuseTex), fSpecularTexName(specularTex), fNormalTexName(normalTex)
	{
	}

	std::string fID;
	glm::vec3 fAmbient;
	glm::vec3 fDiffuse;
	glm::vec3 fSpecular;
	glm::vec3 fTransmittance;
	glm::vec3 fEmission;
	float fShininess;
	float fIndexRefract;

	std::string fAmbientTexName;
	std::string fDiffuseTexName;
	std::string fSpecularTexName;
	std::string fNormalTexName;
};

struct Mesh
{
	std::vector<glm::vec3> fVertices;
	std::vector<unsigned short> fIndices;
	std::vector<glm::vec3> fNormals;
	std::vector<glm::vec2> fUVs;

	Material fMat;
};

class IMeshAccess
{
public:
	virtual ~IMeshAccess() {}
	virtual void LoadOBJFile(std::string & name, std::string &mtl_basepath) = 0;
	virtual void GetMeshData(std::vector<Mesh> & outMeshes) = 0;
};

class MeshAccess : public IMeshAccess
{
public:
	MeshAccess(void);
	virtual ~MeshAccess(void);

	void LoadOBJFile(std::string & name, std::string &mtl_basepath);

	virtual void GetMeshData(std::vector<Mesh> & outMeshes);

	void Colors(size_t nColors, std::vector<float> & colors);

protected:

	std::vector<tinyobj::shape_t> fShapes;
	std::vector<float *> fArrays;
};

