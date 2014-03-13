#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "glut.h"
#include <string>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "UserInput.h"
#include "Camera.h"

#define printOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line)
{
	//
	// Returns 1 if an OpenGL error occurred, 0 otherwise.
	//
	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
		retCode = 1;
		glErr = glGetError();
	}
	return retCode;
}

int width, height;

GLuint vertexArrayID;
GLuint vboID;
GLuint colorID;

Shader *shader = NULL;
UserInput * gInput = NULL;
Camera *gCamera = NULL;

void UpdateRenderMat()
{
	glm::mat4 model = gCamera->GetModel();
	glm::mat4 view = gCamera->GetView();
	glm::mat4 proj = gCamera->GetProj();

	GLuint projID = glGetUniformLocation(shader->GetProgram(), "Proj");
	GLuint viewID = glGetUniformLocation(shader->GetProgram(), "View");
	GLuint modelID = glGetUniformLocation(shader->GetProgram(), "Model");

	glUniformMatrix4fv(projID, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
}


void renderScene(void) {
	printOpenGLError();

	UpdateRenderMat();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindVertexArray(vertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glutSwapBuffers();

	//glBindBuffer(GL_ARRAY_BUFFER, vertexArrayID);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//
	//glDisableVertexAttribArray(0);
}

void Keyboard(unsigned char key, int x, int y)
{
	if(gInput)
		gInput->Keyboard(key, x, y);
	glutPostRedisplay();
}

void Keyboard(int key, int x, int y)
{
	if(gInput)
		gInput->Keyboard(key, x, y);
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	if(gInput)
		gInput->Mouse(button, state, x, y);
	glutPostRedisplay();
}

void MouseMotion(int x, int y)
{
	if(gInput)
		gInput->MouseMotion(x, y);
	glutPostRedisplay();
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

	gCamera = new Camera;
	if(!gInput) {
		gInput = new UserInput(gCamera);
	}
	glutKeyboardFunc(Keyboard);
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

	glClearColor(0.0, 1.0, 0.0, 1.0);

	
}

void EndGL() 
{
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

static const GLfloat g_vertex[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

static const GLfloat g_cube[] = {
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

static const GLfloat g_cube_colors[] = {
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
};

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	InitGL();
	if (glewIsSupported("GL_VERSION_3_1"))
		printf("Ready for OpenGL 3.1.\n");
	else {
		printf("OpenGL 3.1 not supported\n");
		exit(1);
	}

	

	shader = new Shader();
	shader->setShaders("test.vert", "test.frag");
	glBindFragDataLocation(shader->GetProgram(), 0, "out_Color");
	glBindAttribLocation(shader->GetProgram(), 0, "in_Position");
	glBindAttribLocation(shader->GetProgram(), 1, "colors");

	printOpenGLError();

	shader->LinkShaders();

	printOpenGLError();

	GLuint attrloc = glGetAttribLocation(shader->GetProgram(), "in_Position");

	// create vbo
	glGenBuffers(1, &vboID);
	// create vao
	glGenVertexArrays(1, &vertexArrayID);
	// bind vao
	glBindVertexArray(vertexArrayID);
	// enable 0.
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 3 * 2 * 6, g_cube, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &colorID);
	glBindBuffer(GL_ARRAY_BUFFER, colorID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_cube_colors), g_cube_colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);


	printOpenGLError();

	//GLfloat * data = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	//glUnmapBuffer.

	UpdateRenderMat();

	glutMainLoop();

	EndGL();

	if(gCamera)
		delete gCamera;
	if(gInput)
		delete gInput;

	return 0;
}
