#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <vector>
#include "OpenGL.hpp"

typedef struct {
	glm::fvec3 normal;
	glm::fvec3 position;
	glm::fvec2 texCoords;
} vertex_t;

typedef struct {
	GLuint id;
	std::string type;
} texture_t;

#endif
