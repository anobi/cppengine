#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include "lib/OpenGL.hpp"
#include "lib/Vertex.hpp"

class Model {
public:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> indices;

	//void CalculateNormals();
};

class Mesh {
public:

	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int *indices, unsigned int numIndices);
	~Mesh();

	void Draw();

private:
	static const unsigned int NUM_BUFFERS = 4;
	unsigned int mNumIndices;

	GLuint mVAO;
	GLuint mVBOs[NUM_BUFFERS];

	void SetupMesh(Model &model);
};

#endif