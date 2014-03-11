#pragma once

#include <glm.hpp>

class Camera
{
public:
	Camera(void);
	~Camera(void);

	glm::mat4 GetModel();
	glm::mat4 GetProj();
	glm::mat4 GetView();

	void SetCamera(glm::vec3 eyePos);
	enum direction {
		left,
		right,
		up,
		down
	};
	void Move(direction dir);
	void Rotate(const glm::vec2 & degree);

protected:

	// camera position
	glm::vec3 fEyePos;
	glm::vec3 fViewDir;

	// transformation matrices
	glm::mat4 fViewMat;
	glm::mat4 fProjMat;
	glm::mat4 fModelMat;
};

