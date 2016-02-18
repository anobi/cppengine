#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include "EntityComponent.hpp"

class Light : EntityComponent {
public:

	Light(const glm::vec3& position, const glm::vec3& color, float intensity) 
	{
		this->position = position;
		this->color = color;
		this->intensity = intensity;
	};
	virtual ~Light();

	float intensity;
	float maxDistance;
	glm::vec3 position;
	glm::vec3 color;

private:
};

class DirectionalLight : Light {
	DirectionalLight();
	~DirectionalLight();

	glm::vec3 direction;
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
