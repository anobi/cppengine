#include "Mesh.hpp"

void Mesh::Draw() {
/*
	std::vector<vector3<GLfloat> > vertexData;

	for (auto v : entity.vertices) {
		vertexData.push_back(vector3<GLfloat>(v.position.x, v.position.y, v.position.z));
		vertexData.push_back(vector3<GLfloat>(v.color.x, v.color.y, v.color.z));
	}

	*pointcount += vertexData.size() / 2 * 3;

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(vector3<GLfloat>), vertexData.data(), GL_STATIC_DRAW);

	glUseProgram(entity.shader);

	GLint posAttrib = glGetAttribLocation(entity.shader, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3<GLfloat>), 0);
	glEnableVertexAttribArray(posAttrib);


	GLint colAttrib = glGetAttribLocation(entity.shader, "color");
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
		2 * sizeof(vector3<GLfloat>), (void*)(sizeof(vector3<GLfloat>)));
	glEnableVertexAttribArray(colAttrib);

	glDrawArrays(GL_TRIANGLES, 0, pointcount);
	*/
}
