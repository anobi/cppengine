#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "lib/Types.hpp"
#include "Shader.hpp"

class Mesh {
public:

	Mesh();
	~Mesh();

	std::vector<vertex_t> vertices;
	std::vector<GLuint> indices;
	std::vector<texture_t> textures;

	Mesh(std::vector<vertex_t> vertices, std::vector<GLuint> indices, std::vector<texture_t> textures);

	void Init();
	void Load();
	void Draw(glm::mat4 mvp);

private:
	Shader shader;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	void Setup();
};

#endif

/*
reminder:

*/