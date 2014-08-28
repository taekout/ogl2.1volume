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

#define MODELLOADING 1

extern GraphicsEngine * gRenderEngine;

void CreatePlane()
{

	static const GLfloat gPlanes[] = {
		-200.f, 0.f, -200.f, // tri1
		200.f, 0.f, -200.f,
		-200.f, 0.f, 200.f,
		200.f, 0.f, 200.f,
	};

	static const unsigned int gPlaneInds[] = {
		0, 1, 2,
		2, 1, 3
	};

	static const GLfloat gPlaneNormals[] = {
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
	for( size_t i = 0 ; i < sizeof(gPlaneInds) / sizeof(unsigned int) ; i += 3 ) {
		verts.push_back( glm::vec3(gPlaneInds[i], gPlaneInds[i+1], gPlaneInds[i+2]) );
	}

	std::vector<glm::vec3> normals;
	for( size_t i = 0 ; i < sizeof(gPlaneNormals) / sizeof(GLfloat) ; i += 3 ) {
		verts.push_back( glm::vec3(gPlaneNormals[i], gPlaneNormals[i+1], gPlaneNormals[i+2]) );
	}

	std::vector<glm::vec2> UVs;

	gRenderEngine->CreateBatch(verts, inds, normals, -1, UVs, Shader::EShaderKind::eShaderBasic);
}



int main(int argc, char **argv) {
	try {

	glutInit(&argc, argv);

	gRenderEngine = new GraphicsEngine();

	gRenderEngine->AddLight(glm::vec3(100.f, 100.f, 100.f), glm::vec3(1.0f, 1.0f, 1.0f));

#if MODELLOADING
	gRenderEngine->AllocateMeshAccess("truck_color.bmp", "./models/L200-OBJ/", "L200-OBJ.obj");
#else
	std::vector<glm::vec3> verts;
	LoadCube(verts);
#endif

	gRenderEngine->AllocateShader();
	printOpenGLError();

	if(gRenderEngine->fMeshes.size() > 99) { printf("too many meshes\n"); exit(-4); }

	//CreatePlane();

	//gRenderEngine->fShader->UseProgram(Shader::EShaderKind::eShaderTexture);
	printOpenGLError();
/*

	for(size_t i = 0 ; i < gRenderEngine->fMeshes.size() ; i++) {

		Mesh & mesh = gRenderEngine->fMeshes.at(i);
		Material & mat = mesh.fMat;
		gRenderEngine->CreateBatch(mesh.fVertices,
									mesh.fIndices,
									mesh.fNormals,
									mat.fGLTexID,
									mesh.fUVs,
									Shader::EShaderKind::eShaderTexture );

		
		

		// materials.

	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);*/

	
	for(size_t i = 0 ; i < gRenderEngine->fMeshes.size() ; i++) {

		Mesh & mesh = gRenderEngine->fMeshes.at(i);
		Material & mat = mesh.fMat;

		// materials.
		GLuint vao = -1;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh.fVertices.size() * sizeof(glm::vec3), &mesh.fVertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &gRenderEngine->fIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gRenderEngine->fIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.fIndices.size() * sizeof(unsigned int), &mesh.fIndices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &gRenderEngine->fNormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, gRenderEngine->fNormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh.fNormals.size() * sizeof(glm::vec3), &mesh.fNormals[0], GL_STATIC_DRAW);

		glGenBuffers(1, &gRenderEngine->fUVBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, gRenderEngine->fUVBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh.fUVs.size() * sizeof(glm::vec2), &mesh.fUVs[0], GL_STATIC_DRAW);

		/*GLuint colorBuffer;
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, gColors.size() * sizeof(glm::vec3), &gColors[0], GL_STATIC_DRAW);
		*/

		gRenderEngine->fVertexPos = glGetAttribLocation(gRenderEngine->fShader->GetProgram(), "inPositions");
		gRenderEngine->fNormalPos = glGetAttribLocation(gRenderEngine->fShader->GetProgram(), "inNormals");
		gRenderEngine->fUVPos = glGetAttribLocation(gRenderEngine->fShader->GetProgram(), "inUV");
		//gColorPos = glGetAttribLocation(gRenderEngine->gShader->GetProgram(), "inColors");

		glEnableVertexAttribArray(gRenderEngine->fVertexPos);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(gRenderEngine->fVertexPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glEnableVertexAttribArray(gRenderEngine->fNormalPos);
		glBindBuffer(GL_ARRAY_BUFFER, gRenderEngine->fNormalBuffer);
		glVertexAttribPointer(gRenderEngine->fNormalPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glEnableVertexAttribArray(gRenderEngine->fUVPos);
		glBindBuffer(GL_ARRAY_BUFFER, gRenderEngine->fUVBuffer);
		glVertexAttribPointer(gRenderEngine->fUVPos, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

		Batch *batch = new Batch( (unsigned int)vao, mesh.fVertices, mesh.fIndices, mesh.fNormals, (unsigned int)mesh.fMat.fGLTexID, mesh.fUVs, Shader::eShaderTexture );
		//Batch(unsigned int ID, const std::vector<glm::vec3> & vertices, const std::vector<unsigned int> & indices, std::vector<glm::vec3> & normals, unsigned int glTexID, std::vector<glm::vec2> & UVs, Shader::EShaderKind kind);
		gRenderEngine->fVAOs.insert( std::pair<int, Batch *>((int)vao, batch) );

		printOpenGLError();

	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	printOpenGLError();

	
	//gRenderEngine->ComputeRenderMat();
	printOpenGLError();

	glutMainLoop();

	}
	catch(char * e) {
		std::cout << e << std::endl;
	}
	return 0;
}
