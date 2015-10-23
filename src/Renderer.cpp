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
    std::vector<vector3<GLfloat> > colorData;

    int vertCount = entity.vertices.size() * 6;
    GLfloat vertices[vertCount];
    
    int i = 0;
    for (auto v : entity.vertices) {
        vertices[i] = v.position.x;
        vertices[i+1] = v.position.y;
        vertices[i+2] = v.position.z;
        vertices[i+3] = v.color.x;
        vertices[i+4] = v.color.y;
        vertices[i+5] = v.color.z;
        i += 6;
    }

    *pointcount += vertCount / 2;

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glUseProgram(entity.shader);

    GLint posAttrib = glGetAttribLocation(entity.shader, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(entity.shader, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                          6*sizeof(GLfloat),(void*)(3*sizeof(GLfloat)));
}
