#ifndef RES_MESH
#define RES_MESH

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "res_vertex.hpp"

namespace Resources {
	class Mesh
	{
	public:
		std::vector<Resources::Vertex> vertices;
		std::vector<unsigned int> indices;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		~Mesh();

		void Cleanup();

	private:
		void SetupMesh();
	};
}

#endif
