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

#define MODELLOADING 1



int width, height;

Shader *gShader = NULL;
UserInput * gInput = NULL;
Camera *gCamera = NULL;
IMeshAccess *gMeshAccess = NULL;
Light * gLights = NULL;

std::vector<glm::vec3> gColors;
std::vector<Mesh> gMeshes;

GLuint gVertexPos;
GLuint gNormalPos;
GLuint gUVPos;
//GLuint gColorPos;
GLuint gIndexBuffer;
GLuint gNormalBuffer;
GLuint gUVBuffer;
GLuint gVAO_ID[100];

GLuint gTextureID;

void UpdateRenderMat()
{
	glm::mat4 model = gCamera->GetModel();
	glm::mat4 view = gCamera->GetView();
	glm::mat4 proj = gCamera->GetProj();

	glm::vec3 eyePos = gCamera->GetEyePos();

	GLuint projID = glGetUniformLocation(gShader->GetProgram(), "Proj");
	GLuint viewID = glGetUniformLocation(gShader->GetProgram(), "View");
	GLuint modelID = glGetUniformLocation(gShader->GetProgram(), "Model");
	GLuint eyePosID = glGetUniformLocation(gShader->GetProgram(), "EyePos");

	GLuint lightID = glGetUniformLocation(gShader->GetProgram(), "LightPos");

	glUniformMatrix4fv(projID, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
	glUniform3fv(eyePosID, 1, &eyePos[0]);

	if(gLights) {
		std::tuple<glm::vec3, glm::vec3> & lightData = gLights->GetLight(0);

		const int kLightPos = 0;
		const int kLightIntensity = 0;
		auto pos = std::get<kLightPos>(lightData);

		glUniform3f(lightID, pos[0], pos[1], pos[2]);
	}
	
}



void Keyboard(unsigned char key, int x, int y)
{
	if(gInput)
		gInput->Keyboard(key, x, y);
	//glutPostRedisplay();
}

void KeyboardUp(unsigned char key, int x, int y)
{
	if(gInput)
		gInput->KeyboardUp(key, x, y);
	//glutPostRedisplay();
}

void Keyboard(int key, int x, int y)
{
	if(gInput)
		gInput->Keyboard(key, x, y);
	//glutPostRedisplay();
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
	//glutPostRedisplay();
}

void renderScene(void);
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
	gCamera = new Camera(eyepos, -3.141592 / 2, 0);
	if(!gInput) {
		gInput = new UserInput(gCamera);
	}
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
		//gInds.push_back(i);
	}
}

GLuint loadBMP_custom(const char * imagepath)
{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = NULL;
	fopen_s(&file, imagepath,"rb");
	if (!file) {
		printf("Image could not be opened\n");
		return 0;
	}

	if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}

	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file\n");
		return 0;
	}

	// Read ints from the byte array
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void ActivateMoveIfKeyPressed()
{
	if(gInput->IsLeftPressed()) {
		gInput->Move(EDirection::left);
	}

	if(gInput->IsRightPressed()) {
		gInput->Move(EDirection::right);
	}

	if(gInput->IsUpPressed()) {
		gInput->Move(EDirection::up);
	}

	if(gInput->IsDownPressed()) {
		gInput->Move(EDirection::down);
	}

	if(gInput->IsBackPressed()) {
		gInput->Move(EDirection::backward);
	}

	if(gInput->IsForewardPressed()) {
		gInput->Move(EDirection::forward);
	}
}


void renderScene(void) {
	printOpenGLError();

	ActivateMoveIfKeyPressed();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, 0);

	gShader->UseProgram(Shader::EShaderKind::eShaderBasic);
	UpdateRenderMat();
	glBindVertexArray(gVAO_ID[0]);
	glDrawElements(GL_TRIANGLES, sizeof(gPlaneInds), GL_UNSIGNED_SHORT, (void *) 0);

	gShader->UseProgram(Shader::EShaderKind::eShaderTexture);
	UpdateRenderMat();
	glBindTexture(GL_TEXTURE_2D, gTextureID);
	
	for(size_t i = 0 ; i < gMeshes.size() ; i++) {
		glBindVertexArray(gVAO_ID[i + 1]);
		glDrawElements(GL_TRIANGLES, gMeshes[i].fIndices.size(), GL_UNSIGNED_SHORT, (void *) 0);
		glBindVertexArray(0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	glutSwapBuffers();
	glutPostRedisplay();
	//gShader->ShaderFileChangeWatcher();
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

	gLights = new Light();
	gLights->AddLight(glm::vec3(100.f, 100.f, 100.f), glm::vec3(1.0f, 1.0f, 1.0f));

#if MODELLOADING
	gTextureID = loadBMP_custom("./models/L200-OBJ/truck_color.bmp");

	gMeshAccess = new MeshAccess;
	gMeshAccess->LoadOBJFile(std::string("./models/L200-OBJ/L200-OBJ.obj"), std::string("./models/L200-OBJ/"));

	gMeshAccess->GetMeshData(gMeshes);
#else
	std::vector<glm::vec3> verts;
	LoadCube(verts);
#endif

	gShader = new Shader();	
	printOpenGLError();

	if(gMeshes.size() > 99) { printf("too many meshes\n"); exit(-4); }

	glGenVertexArrays(gMeshes.size() + 1, gVAO_ID);

	gShader->UseProgram(Shader::EShaderKind::eShaderBasic);
	printOpenGLError();

	glBindVertexArray(gVAO_ID[0]);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gPlanes), gPlanes, GL_STATIC_DRAW);

	glGenBuffers(1, &gIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gPlaneInds), gPlaneInds, GL_STATIC_DRAW);

	glGenBuffers(1, &gNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gPlaneNormals), gPlaneNormals, GL_STATIC_DRAW);

	gVertexPos = glGetAttribLocation(gShader->GetProgram(), "inPositions");
	gNormalPos = glGetAttribLocation(gShader->GetProgram(), "inNormals");

	glEnableVertexAttribArray(gVertexPos);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(gVertexPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glEnableVertexAttribArray(gNormalPos);
	glBindBuffer(GL_ARRAY_BUFFER, gNormalBuffer);
	glVertexAttribPointer(gNormalPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glBindVertexArray(0);

	gShader->UseProgram(Shader::EShaderKind::eShaderTexture);
	printOpenGLError();

	for(size_t i = 0 ; i < gMeshes.size() ; i++) {

		Mesh & mesh = gMeshes.at(i);
		Material & mat = mesh.fMat;

		// materials.

		glBindVertexArray(gVAO_ID[i + 1]);

		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh.fVertices.size() * sizeof(glm::vec3), &mesh.fVertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &gIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.fIndices.size() * sizeof(unsigned short), &mesh.fIndices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &gNormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, gNormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh.fNormals.size() * sizeof(glm::vec3), &mesh.fNormals[0], GL_STATIC_DRAW);

		glGenBuffers(1, &gUVBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, gUVBuffer);
		glBufferData(GL_ARRAY_BUFFER, mesh.fUVs.size() * sizeof(glm::vec2), &mesh.fUVs[0], GL_STATIC_DRAW);

		/*GLuint colorBuffer;
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, gColors.size() * sizeof(glm::vec3), &gColors[0], GL_STATIC_DRAW);
		*/

		gVertexPos = glGetAttribLocation(gShader->GetProgram(), "inPositions");
		gNormalPos = glGetAttribLocation(gShader->GetProgram(), "inNormals");
		gUVPos = glGetAttribLocation(gShader->GetProgram(), "inUV");
		//gColorPos = glGetAttribLocation(gShader->GetProgram(), "inColors");

		glEnableVertexAttribArray(gVertexPos);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(gVertexPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glEnableVertexAttribArray(gNormalPos);
		glBindBuffer(GL_ARRAY_BUFFER, gNormalBuffer);
		glVertexAttribPointer(gNormalPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glEnableVertexAttribArray(gUVPos);
		glBindBuffer(GL_ARRAY_BUFFER, gUVBuffer);
		glVertexAttribPointer(gUVPos, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
