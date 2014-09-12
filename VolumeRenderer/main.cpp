#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "glut.h"
#include <string>
#include <functional>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "UserInput.h"
#include "Camera.h"
#include "MeshAccess.h"
#include "Light.h"
#include "Batch.h"

#include "RenderEngine.h"

extern RenderEngine * gRenderEngine;

void CreatePlane();

/*
To do:
1 make light directional light(Point light is not good for shadow maps.)
2 SetCamera(eyepos, viewdir) instead of (eyepos, horizonangle, verticalangle)

3 shadow map rendering. (SetCamera(lightPos, viewDir to the center))
4 use shadow map to do light shaft.
*/

glm::vec3 gEyePos(66.5f, 30.0f, 0.0f);
glm::vec3 gEyeDir(glm::vec3(0) - gEyePos);
glm::vec3 gLightPos(100.f, 100.f, 100.f);
glm::vec3 gLightDir(-0.5935844, -0.503884, -0.6275010);

int main(int argc, char **argv) {
	try {

	glutInit(&argc, argv);

	gRenderEngine = new RenderEngine();

	
	gRenderEngine->SetCamera(gEyePos, gEyeDir);
	gRenderEngine->AllocateInput();
	gRenderEngine->AddLight(gLightPos, glm::vec3(0) - gLightPos, glm::vec3(1.0f, 1.0f, 1.0f));

	gRenderEngine->AllocateMeshAccess("truck_color.bmp", "./models/L200-OBJ/", "L200-OBJ.obj");

	gRenderEngine->AllocateShader();
	printOpenGLError();

	if(gRenderEngine->fMeshes.size() > 99) { printf("too many meshes\n"); exit(-4); }

	printOpenGLError();

	CreatePlane();

	for(size_t i = 0 ; i < gRenderEngine->fMeshes.size() ; i++) {

		Mesh & mesh = gRenderEngine->fMeshes.at(i);
		Material & mat = mesh.fMat;

		gRenderEngine->CreateBatch(mesh.fVertices, mesh.fIndices, mesh.fNormals, mesh.fMat.fGLTexID, mesh.fUVs, Shader::EShaderKind::eShaderTexture);
	}

	printOpenGLError();

	//gRenderEngine->SetupRenderTarget(gRenderEngine->fLightCamera->GetEyePos());
	//gRenderEngine->RenderBatch();
	//gRenderEngine->SetdownRenderTarget();


	
	//gRenderEngine->ComputeRenderMat();
	printOpenGLError();

	glutMainLoop();

	}
	catch(char * e) {
		std::cout << e << std::endl;
	}
	return 0;
}


void CreatePlane()
{
	const GLfloat gPlanes[] = {
		-200.f, 0.f, -200.f, // tri1
		200.f, 0.f, -200.f,
		-200.f, 0.f, 200.f,
		200.f, 0.f, 200.f,
	};

	const unsigned int gPlaneInds[] = {
		0, 1, 2,
		2, 1, 3
	};

	const GLfloat gPlaneNormals[] = {
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
	};

	std::vector<glm::vec3> verts;
	for( size_t i = 0 ; i < sizeof(gPlanes) / sizeof(GLfloat) ; i += 3 ) {
		verts.push_back( glm::vec3(gPlanes[i], gPlanes[i+1], gPlanes[i+2]) );
	}

	std::vector<unsigned int> inds;
	for( size_t i = 0 ; i < sizeof(gPlaneInds) / sizeof(unsigned int) ; i ++ ) {
		inds.push_back( gPlaneInds[i] );
	}

	std::vector<glm::vec3> normals;
	for( size_t i = 0 ; i < sizeof(gPlaneNormals) / sizeof(GLfloat) ; i += 3 ) {
		normals.push_back( glm::vec3(gPlaneNormals[i], gPlaneNormals[i+1], gPlaneNormals[i+2]) );
	}

	std::vector<glm::vec2> UVs;

	gRenderEngine->CreateBatch(verts, inds, normals, 0, UVs, Shader::EShaderKind::eShaderBasic);
}

