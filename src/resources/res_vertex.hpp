#ifndef RES_VERTEX_H
#define RES_VERTEX_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>


namespace Resources {
	struct Vertex
	{
		glm::fvec3 position;
		glm::fvec3 normal;
		glm::fvec3 tangent;
		glm::fvec3 bitangent;
		glm::fvec2 texCoords;
	};
}

#endif