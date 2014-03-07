#pragma once

#include <glm.hpp>

class Camera
{
public:
	Camera(void);
	~Camera(void);

	void SetCamera(glm::vec3 eyePos);

protected:

	glm::vec3 fEyePos;
};

