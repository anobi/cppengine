#ifndef RES_VERTEX
#define RES_VERTEX

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