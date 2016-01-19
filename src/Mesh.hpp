#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "lib/OpenGL.hpp"

struct Vertex {

public:
	Vertex() {
		this->position = glm::vec3();
		this->normal = glm::vec3();
		this->texCoords = glm::vec2();
	}

	Vertex(const glm::vec3 position, const glm::vec3 normal, const glm::vec2 texCoords) {
		this->position = position;
		this->normal = normal;
		this->texCoords = texCoords;
	}

	inline glm::vec3 *Position() { return &this->position; }
	inline glm::vec3 *Normal() { return &this->normal; }
	inline glm::vec2 *TexCoords() { return &this->texCoords; }

private:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

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
	Mesh(const std::string fileName);
	Mesh();
	~Mesh();

	void Draw();

private:

	Mesh(const Mesh& other);
	static const unsigned int NUM_BUFFERS = 4;
	unsigned int mNumIndices;

	GLuint mVAO;
	GLuint mVBOs[NUM_BUFFERS];

	void SetupMesh(Model &model);
};

#endif