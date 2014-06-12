#pragma once
#include <glm.hpp>
#include <vector>

class Light
{
	std::vector<std::tuple<glm::vec3 /* light pos */, glm::vec3 /*light dir*/, glm::vec3 /* intensity */> > fLights;

public:
	Light();
	virtual ~Light(void);

	void AddLight(const glm::vec3 & lightPos, const glm::vec3 & dir, const glm::vec3 & intensity);
	std::tuple<glm::vec3, glm::vec3, glm::vec3> & GetLight(size_t index);

	glm::vec3 GetLightPos(size_t index);
	glm::vec3 GetLightDir(size_t index);
	glm::vec3 GetLightIntensity(size_t index);

	static const int kLightPos = 0;
	static const int kLightDir = 1;
	static const int kLightIntensity = 2;
};

