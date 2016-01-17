#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

struct Vertex {

public:
	Vertex() {
		this->position	= glm::vec3();
		this->normal	= glm::vec3();
		this->texCoords = glm::vec2();
	}

	Vertex(const glm::vec3 position, const glm::vec3 normal, const glm::vec2 texCoords) {
		this->position	= position;
		this->normal	= normal;
		this->texCoords = texCoords;
	}

	inline glm::vec3 *Position()	{ return &this->position; }
	inline glm::vec3 *Normal()		{ return &this->normal; }
	inline glm::vec2 *TexCoords()	{ return &this->texCoords; }

private:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

#endif