#include "Mesh.hpp"
#include "lib/Geometry.hpp"

Mesh::Mesh(){}
Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::Init() {

	const float cube[] = {
		//left
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.5f,

		//front
		0.0f, 0.0f, 0.5f,
		0.5f, 0.0f, 0.5f,
		0.0f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,

		//right
		0.5f, 0.0f, 0.5f,
		0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f,

		//back
		0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,

		//top
		0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,

		//bottom
		0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f,
		0.5f, 0.0f, 0.5f
	};

	int count = sizeof(cube) / sizeof(float);
	for (int i = 0; i < count; i += 3) {
		vertex_t vertex;
		vertex.position = glm::fvec3(cube[i], cube[i + 1], cube[i + 2]);
		vertices.push_back(vertex);
	}
}

void Mesh::Load() {

	std::vector<glm::fvec3> positions;
	int vcount = this->vertices.size();
	for (int i = 0; i < vcount; i++) {
		positions.push_back(this->vertices[i].position);
		positions.push_back(glm::fvec3(0.0f, 1.0f, 0.0f));
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::fvec3), positions.data(), GL_STATIC_DRAW);

	shader = Shader("default");

	GLint posAttrib = glGetAttribLocation(shader.Program(), "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(glm::fvec3), 0);
	glEnableVertexAttribArray(posAttrib);

	glBindVertexArray(0);
}

void Mesh::Draw(glm::mat4 mvp) {

	glBindVertexArray(VAO);

	GLuint MatrixID = glGetUniformLocation(shader.Program(), "mvp");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

	GLuint col = glGetUniformLocation(shader.Program(), "inColor");
	glProgramUniform4f(shader.Program(), col, 0.0f, 0.0f, 1.0f, 1.0f);

	shader.Bind();

	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() / 2 * 3);

	glBindVertexArray(0);
}
