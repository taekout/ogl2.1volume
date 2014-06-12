#pragma once

#include <glm.hpp>
#include "Defs.h"

class Light;

class Camera
{
public:
	Camera(const glm::vec3 & eyepos, float horizonAngle, float verticalAngle);
	~Camera(void);

	glm::mat4 GetModel();
	glm::mat4 GetProj();
	glm::mat4 GetView();
	glm::vec3 GetEyePos();

	void SetCamera();
	void SetCamera(glm::vec3 eyePos, float horizonAngle, float VerticalAngle);

	void Print();
	void Print(glm::vec3 eyePos, glm::vec2 angles);

	void Move(EDirection dir);
	void Rotate(const glm::vec2 & degree);

	void UpdateRenderMat(/*GraphicsEngine * ge*/);
	void SetMVPForDepth(Light * inLight);

	const float fov;
	const float aspect;
	const float near;
	const float far;

protected:

	// camera position
	glm::vec3 fEyePos;
	float fHorizonAngle;
	float fVerticalAngle;

	// transformation matrices
	glm::mat4 fViewMat;
	glm::mat4 fProjMat;
	glm::mat4 fModelMat;

};

//float Camera::fov = 45.0f;
//float Camera::aspect = 1.0f;
//float Camera::near = 0.1f;
//float Camera::far = 500.0f;
