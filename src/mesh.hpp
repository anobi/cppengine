#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "opengl.hpp"
#include "entitycomponent.hpp"
#include "renderer.hpp"

struct Vertex {

public:
	Vertex() {
		this->position = glm::fvec3();
		this->normal = glm::fvec3();
		this->texCoords = glm::fvec2();
	}

	Vertex(const glm::fvec3 position, const glm::fvec3 normal, const glm::fvec2 texCoords) {
		this->position = position;
		this->normal = normal;
		this->texCoords = texCoords;
	}

	inline glm::fvec3* Position() { return &this->position; }
	inline glm::fvec3* Normal() { return &this->normal; }
	inline glm::fvec2* TexCoords() { return &this->texCoords; }

private:
	glm::fvec3 position;
	glm::fvec3 normal;
	glm::fvec2 texCoords;
};

class Model {
public:
	std::vector<glm::fvec3> positions;
	std::vector<glm::fvec3> normals;
	std::vector<glm::fvec3> tangents;
	std::vector<glm::fvec3> bitangents;
	std::vector<glm::fvec2> texCoords;
	std::vector<unsigned int> indices;

	//void CalculateNormals();
};

class Mesh : public EntityComponent {
public:
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int *indices, unsigned int numIndices);
	Mesh(const std::string fileName);
	Mesh();
	~Mesh();

	void Render(Renderer& renderer);

private:

	static const unsigned int NUM_BUFFERS = 6;
	unsigned int mNumIndices;

	GLuint mVAO;
	GLuint mVBOs[NUM_BUFFERS];

	void SetupMesh(Model &model);
	void Draw();
};

#endif
