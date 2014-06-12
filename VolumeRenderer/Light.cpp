#include "Light.h"
#include <tuple>

Light::Light()
{
}


Light::~Light(void)
{
}

void Light::AddLight(const glm::vec3 & lightPos, const glm::vec3 & dir, const glm::vec3 & intensity)
{
	fLights.push_back(std::make_tuple(lightPos, dir, intensity));
}

std::tuple<glm::vec3, glm::vec3, glm::vec3> & Light::GetLight(size_t index)
{
	if(fLights.size() > index) {
		return fLights.at(index);
	}
	
	throw "Light not exist.";
	return std::tuple<glm::vec3, glm::vec3, glm::vec3>();
}

glm::vec3 Light::GetLightPos(size_t index)
{
	if(fLights.size() > index) {
		return std::get<kLightPos> ( fLights.at(index) );
	}
	throw "Light not exist.";
	return glm::vec3();
}

glm::vec3 Light::GetLightDir(size_t index)
{
	if(fLights.size() > index) {
		return std::get<kLightDir> ( fLights.at(index) );
	}
	throw "Light not exist.";
	return glm::vec3();
}

glm::vec3 Light::GetLightIntensity(size_t index)
{
	if(fLights.size() > index) {
		return std::get<kLightIntensity> ( fLights.at(index) );
	}
	throw "Light not exist.";
	return glm::vec3();
}


