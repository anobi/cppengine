#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "opengl.hpp"
#include "entitycomponent.hpp"
#include "material.hpp"
#include "shader.hpp"

struct Vertex
{
	glm::fvec3 position;
	glm::fvec3 normal;
	glm::fvec3 tangent;
	glm::fvec3 bitangent;
	glm::fvec2 texCoords;
};

class Mesh
{
public:
	GLuint VAO;
	std::vector<Texture> textures;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	~Mesh();

	void Draw(std::shared_ptr<Shader> shader);

private:
	GLuint VBO;
	GLuint EBO;

	void SetupMesh();
};

#endif
