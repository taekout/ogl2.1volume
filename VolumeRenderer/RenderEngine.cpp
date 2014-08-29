#include "RenderEngine.h"
#include <tuple>
#include <assert.h>

#include <GL/glew.h>
#include "glut.h"

#include "Batch.h"

RenderEngine * gRenderEngine = NULL;

void RenderScene()
{
	printOpenGLError();
	gRenderEngine->RecompileShaderIfNecessary();

	gRenderEngine->ActivateMoveIfKeyPressed();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gRenderEngine->RenderBatch();

	glutSwapBuffers();
	glutPostRedisplay();
	//gDC.gShader->ShaderFileChangeWatcher();
	
	printOpenGLError();
}

IGraphicsEngine::IGraphicsEngine(void)
{
}


RenderEngine::RenderEngine() : fShader(NULL), fInput(NULL), fCamera(NULL), fLightCamera(NULL), fMeshAccess(NULL), fLights(NULL), 
	fIndexBuffer(-1), fNormalBuffer(-1), fUVBuffer(-1), fTextureID(-1)
{
	GLInit();
}



void Keyboard(unsigned char key, int x, int y)
{
	if(gRenderEngine->fInput)
		gRenderEngine->fInput->Keyboard(key, x, y);
	//glutPostRedisplay();
}

void KeyboardUp(unsigned char key, int x, int y)
{
	if(gRenderEngine->fInput)
		gRenderEngine->fInput->KeyboardUp(key, x, y);
	//glutPostRedisplay();
}

void Keyboard(int key, int x, int y)
{
	if(gRenderEngine->fInput)
		gRenderEngine->fInput->Keyboard(key, x, y);
	//glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	if(gRenderEngine->fInput)
		gRenderEngine->fInput->Mouse(button, state, x, y);
	glutPostRedisplay();
}

void MouseMotion(int x, int y)
{
	if(gRenderEngine->fInput)
		gRenderEngine->fInput->MouseMotion(x, y);
	//glutPostRedisplay();
}


void RenderEngine::GLInit()
{

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("Volume Raycasting 3D");

	glutDisplayFunc(RenderScene);
	//glutIdleFunc(renderScene);
	//glutReshapeFunc(changeSize);

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

	if (glewIsSupported("GL_VERSION_3_1"))
		printf("Ready for OpenGL 3.1.\n");
	else {
		printf("OpenGL 3.1 not supported\n");
		exit(1);
	}

}


RenderEngine::~RenderEngine(void)
{
	if(fShader)
		delete fShader;
	if(fInput)
		delete fInput;
	if(fCamera)
		delete fCamera;
	if(fLightCamera)
		delete fLightCamera;
	if(fMeshAccess)
		delete fMeshAccess;
	if(fLights)
		delete fLights;
}


void RenderEngine::AllocateShader()
{
	fShader = new Shader();
}

void RenderEngine::RecompileShaderIfNecessary()
{
	if(fInput && fInput->ShouldRecompileShader()) {
		if(fShader)
			fShader->CompileAllShaders();
		fInput->ShouldRecompileShader(false);
	}
}

void RenderEngine::AllocateInput()
{
	if(!fInput) {
		if(fCamera)
			fInput = new UserInput(fCamera);
		else
			throw "Camera is NULL and trying to initialize user input object.";
	}
}

void RenderEngine::SetCamera(const glm::vec3 & eyepos, float horizonAngle, float verticalAngle)
{
	if(fCamera)
		delete fCamera;
	fCamera = new Camera(eyepos, horizonAngle, verticalAngle);
}

void RenderEngine::SetLightCamera(const glm::vec3 & eyepos, float horizonAngle, float verticalAngle)
{
	if(fLightCamera)
		delete fLightCamera;
	fLightCamera = new Camera(eyepos, horizonAngle, verticalAngle);
}

void RenderEngine::AllocateMeshAccess(std::string textureFileName, std::string objPath, std::string objFileName)
{
	unsigned texID = loadBMP_custom((objPath + textureFileName).c_str());

	fMeshAccess = new MeshAccess;
	fMeshAccess->LoadOBJFile(objPath + objFileName, objPath);

	fMeshAccess->GetMeshData(fMeshes);
	for(auto it = fMeshes.begin() ; it != fMeshes.end() ; ++it) {
		it->fMat.fGLTexID = texID;
	}
}

void RenderEngine::AddLight(glm::vec3 & pos, glm::vec3 intensity)
{
	fLights = new Light();
	fLights->AddLight(pos, glm::vec3(0) - pos, intensity);
}

void RenderEngine::ComputeRenderMat()
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

void RenderEngine::ComputeShadowMat()
{
	if(fLightCamera) {

		glm::vec3 lightInvDir = fLightCamera->GetEyePos();

		// Compute the MVP matrix from the light's point of view
		glm::mat4 depthProjectionMatrix = fLightCamera->GetProj();
		glm::mat4 depthViewMatrix = fLightCamera->GetView();
		glm::mat4 depthModelMatrix = fLightCamera->GetModel();
		//glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		fShader->UpdateUniformMat4("Proj", &depthProjectionMatrix[0][0]);
		fShader->UpdateUniformMat4("View", &depthViewMatrix[0][0]);
		fShader->UpdateUniformMat4("Model", &depthModelMatrix[0][0]);
	}
}


bool RenderEngine::SetupRenderTarget(const glm::vec3 & pos)
{
	return fFramebuf.SetupRenderTarget();
}

void RenderEngine::SetdownRenderTarget()
{
	fFramebuf.SetdownRenderTarget();
}


void RenderEngine::ActivateMoveIfKeyPressed()
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


void RenderEngine::CreateBatch(std::vector<glm::vec3> & inVerts, std::vector<unsigned int> & inInds,
				 std::vector<glm::vec3> & inNormals, unsigned int inGLTexID, std::vector<glm::vec2> & inUVs, Shader::EShaderKind kind)
{
	
	fShader->UseProgram(kind); // It might be better to use the program here just avoid opengl error message.

	GLuint vao = -1, vertexPos = -1, normalPos = -1, UVPos = -1;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	printOpenGLError();

	assert(inVerts.size() != 0 && inInds.size() != 0);

	if( kind == Shader::eShaderBasic || kind == Shader::eShaderTexture || kind == Shader::eShaderShadow ) {

		vertexPos = glGetAttribLocation(fShader->GetProgram(), "inPositions");
		normalPos = glGetAttribLocation(fShader->GetProgram(), "inNormals");
		
		UVPos = glGetAttribLocation(fShader->GetProgram(), "inUV");
	}
	printOpenGLError();


	if( inVerts.size() != 0 && inInds.size() != 0 && vertexPos != -1 ) {

		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, inVerts.size() * sizeof(glm::vec3), &inVerts[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(vertexPos);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

		glGenBuffers(1, &fIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, inInds.size() * sizeof(unsigned int), &inInds[0], GL_STATIC_DRAW);
	}
	printOpenGLError();

	if( inNormals.size() != 0 && normalPos != -1 ) {

		glGenBuffers(1, &fNormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, fNormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, inNormals.size() * sizeof(glm::vec3), &inNormals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(normalPos);
		glBindBuffer(GL_ARRAY_BUFFER, fNormalBuffer);
		glVertexAttribPointer(normalPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	}
	printOpenGLError();

	if( inUVs.size() != 0 && UVPos  != -1) {

		glGenBuffers(1, &fUVBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, fUVBuffer);
		glBufferData(GL_ARRAY_BUFFER, inUVs.size() * sizeof(glm::vec2), &inUVs[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(UVPos);
		glBindBuffer(GL_ARRAY_BUFFER, fUVBuffer);
		glVertexAttribPointer(UVPos, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	}
	printOpenGLError();

	Batch *batch = new Batch( vao, inVerts, inInds, inNormals, inGLTexID, inUVs, kind );
	fVAOs.emplace( std::pair<int, Batch *>((int)vao, batch) );

	printOpenGLError();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void RenderEngine::RenderBatch()
{
	printOpenGLError();
	glBindTexture(GL_TEXTURE_2D, 0);

	for(auto it = fVAOs.begin() ; it != fVAOs.end() ; ++it) {

		printOpenGLError();
		Batch * batch = it->second;
		fShader->UseProgram(batch->fProgram);
		ComputeRenderMat();
		glBindVertexArray( batch->fID );
		if(batch->fGLTexID)
			glBindTexture(GL_TEXTURE_2D, batch->fGLTexID);
		glDrawElements(GL_TRIANGLES, batch->fIndices.size(), GL_UNSIGNED_INT, (void *) 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		printOpenGLError();

	}
}


