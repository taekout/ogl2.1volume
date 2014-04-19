#include "Light.h"
#include <tuple>

Light::Light()
{
}


Light::~Light(void)
{
}


void Light::AddLight(const glm::vec3 & lightPos, const glm::vec3 & intensity)
{
	fLights.push_back(std::make_tuple(lightPos, intensity));
}

std::tuple<glm::vec3 /* light pos*/, glm::vec3 /* light intensity */> & Light::GetLight(size_t index)
{
	if(fLights.size() > index) {
		return fLights.at(index);
	}
	
	return std::tuple<glm::vec3, glm::vec3>();
}
