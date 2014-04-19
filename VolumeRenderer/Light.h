#pragma once
#include <glm.hpp>
#include <vector>

class Light
{
	std::vector<std::tuple<glm::vec3 /* light pos */, glm::vec3 /* intensity */> > fLights;

public:
	Light();
	virtual ~Light(void);

	void AddLight(const glm::vec3 & lightPos, const glm::vec3 & intensity);
	std::tuple<glm::vec3, glm::vec3> & GetLight(size_t index);
};

