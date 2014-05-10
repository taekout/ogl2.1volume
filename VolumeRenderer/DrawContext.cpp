#include "DrawContext.h"


IGraphicsEngine::IGraphicsEngine(void)
{
}


GraphicsEngine::GraphicsEngine() : fShader(NULL), fInput(NULL), fCamera(NULL), fMeshAccess(NULL), fLights(NULL), 
	fVertexPos(-1), fNormalPos(-1), fUVPos(-1), fIndexBuffer(-1), fNormalBuffer(-1), fUVBuffer(-1), fTextureID(-1)
{
	fori(fVAO_ID, 100) {
		fVAO_ID[i] = -1;
	}

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

