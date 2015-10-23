#include "Renderer.hpp"

#include <vector>
#include <memory>

#include "lib/OpenGL.hpp"

Renderer::Renderer() {
}

Renderer::~Renderer() {
}

bool Renderer::Init() {
	return true;
}

void Renderer::Shutdown() {
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Renderer::RenderEntities(std::vector<Entity>* entities) {
	int pointcount = 0;

	for (auto e : *entities) {
		RenderEntity(e.renderEntity, &pointcount);
	}

	glDrawArrays(GL_TRIANGLES, 0, pointcount);
}

void Renderer::RenderEntity(renderEntity_t entity, int* pointcount) {

    std::vector<vector3<GLfloat> > vertexData;

    int i = 0;
    for (auto v : entity.vertices) {
		vertexData.push_back(vector3<GLfloat>(v.position.x , v.position.y , v.position.z));
		vertexData.push_back(vector3<GLfloat>(v.color.x , v.color.y , v.color.z));
    }

    *pointcount += vertexData.size() / 2 * 3;

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size(), vertexData.data(), GL_STATIC_DRAW);

    glUseProgram(entity.shader);

    GLint posAttrib = glGetAttribLocation(entity.shader, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 2*sizeof(vector3<GLfloat>), 0);

    GLint colAttrib = glGetAttribLocation(entity.shader, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
		2 * sizeof(vector3<GLfloat>),(void*)(sizeof(vector3<GLfloat>)));
}
