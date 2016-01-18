#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "Mesh.hpp"

Mesh::Mesh(Vertex *vertices, unsigned int numVertices, unsigned int *indices, unsigned int numIndices) {

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

Mesh::Mesh(const std::string fileName) {
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fileName, 0);

	Vertex *vertices;
	unsigned int *indices;
	unsigned int numVertices = 0;
	unsigned int numIndices = 0;

	if (scene->HasMeshes()) {
		//construct the vertexes
		for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
			numVertices += scene->mMeshes[i]->mNumVertices;
		}
	}
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
