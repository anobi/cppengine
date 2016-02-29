#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include "EntityComponent.hpp"

class Light : public EntityComponent {
public:

	Light(const glm::fvec3 color, float intensity, float maxDistance) : EntityComponent()
	{
		this->SetName("Light");
		this->color = color;
		this->intensity = intensity;
		this->maxDistance = maxDistance;
	};
	virtual ~Light() { }

	float intensity;
	float maxDistance;
	glm::fvec3 color;

private:
};

class DirectionalLight : public Light {
	DirectionalLight(const glm::fvec3 color, float intensity, float maxDistance) : Light(color, intensity, maxDistance)
	{
		this->SetName("DirectionalLight");
		this->color = color;
		this->intensity = intensity;
		this->maxDistance = maxDistance;
	};
	~DirectionalLight();

	glm::fvec3 direction;
};

class PointLight : public Light {
public:
	PointLight(const glm::fvec3 color, float intensity, float maxDistance) : Light(color, intensity, maxDistance)
	{
		this->SetName("PointLight");
		this->color = color;
		this->intensity = intensity;
		this->maxDistance = maxDistance;
	};
	virtual ~PointLight() {};
};

class SpotLight : public Light {
public:

	SpotLight(const glm::fvec3 color, float intensity, float maxDistance) : Light(color, intensity, maxDistance)
	{
		this->SetName("SpotLight");
		this->color = color;
		this->intensity = intensity;
		this->maxDistance = maxDistance;
	};
	~SpotLight();

	float angle;
	glm::fvec3 direction;
private:

};

#endif
