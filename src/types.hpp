#ifndef TYPES_H
#define TYPES_H

#include <glm/glm.hpp>

struct boundingBox
{
	glm::fvec3 center;
};

struct boundingSphere
{
	float radius;
	glm::fvec3 center;
};

#endif
