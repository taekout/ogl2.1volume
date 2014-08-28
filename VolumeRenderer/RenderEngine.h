#pragma once
#include "Defs.h"
#include "Shader.h"
#include "UserInput.h"
#include "MeshAccess.h"
#include "Light.h"
#include "FrameBuffer.h"

class Batch;

class IGraphicsEngine
{
public:
	IGraphicsEngine(void);
	virtual ~IGraphicsEngine(void) {}
};


class GraphicsEngine : public IGraphicsEngine
{
public:
	GraphicsEngine(void);
	virtual ~GraphicsEngine(void);

	void AllocateShader();
	void AllocateInput();
	void SetCamera(const glm::vec3 & eyepos, float horizonAngle, float verticalAngle);
	void SetLightCamera(const glm::vec3 & eyepos, float horizonAngle, float verticalAngle);
	void AllocateMeshAccess(std::string textureFileName, std::string objPath, std::string objFileName);
	void AddLight(glm::vec3 & pos, glm::vec3 intensity);

	void ComputeRenderMat();
	void ComputeShadowMat();

	void ActivateMoveIfKeyPressed();

	void CreateBatch(std::vector<glm::vec3> & inVerts, std::vector<unsigned int> & inInds,
		std::vector<glm::vec3> & inNormals, unsigned int inGLTexID, std::vector<glm::vec2> & inUVs, Shader::EShaderKind kind);

	void RenderBatch();

	Shader *fShader;
	UserInput * fInput ;
	Camera *fCamera ;
	Camera *fLightCamera;
	IMeshAccess *fMeshAccess;
	Light * fLights;

	bool SetupRenderTarget(const glm::vec3 & pos);
	FrameBuffer fFramebuf;

	std::vector<glm::vec3> fColors;
	std::vector<Mesh> fMeshes;

	GLint fVertexPos;
	GLint fNormalPos;
	GLint fUVPos;
	GLuint fIndexBuffer;
	GLuint fNormalBuffer;
	GLuint fUVBuffer;
	std::map<int, Batch *> fVAOs;

	GLuint fTextureID;

protected:
	virtual void GLInit();

	

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
