#pragma once

#include <glm.hpp>

class Camera
{
public:
	Camera(void);
	~Camera(void);

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

protected:

	glm::vec3 fEyePos;
	glm::mat4 fViewMat;
	glm::mat4 fProjMat;
};

