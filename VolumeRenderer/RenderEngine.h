#pragma once
#include "Defs.h"
#include "Shader.h"
#include "UserInput.h"
#include "MeshAccess.h"
#include "Light.h"
#include "FrameBuffer.h"

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
	void AllocateMeshAccess(std::string textureFileName, std::string objPath, std::string objFileName);
	void AddLight(glm::vec3 & pos, glm::vec3 intensity);

	void ComputeRenderMat();

	Shader *fShader;
	UserInput * fInput ;
	Camera *fCamera ;
	IMeshAccess *fMeshAccess;
	Light * fLights;

	bool SetupRenderTarget(const glm::vec3 & pos);
	FrameBuffer fFramebuf;

	std::vector<glm::vec3> fColors;
	std::vector<Mesh> fMeshes;

	GLuint fVertexPos;
	GLuint fNormalPos;
	GLuint fUVPos;
	GLuint fIndexBuffer;
	GLuint fNormalBuffer;
	GLuint fUVBuffer;
	GLuint fVAO_ID[100];

	GLuint fTextureID;

	

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
