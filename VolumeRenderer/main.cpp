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

#include "RenderEngine.h"

#define MODELLOADING 1

GraphicsEngine gRenderEngine;


void Keyboard(unsigned char key, int x, int y)
{
	if(gRenderEngine.fInput)
		gRenderEngine.fInput->Keyboard(key, x, y);
	//glutPostRedisplay();
}

void KeyboardUp(unsigned char key, int x, int y)
{
	if(gRenderEngine.fInput)
		gRenderEngine.fInput->KeyboardUp(key, x, y);
	//glutPostRedisplay();
}

void Keyboard(int key, int x, int y)
{
	if(gRenderEngine.fInput)
		gRenderEngine.fInput->Keyboard(key, x, y);
	//glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	if(gRenderEngine.fInput)
		gRenderEngine.fInput->Mouse(button, state, x, y);
	glutPostRedisplay();
}

void MouseMotion(int x, int y)
{
	if(gRenderEngine.fInput)
		gRenderEngine.fInput->MouseMotion(x, y);
	//glutPostRedisplay();
}


void EndGL() 
{
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

static const GLfloat gPlanes[] = {
	-200.f, 0.f, -200.f, // tri1
	200.f, 0.f, -200.f,
	-200.f, 0.f, 200.f,
	200.f, 0.f, 200.f,
};

static const unsigned short gPlaneInds[] = {
	0, 1, 2,
	2, 1, 3
};

static const GLfloat gPlaneNormals[] = {
	0.f, 1.f, 0.f,
	0.f, 1.f, 0.f,
	0.f, 1.f, 0.f,
	0.f, 1.f, 0.f,
};


void ActivateMoveIfKeyPressed()
{
	if(gRenderEngine.fInput->IsLeftPressed()) {
		gRenderEngine.fInput->Move(EDirection::left);
	}

	if(gRenderEngine.fInput->IsRightPressed()) {
		gRenderEngine.fInput->Move(EDirection::right);
	}

	if(gRenderEngine.fInput->IsUpPressed()) {
		gRenderEngine.fInput->Move(EDirection::up);
	}

	if(gRenderEngine.fInput->IsDownPressed()) {
		gRenderEngine.fInput->Move(EDirection::down);
	}

	if(gRenderEngine.fInput->IsBackPressed()) {
		gRenderEngine.fInput->Move(EDirection::backward);
	}

	if(gRenderEngine.fInput->IsForewardPressed()) {
		gRenderEngine.fInput->Move(EDirection::forward);
	}
}


void renderScene(void) {
	printOpenGLError();

	ActivateMoveIfKeyPressed();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, 0);

	gRenderEngine.fShader->UseProgram(Shader::EShaderKind::eShaderBasic);
	gRenderEngine.ComputeRenderMat();
	glBindVertexArray(gRenderEngine.fVAO_ID[0]);
	glDrawElements(GL_TRIANGLES, sizeof(gPlaneInds), GL_UNSIGNED_SHORT, (void *) 0);

	gRenderEngine.fShader->UseProgram(Shader::EShaderKind::eShaderTexture);
	gRenderEngine.ComputeRenderMat();
	glBindTexture(GL_TEXTURE_2D, gRenderEngine.fTextureID);
	
	for(size_t i = 0 ; i < gRenderEngine.fMeshes.size() ; i++) {
		glBindVertexArray(gRenderEngine.fVAO_ID[i + 1]);
		glDrawElements(GL_TRIANGLES, gRenderEngine.fMeshes[i].fIndices.size(), GL_UNSIGNED_SHORT, (void *) 0);
		glBindVertexArray(0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	gRenderEngine.fShader->UseProgram(Shader::eShaderNothing);

	glutSwapBuffers();
	glutPostRedisplay();
	//gDC.gShader->ShaderFileChangeWatcher();
}

void InitGL()
{
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("Volume Raycasting 3D");

	glutDisplayFunc(renderScene);
	//glutIdleFunc(renderScene);
	//glutReshapeFunc(changeSize);

	glm::vec3 eyepos(66.5f, 30.0f, 0.0f);
	gRenderEngine.SetCamera(eyepos, -3.141592 / 2, 0);
	gRenderEngine.AllocateInput();

	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMotion);
	glewInit();

	//glDisable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_TEXTURE_3D);
	//glEnable(GL_TEXTURE_2D);

	glClearColor(0.0, 0.0, 0.0, 1.0);


}


int main(int argc, char **argv) {
	try {

	glutInit(&argc, argv);
	InitGL();
	if (glewIsSupported("GL_VERSION_3_1"))
		printf("Ready for OpenGL 3.1.\n");
	else {
		printf("OpenGL 3.1 not supported\n");
		exit(1);
	}

	gRenderEngine.AddLight(glm::vec3(100.f, 100.f, 100.f), glm::vec3(1.0f, 1.0f, 1.0f));

#if MODELLOADING
	gRenderEngine.AllocateMeshAccess("truck_color.bmp", "./models/L200-OBJ/", "L200-OBJ.obj");
#else
	std::vector<glm::vec3> verts;
	LoadCube(verts);
#endif

	gRenderEngine.AllocateShader();
	printOpenGLError();

	if(gRenderEngine.fMeshes.size() > 99) { printf("too many meshes\n"); exit(-4); }

	glGenVertexArrays(gRenderEngine.fMeshes.size() + 1, gRenderEngine.fVAO_ID);

	gRenderEngine.fShader->UseProgram(Shader::EShaderKind::eShaderBasic);
	printOpenGLError();

	glBindVertexArray(gRenderEngine.fVAO_ID[0]);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gPlanes), gPlanes, GL_STATIC_DRAW);

	glGenBuffers(1, &gRenderEngine.fIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gRenderEngine.fIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gPlaneInds), gPlaneInds, GL_STATIC_DRAW);

	glGenBuffers(1, &gRenderEngine.fNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gRenderEngine.fNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gPlaneNormals), gPlaneNormals, GL_STATIC_DRAW);

	gRenderEngine.fVertexPos = glGetAttribLocation(gRenderEngine.fShader->GetProgram(), "inPositions");
	gRenderEngine.fNormalPos = glGetAttribLocation(gRenderEngine.fShader->GetProgram(), "inNormals");

	glEnableVertexAttribArray(gRenderEngine.fVertexPos);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(gRenderEngine.fVertexPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glEnableVertexAttribArray(gRenderEngine.fNormalPos);
	glBindBuffer(GL_ARRAY_BUFFER, gRenderEngine.fNormalBuffer);
	glVertexAttribPointer(gRenderEngine.fNormalPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glBindVertexArray(0);

	gRenderEngine.fShader->UseProgram(Shader::EShaderKind::eShaderTexture);
	printOpenGLError();

	for(size_t i = 0 ; i < gRenderEngine.fMeshes.size() ; i++) {

		Mesh & mesh = gRenderEngine.fMeshes.at(i);
		Material & mat = mesh.fMat;

		// materials.

		glBindVertexArray(gRenderEngine.fVAO_ID[i + 1]);

		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh.fVertices.size() * sizeof(glm::vec3), &mesh.fVertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &gRenderEngine.fIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gRenderEngine.fIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.fIndices.size() * sizeof(unsigned short), &mesh.fIndices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &gRenderEngine.fNormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, gRenderEngine.fNormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh.fNormals.size() * sizeof(glm::vec3), &mesh.fNormals[0], GL_STATIC_DRAW);

		glGenBuffers(1, &gRenderEngine.fUVBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, gRenderEngine.fUVBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh.fUVs.size() * sizeof(glm::vec2), &mesh.fUVs[0], GL_STATIC_DRAW);

		/*GLuint colorBuffer;
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, gColors.size() * sizeof(glm::vec3), &gColors[0], GL_STATIC_DRAW);
		*/

		gRenderEngine.fVertexPos = glGetAttribLocation(gRenderEngine.fShader->GetProgram(), "inPositions");
		gRenderEngine.fNormalPos = glGetAttribLocation(gRenderEngine.fShader->GetProgram(), "inNormals");
		gRenderEngine.fUVPos = glGetAttribLocation(gRenderEngine.fShader->GetProgram(), "inUV");
		//gColorPos = glGetAttribLocation(gDC.gShader->GetProgram(), "inColors");

		glEnableVertexAttribArray(gRenderEngine.fVertexPos);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(gRenderEngine.fVertexPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glEnableVertexAttribArray(gRenderEngine.fNormalPos);
		glBindBuffer(GL_ARRAY_BUFFER, gRenderEngine.fNormalBuffer);
		glVertexAttribPointer(gRenderEngine.fNormalPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glEnableVertexAttribArray(gRenderEngine.fUVPos);
		glBindBuffer(GL_ARRAY_BUFFER, gRenderEngine.fUVBuffer);
		glVertexAttribPointer(gRenderEngine.fUVPos, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//glEnableVertexAttribArray(gColorPos);
	//glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	//glVertexAttribPointer(gColorPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);


	printOpenGLError();

	
	gRenderEngine.ComputeRenderMat();

	glutMainLoop();

	EndGL();

	}
	catch(char * e) {
		std::cout << e << std::endl;
	}
	return 0;
}
