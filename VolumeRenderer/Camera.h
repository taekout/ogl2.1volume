#pragma once

#include <glm.hpp>

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

	enum direction {
		left,
		right,
		forward,
		backward,
		up,
		down
	};
	void Move(direction dir);
	void Rotate(const glm::vec2 & degree);

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

