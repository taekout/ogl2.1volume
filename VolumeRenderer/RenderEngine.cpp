#include "RenderEngine.h"
#include <tuple>

#include <GL/glew.h>
#include "glut.h"

#include "Batch.h"

void RenderScene()
{
	printOpenGLError();

	gRenderEngine.ActivateMoveIfKeyPressed();

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

IGraphicsEngine::IGraphicsEngine(void)
{
}


GraphicsEngine::GraphicsEngine() : fShader(NULL), fInput(NULL), fCamera(NULL), fMeshAccess(NULL), fLights(NULL), 
	fVertexPos(-1), fNormalPos(-1), fUVPos(-1), fIndexBuffer(-1), fNormalBuffer(-1), fUVBuffer(-1), fTextureID(-1)
{
	fori(fVAO_ID, 100) {
		fVAO_ID[i] = -1;
	}

	GLInit();
}



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


void GraphicsEngine::GLInit()
{
	if (glewIsSupported("GL_VERSION_3_1"))
		printf("Ready for OpenGL 3.1.\n");
	else {
		printf("OpenGL 3.1 not supported\n");
		exit(1);
	}

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("Volume Raycasting 3D");

	glutDisplayFunc(RenderScene);
	//glutIdleFunc(renderScene);
	//glutReshapeFunc(changeSize);

	glm::vec3 eyepos(66.5f, 30.0f, 0.0f);
	SetCamera(eyepos, -3.141592 / 2, 0);
	AllocateInput();

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

	AllocateShader();
}


GraphicsEngine::~GraphicsEngine(void)
{
	if(fShader)
		delete fShader;
	if(fInput)
		delete fInput;
	if(fCamera)
		delete fCamera;
	if(fMeshAccess)
		delete fMeshAccess;
	if(fLights)
		delete fLights;
}


void GraphicsEngine::AllocateShader()
{
	fShader = new Shader();
}

void GraphicsEngine::AllocateInput()
{
	if(!fInput) {
		if(fCamera)
			fInput = new UserInput(fCamera);
		else
			throw "Camera is NULL and trying to initialize user input object.";
	}
}

void GraphicsEngine::SetCamera(const glm::vec3 & eyepos, float horizonAngle, float verticalAngle)
{
	if(fCamera)
		delete fCamera;
	fCamera = new Camera(eyepos, horizonAngle, verticalAngle);
}

void GraphicsEngine::AllocateMeshAccess(std::string textureFileName, std::string objPath, std::string objFileName)
{
	fTextureID = loadBMP_custom((objPath + textureFileName).c_str());

	fMeshAccess = new MeshAccess;
	fMeshAccess->LoadOBJFile(objPath + objFileName, objPath);

	fMeshAccess->GetMeshData(fMeshes);
}

void GraphicsEngine::AddLight(glm::vec3 & pos, glm::vec3 intensity)
{
	fLights = new Light();
	fLights->AddLight(pos, glm::vec3(0) - pos, intensity);
}

void GraphicsEngine::ComputeRenderMat()
{
	glm::mat4 model = fCamera->GetModel();
	glm::mat4 view = fCamera->GetView();
	glm::mat4 proj = fCamera->GetProj();
	glm::mat4 normalMat = glm::transpose(glm::inverse(view));

	glm::vec3 eyePos = fCamera->GetEyePos();

	// I should split this part later. Not good to put together computing and updating shader uniforms.
	fShader->UpdateUniformMat4("Proj", &proj[0][0]);
	fShader->UpdateUniformMat4("View", &view[0][0]);
	fShader->UpdateUniformMat4("Model", &model[0][0]);
	fShader->UpdateUniformMat4("NormalMat", &normalMat[0][0]);
	fShader->UpdateUniform3fv("EyePos", eyePos[0], eyePos[1], eyePos[2]);

	if(fLights) {
		std::tuple<glm::vec3, glm::vec3, glm::vec3> & lightData = fLights->GetLight(0);

		auto pos = std::get<Light::kLightPos>(lightData);

		fShader->UpdateUniform3fv("LightPos", pos[0], pos[1], pos[2]);
	}

}


bool GraphicsEngine::SetupRenderTarget(const glm::vec3 & pos)
{
	return fFramebuf.SetupRenderTarget();
}


void GraphicsEngine::ActivateMoveIfKeyPressed()
{
	if(fInput->IsLeftPressed()) {
		fInput->Move(EDirection::left);
	}

	if(fInput->IsRightPressed()) {
		fInput->Move(EDirection::right);
	}

	if(fInput->IsUpPressed()) {
		fInput->Move(EDirection::up);
	}

	if(fInput->IsDownPressed()) {
		fInput->Move(EDirection::down);
	}

	if(fInput->IsBackPressed()) {
		fInput->Move(EDirection::backward);
	}

	if(fInput->IsForewardPressed()) {
		fInput->Move(EDirection::forward);
	}
}


void GraphicsEngine::CreateBatch(std::vector<glm::vec3> & inVerts, std::vector<unsigned int> & inInds,
				 std::vector<glm::vec3> & inNormals, std::vector<glm::vec2> & inUVs, Shader::EShaderKind kind)
{
	
	fShader->UseProgram(kind); // I don't know why. It might be better to use the program here just avoid opengl error message.

	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	this->fVertexPos = glGetAttribLocation(fShader->GetProgram(kind), "inPositions");
	this->fNormalPos = glGetAttribLocation(fShader->GetProgram(kind), "inNormals");
	this->fUVPos = glGetAttribLocation(gRenderEngine.fShader->GetProgram(), "inUV");

	if( !inVerts.empty() && !inInds.empty() ) {

		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, inVerts.size() * sizeof(glm::vec3), &inVerts[0], GL_STATIC_DRAW);


		glEnableVertexAttribArray(fVertexPos);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(fVertexPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		GLuint indexBuffer;
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, inInds.size() * sizeof(unsigned int), &inInds[0], GL_STATIC_DRAW);
	}

	if( !inNormals.empty() ) {

		GLuint normalBuffer;
		glGenBuffers(1, &normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, inNormals.size() * sizeof(glm::vec3), &inNormals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(gRenderEngine.fNormalPos);
		glBindBuffer(GL_ARRAY_BUFFER, gRenderEngine.fNormalBuffer);
		glVertexAttribPointer(gRenderEngine.fNormalPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	}

	if( !inUVs.empty() ) {

		GLuint UVBuffer;
		glGenBuffers(1, &UVBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
		glBufferData(GL_ARRAY_BUFFER, inUVs.size() * sizeof(glm::vec2), &inUVs[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(gRenderEngine.fUVPos);
		glBindBuffer(GL_ARRAY_BUFFER, gRenderEngine.fUVBuffer);
		glVertexAttribPointer(gRenderEngine.fUVPos, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // ??? Necessary?
	glBindVertexArray(0);

	Batch * batch = new Batch(vaoID, inVerts, inInds, inNormals, inUVs);
	fVAO.emplace(std::pair<int, Batch *>(vaoID, batch));
}


