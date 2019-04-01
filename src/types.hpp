#ifndef TYPES_H
#define TYPES_H

#include <glm/glm.hpp>

struct BoundingBox
{
	glm::fvec3 center;
};

struct BoundingSphere
{
	float radius;
	glm::fvec3 center;
};

#endif
