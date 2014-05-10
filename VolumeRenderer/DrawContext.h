#pragma once

#include "Shader.h"
#include "UserInput.h"
#include "MeshAccess.h"
#include "Light.h"

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

	void AllocateShader() {
		fShader = new Shader();
	}

	void AllocateInput() {
		if(!fInput) {
			if(fCamera)
				fInput = new UserInput(fCamera);
			else
				throw "Camera is NULL and trying to initialize user input object.";
		}
	}

	void SetCamera(const glm::vec3 & eyepos, float horizonAngle, float verticalAngle) {
		if(fCamera)
			delete fCamera;
		fCamera = new Camera(eyepos, horizonAngle, verticalAngle);
	}

	void AllocateMeshAccess() {
	}

	void AddLight(glm::vec3 & pos, glm::vec3 intensity) {
		fLights = new Light();
		fLights->AddLight(pos, intensity);
	}

	Shader *fShader;
	UserInput * fInput ;
	Camera *fCamera ;
	IMeshAccess *fMeshAccess;
	Light * fLights;

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
