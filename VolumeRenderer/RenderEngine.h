#pragma once
#include "Defs.h"
#include "Shader.h"
#include "UserInput.h"
#include "MeshAccess.h"
#include "Light.h"
#include "FrameBuffer.h"
#include "Batch.h"

class Batch;
class TextureMgr;

class IGraphicsEngine
{
public:
	IGraphicsEngine(void);
	virtual ~IGraphicsEngine(void) {}
};


class RenderEngine : public IGraphicsEngine
{
public:
	RenderEngine(void);
	virtual ~RenderEngine(void);

	void AllocateShader();
	void RecompileShaderIfNecessary();
	void AllocateInput();
	void SetCamera(const glm::vec3 & eyepos, const glm::vec3 & viewDir, const glm::vec3 & lightPos, const glm::vec3 & lightViewDir);
	//void SetLightCamera(const glm::vec3 & eyepos, float horizonAngle, float verticalAngle);
	void AllocateMeshAccess(std::string textureFileName, std::string objPath, std::string objFileName);
	void AddLight(glm::vec3 & pos, glm::vec3 & lightDir, glm::vec3 intensity);

	void ComputeRenderMat(Camera & cam);

	void ActivateMoveIfKeyPressed();
	Camera * GetCamera();
	Camera * GetLightCamera();

	int WindowWidth() { return fWidth; }
	int WindowHeight() { return fHeight; }
	void WindowWidth(int inWidth) { fWidth = inWidth; }
	void WindowHeight(int inHeight) { fHeight = inHeight; }

	void CreateBatch(std::vector<glm::vec3> & inVerts, std::vector<unsigned int> & inInds,
		std::vector<glm::vec3> & inNormals, unsigned int inGLTexID, std::vector<glm::vec2> & inUVs);
	//void CreateBatchTmp(std::vector<glm::vec3> & inVerts, std::vector<unsigned int> & inInds,
	//	std::vector<glm::vec3> & inNormals, unsigned int inGLTexID, std::vector<glm::vec2> & inUVs, Shader::EShaderKind kind);

	size_t BatchSize() { return fVBOs.size(); }
	//unsigned int BatchActiveTexNo(int index) { return fVBOs[index]->fGLActiveTexNo; }
	//unsigned int BatchTexID(int index) { return fVBOs[index]->fGLTexID; }

	void RenderBatch(Camera & cam, size_t index, Shader::EShaderKind kind, std::string & texSamplerName, int texActiveNo, int texID);

	TextureMgr * fTextureMgr;
	Shader *fShader;
	UserInput * fInput ;
	IMeshAccess *fMeshAccess;
	Light * fLights;

	bool CreateRenderTarget();
	bool SetupRenderTarget();
	void SetdownRenderTarget();
	FrameBuffer fFramebuf;

	std::vector<glm::vec3> fColors;
	std::vector<Mesh> fMeshes;

	//GLuint fVertexBuffer;
	//GLuint fIndexBuffer;
	//GLuint fNormalBuffer;
	//GLuint fUVBuffer;
	std::vector<Batch *> fVBOs;

	GLuint fTextureID;

protected:
	virtual void GLInit();
	virtual void Init();

	Camera *fCamera ;
	Camera *fLightCamera;

	int fWidth, fHeight; // for glViewport.

};




//Shader * DrawContext::gShader = NULL;
//UserInput * DrawContext::gInput = NULL;
//Camera * DrawContext::gCamera = NULL;
//IMeshAccess * DrawContext::gMeshAccess = NULL;
//Light * DrawContext::gLights = NULL;

//GLuint DrawContext::gVertexPos = -1;
//GLuint DrawContext::gNormalPos = -1;
//GLuint DrawContext::gUVPos = -1;
//GLuint DrawContext::gIndexBuffer = -1;
//GLuint DrawContext::gNormalBuffer = -1;
//GLuint DrawContext::gUVBuffer = -1;
//GLuint DrawContext::gVAO_ID[100] = {-1};

//GLuint DrawContext::gTextureID = -1;
