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
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,

		//front
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		//right
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,

		//back
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		//top
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		//bottom
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f
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
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::fvec3), &positions[0], GL_STATIC_DRAW);

	#ifdef _WIN32
	shader = GLUtils::LoadShader("../shaders/test_vert.glsl", "../shaders/test_frag.glsl");
	#else
	shader = LoadShader("shaders/test_vert.glsl", "shaders/test_frag.glsl");
	#endif

	GLint posAttrib = glGetAttribLocation(this->shader, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(glm::fvec3), 0);
	glEnableVertexAttribArray(posAttrib);

	GLint colAttrib = glGetAttribLocation(this->shader, "color");
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
		sizeof(glm::fvec3), (void*)(sizeof(glm::fvec3)));
	glEnableVertexAttribArray(colAttrib);


}

void Mesh::Draw() {

	glBindVertexArray(VAO);
	//Load();

	glUseProgram(this->shader);

	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() / 2 * 3);

	glBindVertexArray(0);
}
