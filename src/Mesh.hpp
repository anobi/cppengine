#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <glm/glm.hpp>

#include "lib/Types.hpp"
#include "lib/OpenGL.hpp"

class Mesh {
public:

	Mesh();
	~Mesh();

	glm::tvec3<vertex_t> vertices;
	glm::tvec3<GLuint> indices;
	glm::tvec1<texture_t> textures;

	Mesh(glm::tvec3<vertex_t> vertices, glm::tvec3<GLuint> indices, glm::tvec1<texture_t> textures);

	void Draw();

private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	void Setup();
};

#endif

/*
reminder:

#ifdef _WIN32
renderEntity.shader = GLUtils::LoadShader("../shaders/test_vert.glsl", "../shaders/test_frag.glsl");
#else
renderEntity.shader = LoadShader("shaders/test_vert.glsl", "shaders/test_frag.glsl");
#endif
*/