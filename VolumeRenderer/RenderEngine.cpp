#include "RenderEngine.h"
#include <tuple>


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




