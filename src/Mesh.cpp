#include "Mesh.hpp"
#include "lib/Geometry.hpp"

Mesh::Mesh(Vertex *vertices, unsigned int numVertices, unsigned int *indices, unsigned int numIndices) {

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

	Model model;

	for (unsigned int i = 0; i < numVertices; i++) {
		model.positions.push_back(*vertices[i].Position());
		model.normals.push_back(*vertices[i].Normal());
		model.texCoords.push_back(*vertices[i].TexCoords());
	}

	for (unsigned int i = 0; i < numIndices; i++) {
		model.indices.push_back(indices[i]);
	}

	SetupMesh(model);
}

Mesh::~Mesh() {
	glDeleteBuffers(NUM_BUFFERS, mVBOs);
	glDeleteVertexArrays(1, &mVAO);
}

void Mesh::SetupMesh(Model &model) {

	mNumIndices = model.indices.size();

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(NUM_BUFFERS, mVBOs);

	//positions
	glBindBuffer(GL_ARRAY_BUFFER, mVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//texture coordinates, rename to uv mayhaps?
	glBindBuffer(GL_ARRAY_BUFFER, mVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//normals
	glBindBuffer(GL_ARRAY_BUFFER, mVBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBOs[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Draw() {

	glBindVertexArray(mVAO);

	glDrawArrays(GL_TRIANGLES, 0, mNumIndices);

	glBindVertexArray(0);
}
