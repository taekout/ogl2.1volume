#include "RenderEngine.h"
#include <tuple>
#include <assert.h>

#include <GL/glew.h>
#include "glut.h"

#include "Batch.h"
#include "TextureMgr.h"

RenderEngine * gRenderEngine = NULL;

inline int GetTexID(int texNo)
{
	return (gRenderEngine->fTextureMgr->fTextures.size() > texNo) ? gRenderEngine->fTextureMgr->fTextures[texNo]->fTexID : -1;
}

inline int GetTexActiveNo(int texNo)
{
	return (gRenderEngine->fTextureMgr->fTextures.size() > texNo) ? gRenderEngine->fTextureMgr->fTextures[texNo]->fActiveTexNo : -1;
}

void RenderScene()
{
	// Problem is that the shadow map is left-right flipped. Why?
	printOpenGLError();
	gRenderEngine->RecompileShaderIfNecessary();
	gRenderEngine->ActivateMoveIfKeyPressed();
	printOpenGLError();

	gRenderEngine->SetupRenderTarget();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	gRenderEngine->RenderBatch(*gRenderEngine->GetLightCamera(), 0, Shader::eShaderShadowMapGeneration, std::string(), -1, -1);
	for(size_t i = 1 ; i < gRenderEngine->fVBOs.size() ; i++) {
		Batch * b = gRenderEngine->fVBOs[i];
		gRenderEngine->RenderBatch(*gRenderEngine->GetLightCamera(), i, Shader::eShaderShadowMapGeneration, std::string(), -1, -1);
	}
	gRenderEngine->SetdownRenderTarget();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int texID = GetTexID(1);
	int activeTexNo = GetTexActiveNo(1);
	gRenderEngine->RenderBatch(*gRenderEngine->GetCamera(), 0, Shader::eShaderBasicWithShadow, std::string("shadowMap"), activeTexNo, texID);
	for(size_t i = 1 ; i < gRenderEngine->fVBOs.size() ; i++) {
		Batch * b = gRenderEngine->fVBOs[i];
		texID = GetTexID(1);
		activeTexNo = GetTexActiveNo(1);
		gRenderEngine->RenderBatch(*gRenderEngine->GetCamera(), i, Shader::eShaderBasicWithShadow, std::string("shadowMap"), activeTexNo, texID);
	}

	glutSwapBuffers();
	glutPostRedisplay();
	
	printOpenGLError();
}

void ChangeSize(int width, int height)
{
	gRenderEngine->WindowWidth(width);
	gRenderEngine->WindowHeight(height);
	glViewport(0, 0, width, height);
}

IGraphicsEngine::IGraphicsEngine(void)
{
}


RenderEngine::RenderEngine() : fShader(NULL), fInput(NULL), fCamera(NULL), fLightCamera(NULL), fMeshAccess(NULL), fLights(NULL), fTextureID(-1), fTextureMgr(NULL), fWidth(0), fHeight(0)
{
	GLInit();
	Init();
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

void RenderEngine::Init()
{
	fTextureMgr = new TextureMgr();
}

void RenderEngine::GLInit()
{

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("Volume Raycasting 3D");

	glutDisplayFunc(RenderScene);
	//glutIdleFunc(renderScene);
	glutReshapeFunc(ChangeSize);

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
		if(fCamera && fLightCamera)
			fInput = new UserInput(fCamera, fLightCamera);
		else
			throw "Camera is NULL and trying to initialize user input object.";
	}
}

void RenderEngine::SetCamera(const glm::vec3 & eyepos, const glm::vec3 & viewDir,
							 const glm::vec3 & lightPos, const glm::vec3 & lightViewDir)
{
	if(fCamera)
		delete fCamera;
	if(fLightCamera)
		delete fLightCamera;
	fCamera = new Camera(eyepos, viewDir);
	fLightCamera = new Camera(lightPos, lightViewDir);
}


/*
void RenderEngine::SetLightCamera(const glm::vec3 & eyepos, float horizonAngle, float verticalAngle)
{
	if(fLightCamera)
		delete fLightCamera;
	fLightCamera = new Camera(eyepos, horizonAngle, verticalAngle);
}
*/

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

void RenderEngine::AddLight(glm::vec3 & pos, glm::vec3 & lightDir, glm::vec3 intensity)
{
	fLights = new Light();
	fLights->AddLight(pos, lightDir, intensity);
}

void RenderEngine::ComputeRenderMat(Camera & cam)
{
	glm::mat4 model = cam.GetModel();
	glm::mat4 view = cam.GetView();
	glm::mat4 proj = cam.GetProj();
	glm::mat4 normalMat = glm::transpose(glm::inverse(view));

	glm::vec3 eyePos = cam.GetEyePos();

	// I should split this part later. Not good to put together computing and updating shader uniforms.
	fShader->UpdateUniformMat4("Proj", &proj[0][0]);
	fShader->UpdateUniformMat4("View", &view[0][0]);
	fShader->UpdateUniformMat4("Model", &model[0][0]);
	fShader->UpdateUniformMat4("NormalMat", &normalMat[0][0]);
	fShader->UpdateUniform3fv("EyePos", eyePos[0], eyePos[1], eyePos[2]);

	glm::mat4 DepthMVP = gRenderEngine->GetLightCamera()->GetProj() * gRenderEngine->GetLightCamera()->GetView() * gRenderEngine->GetLightCamera()->GetModel();
	fShader->UpdateUniformMat4("DepthMVP", &DepthMVP[0][0]);

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0);
	fShader->UpdateUniformMat4("BiasMat", &biasMatrix[0][0]);

#ifdef _DEBUG
	glm::vec4 testVec = view * model * glm::vec4(100.f, 100.f, 100.f, 1.f);
	glm::vec4 testVec2 = proj * testVec;
	glm::vec4 testVec3  = biasMatrix * proj * view * model * glm::vec4(100.f, 100.f, 100.f, 1.f);

	glm::mat4 testMVP = proj * view * model;
#endif

	if(fLights) {
		std::tuple<glm::vec3, glm::vec3, glm::vec3> & lightData = fLights->GetLight(0);

		auto lightDir = std::get<Light::kLightDir>(lightData);
		fShader->UpdateUniform3fv("LightDir", -lightDir[0], -lightDir[1], -lightDir[2]);
	}
}

bool RenderEngine::CreateRenderTarget()
{
	return fFramebuf.CreateFrameBuffer();
}

bool RenderEngine::SetupRenderTarget()
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

Camera * RenderEngine::GetCamera()
{
	if(fInput->ShouldShowLightCamera())
		return fLightCamera;
	else
		return fCamera;
}

Camera * RenderEngine::GetLightCamera()
{
	return fLightCamera;
}

void RenderEngine::CreateBatch(std::vector<glm::vec3> & inVerts, std::vector<unsigned int> & inInds,
				 std::vector<glm::vec3> & inNormals, unsigned int inGLTexID, std::vector<glm::vec2> & inUVs)
{
	printOpenGLError();

	assert(inVerts.size() != 0 && inInds.size() != 0);

	int vertexVBO = -1;
	int indexVBO = -1;
	int normalVBO = -1;
	int UVVBO = -1;

	if( inVerts.size() != 0 && inInds.size() != 0 ) {

		glGenBuffers(1, (unsigned int *)&vertexVBO);
		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, inVerts.size() * sizeof(glm::vec3), &inVerts[0], GL_STATIC_DRAW);

		glGenBuffers(1, (unsigned int *)&indexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, inInds.size() * sizeof(unsigned int), &inInds[0], GL_STATIC_DRAW);
	}
	printOpenGLError();

	if( inNormals.size() != 0 ) {

		glGenBuffers(1, (unsigned int *)&normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, inNormals.size() * sizeof(glm::vec3), &inNormals[0], GL_STATIC_DRAW);
	}
	printOpenGLError();

	if( inUVs.size() != 0 ) {

		glGenBuffers(1, (unsigned int *)&UVVBO);
		glBindBuffer(GL_ARRAY_BUFFER, UVVBO);
		glBufferData(GL_ARRAY_BUFFER, inUVs.size() * sizeof(glm::vec2), &inUVs[0], GL_STATIC_DRAW);
	}
	printOpenGLError();

	Batch *batch = new Batch( vertexVBO, inVerts, indexVBO, inInds, normalVBO, inNormals, UVVBO, inUVs );
	fVBOs.push_back( batch );

	printOpenGLError();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*
void RenderEngine::CreateBatchTmp(std::vector<glm::vec3> & inVerts, std::vector<unsigned int> & inInds,
				 std::vector<glm::vec3> & inNormals, unsigned int inGLTexID, std::vector<glm::vec2> & inUVs, Shader::EShaderKind kind,
				 unsigned int )
{
	fShader->UseProgram(kind); // It might be better to use the program here just avoid opengl error message.

	GLuint vertexPos = -1, normalPos = -1, UVPos = -1;

	assert(inVerts.size() != 0 && inInds.size() != 0);

	if( kind == Shader::eShaderBasic || kind == Shader::eShaderTexture || kind == Shader::eShaderShadow || kind == Shader::eShaderBasicWithShadow ) {

		vertexPos = glGetAttribLocation(fShader->GetProgram(), "inPositions");
		normalPos = glGetAttribLocation(fShader->GetProgram(), "inNormals");
		
		UVPos = glGetAttribLocation(fShader->GetProgram(), "inUV");
	}
	else
		return;
	
	printOpenGLError();

	if( inVerts.size() != 0 && inInds.size() != 0 && vertexPos != -1 ) {

		glEnableVertexAttribArray(vertexPos);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	}
	printOpenGLError();

	if( inNormals.size() != 0 && normalPos != -1 ) {

		glEnableVertexAttribArray(normalPos);
		//glBindBuffer(GL_ARRAY_BUFFER, fNormalBuffer);
		glVertexAttribPointer(normalPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	}
	printOpenGLError();

	if( inUVs.size() != 0 && UVPos  != -1) {

		glEnableVertexAttribArray(UVPos);
		//glBindBuffer(GL_ARRAY_BUFFER, fUVBuffer);
		glVertexAttribPointer(UVPos, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	}
	printOpenGLError();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
*/

void BindVBOForDrawing(const Shader::ShaderData & sd, Batch * batch, Shader::EShaderKind kind)
{
	printOpenGLError();

	if( sd.fVertexID != -1 && batch->fVertexVBO != -1 && batch->fIndexVBO != -1 ) {

		GLuint vertexPos = sd.fVertexID;
		glEnableVertexAttribArray(vertexPos);
		glBindBuffer(GL_ARRAY_BUFFER, batch->fVertexVBO);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->fIndexVBO);
	}
	printOpenGLError();

	if( sd.fNormalID != -1 && batch->fNormalVBO != -1 ) {

		GLuint normalPos = sd.fNormalID;
		glEnableVertexAttribArray(normalPos);
		glBindBuffer(GL_ARRAY_BUFFER, batch->fNormalVBO);
		//glBindBuffer(GL_ARRAY_BUFFER, fNormalBuffer);
		glVertexAttribPointer(normalPos, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	}
	printOpenGLError();

	if( sd.fUVID  != -1 && batch->fUVVBO != -1 ) {

		GLuint UVPos = sd.fUVID;
		glEnableVertexAttribArray(UVPos);
		glBindBuffer(GL_ARRAY_BUFFER, batch->fUVVBO);
		//glBindBuffer(GL_ARRAY_BUFFER, fUVBuffer);
		glVertexAttribPointer(UVPos, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	}
	printOpenGLError();
}

void RenderEngine::RenderBatch(Camera & cam, size_t index, Shader::EShaderKind kind, std::string & texSamplerName, int texActiveNo, int texID)
{
	if(index >= fVBOs.size())
		return;

	printOpenGLError();

	Batch * batch = fVBOs.at(index);
	fShader->UseProgram(kind);
	ComputeRenderMat(cam);

	if(texActiveNo != -1 && texID != -1 && texSamplerName.compare("") != 0) {
		glActiveTexture(GL_TEXTURE0 + texActiveNo);
		fShader->UpdateUniform1i((char *)texSamplerName.c_str(), texActiveNo);
		glBindTexture(GL_TEXTURE_2D, texID);
	}

	BindVBOForDrawing(fShader->GetShaderData(kind), batch, kind);

	glDrawElements(GL_TRIANGLES, batch->fIndices.size(), GL_UNSIGNED_INT, (void *) 0);
	printOpenGLError();
}


