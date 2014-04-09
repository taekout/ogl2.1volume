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
#include "MeshAccess.h"

#define MODELLOADING 1

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

Shader *gShader = NULL;
UserInput * gInput = NULL;
Camera *gCamera = NULL;
IMeshAccess *gMeshAccess = NULL;

std::vector<glm::vec3> gColors;
std::vector<unsigned short> gInds;

GLuint gVertexPos;
GLuint gNormalPos;
//GLuint gColorPos;
GLuint gIndexBuffer;
GLuint gNormalBuffer;
GLuint gVAO_ID;

void UpdateRenderMat()
{
	glm::mat4 model = gCamera->GetModel();
	glm::mat4 view = gCamera->GetView();
	glm::mat4 proj = gCamera->GetProj();

	GLuint projID = glGetUniformLocation(gShader->GetProgram(), "Proj");
	GLuint viewID = glGetUniformLocation(gShader->GetProgram(), "View");
	GLuint modelID = glGetUniformLocation(gShader->GetProgram(), "Model");

	GLuint lightID = glGetUniformLocation(gShader->GetProgram(), "lightPos");

	glUniformMatrix4fv(projID, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

	glUniform3f(lightID, 100.0, 100.0, 100.0);
}


void renderScene(void) {
	printOpenGLError();

	UpdateRenderMat();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindVertexArray(gVAO_ID);
	glDrawElements(GL_TRIANGLES, gInds.size(), GL_UNSIGNED_SHORT, (void *) 0);
	glBindVertexArray(0);
	glutSwapBuffers();

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

	glm::vec3 eyepos(50,45,45);
	gCamera = new Camera(eyepos, glm::vec3(0) - eyepos);
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

	glClearColor(0.0, 0.0, 0.0, 1.0);

	
}

void EndGL() 
{
	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	
}

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


void LoadCube(std::vector<glm::vec3> & verts)
{
	for(size_t i = 0 ; i < sizeof(g_cube) / sizeof(float) ; i+=3) {
		// is the index correct?
		verts.push_back(glm::vec3(g_cube[i], g_cube[i+1], g_cube[i+2]));
	}

	for(size_t i = 0 ; i < sizeof(g_cube_colors) / sizeof(float) ; i+=3) {
		// is the index correct?
		gColors.push_back(glm::vec3(g_cube_colors[i], g_cube_colors[i+1], g_cube_colors[i+2]));
	}

	for(size_t i = 0 ; i < sizeof(g_cube) / sizeof(float) / 3 ; i++) {
		gInds.push_back(i);
	}
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

#if MODELLOADING
	gMeshAccess = new MeshAccess;
	gMeshAccess->LoadOBJFile(std::string("./models/L200-OBJ/L200-OBJ.obj"), std::string("./models/L200-OBJ/"));

	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> normals;
	gMeshAccess->Vertices(verts, gInds, normals);
#else
	std::vector<glm::vec3> verts;
	LoadCube(verts);
#endif

	const unsigned int kOutColorID = 0;
	const unsigned int kInPosID= 0;
	const unsigned int kInNormals = 1;
	//const unsigned int kInColorID = 1;
	gShader = new Shader();
	gShader->setShaders("test.vert", "test.frag");
	glBindFragDataLocation(gShader->GetProgram(), kOutColorID, "outColor");
	glBindAttribLocation(gShader->GetProgram(), kInPosID, "inPositions");
	glBindAttribLocation(gShader->GetProgram(), kInNormals, "inNormals");
	//glBindAttribLocation(gShader->GetProgram(), kInColorID, "inColors");

	printOpenGLError();

	gShader->LinkShaders();

	printOpenGLError();

	glGenVertexArrays(1, &gVAO_ID);
	glBindVertexArray(gVAO_ID);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &gIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, gInds.size() * sizeof(unsigned short), &gInds[0], GL_STATIC_DRAW);

	glGenBuffers(1, &gNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, verts.size(), &normals[0], GL_STATIC_DRAW);

	/*GLuint colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, gColors.size() * sizeof(glm::vec3), &gColors[0], GL_STATIC_DRAW);
*/

	gVertexPos = glGetAttribLocation(gShader->GetProgram(), "inPositions");
	gNormalPos = glGetAttribLocation(gShader->GetProgram(), "inNormals");
	//gColorPos = glGetAttribLocation(gShader->GetProgram(), "inColors");
	
	glEnableVertexAttribArray(gVertexPos);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(gVertexPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glEnableVertexAttribArray(gNormalPos);
	glBindBuffer(GL_ARRAY_BUFFER, gNormalBuffer);
	glVertexAttribPointer(gNormalPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	//glEnableVertexAttribArray(gColorPos);
	//glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	//glVertexAttribPointer(gColorPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);


	printOpenGLError();

	
	UpdateRenderMat();

	glutMainLoop();

	EndGL();

	if(gCamera)
		delete gCamera;
	if(gInput)
		delete gInput;
	if(gMeshAccess)
		delete gMeshAccess;

	}

	catch(char * e) {
		std::cout << e << std::endl;
	}
	return 0;
}
