﻿#pragma once
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
void CreateTriangle();

/*
To do:
1 make light directional light(Point light is not good for shadow maps.)
2 SetCamera(eyepos, viewdir) instead of (eyepos, horizonangle, verticalangle)

3 shadow map rendering. (SetCamera(lightPos, viewDir to the center))
4 use shadow map to do light shaft.
*/

int main(int argc, char **argv) {
	try {

	glutInit(&argc, argv);

	gRenderEngine = new RenderEngine();

	glm::vec3 eyePos(66.5f, 30.0f, 0.0f);
	glm::vec3 eyeDir(glm::vec3(0) - eyePos);
	glm::vec3 lightPos(100.f, 100.f, 100.f);
	glm::vec3 lightDir(glm::vec3(0) - lightPos);
	gRenderEngine->SetCamera(eyePos, eyeDir, lightPos, lightDir);
	gRenderEngine->AllocateInput();
	gRenderEngine->AddLight(lightPos, lightDir, glm::vec3(1.0f, 1.0f, 1.0f));

	gRenderEngine->AllocateMeshAccess("truck_color.bmp", "./models/L200-OBJ/", "L200-OBJ.obj");

	gRenderEngine->AllocateShader();
	printOpenGLError();

	if(gRenderEngine->fMeshes.size() > 99) { printf("too many meshes\n"); exit(-4); }

	printOpenGLError();

	CreatePlane();
	CreateTriangle();

	for(size_t i = 0 ; i < gRenderEngine->fMeshes.size() ; i++) {

		Mesh & mesh = gRenderEngine->fMeshes.at(i);
		Material & mat = mesh.fMat;

		gRenderEngine->CreateBatch(mesh.fVertices, mesh.fIndices, mesh.fNormals, mesh.fMat.fGLTexID, mesh.fUVs);
	}

	gRenderEngine->CreateRenderTarget();

	printOpenGLError();

	glutMainLoop();

	}
	catch(char * e) {
		std::cout << e << std::endl;
	}
	return 0;
}

#define SHRINKPLANE 4


void CreatePlane()
{
	const GLfloat gPlanes[] = {
		-200.f, 0.f, -200.f, // tri1
		200.f, 0.f, -200.f,
		-200.f, 0.f, 200.f,
		200.f, 0.f, 200.f,
	};

	const GLfloat gUVs[] = {
		0, 0,
		1, 0,
		0, 1,
		1, 1
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
		verts.push_back( glm::vec3(gPlanes[i] /SHRINKPLANE, gPlanes[i+1]/SHRINKPLANE, gPlanes[i+2]/SHRINKPLANE) );
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
	for( size_t i = 0 ; i < sizeof(gUVs) / sizeof(GLfloat) ; i += 2 ) {
		UVs.push_back( glm::vec2(gUVs[i], gUVs[i+1]) );
	}

	gRenderEngine->CreateBatch(verts, inds, normals, 0, UVs);
}

void CreateTriangle()
{
	const GLfloat gTriangles[] = {
		0.f, 20.f, 0.f, // tri1
		10.f, 20.f, 40.f,
		10.f, 20.f, -10.f
	};

	//const GLfloat gUVs[] = {
	//	0, 0,
	//	1, 0,
	//	0, 1,
	//	1, 1
	//};

	const unsigned int gPlaneInds[] = {
		0, 1, 2
	};

	const GLfloat gPlaneNormals[] = {
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
	};

	std::vector<glm::vec3> verts;
	for( size_t i = 0 ; i < sizeof(gTriangles) / sizeof(GLfloat) ; i += 3 ) {
		verts.push_back( glm::vec3(gTriangles[i], gTriangles[i+1], gTriangles[i+2]) );
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
	//for( size_t i = 0 ; i < sizeof(gUVs) / sizeof(GLfloat) ; i += 2 ) {
	//	UVs.push_back( glm::vec2(gUVs[i], gUVs[i+1]) );
	//}

	gRenderEngine->CreateBatch(verts, inds, normals, 0, UVs);
}

