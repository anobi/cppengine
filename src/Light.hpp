#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include "EntityComponent.hpp"

class Light : EntityComponent{
public:

	Light();
	~Light();

	float intensity;
	float maxDistance;
	glm::fvec3 position;
	glm::fvec3 color;

private:
};

class SpotLight : Light {
public:

	SpotLight();
	~SpotLight();

	float angle;
	glm::fvec3 direction;
private:

};

#endif
