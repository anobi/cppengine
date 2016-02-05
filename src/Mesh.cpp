#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Mesh.hpp"
#include "Shader.hpp"

Mesh::Mesh(){}

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
	const aiScene *scene = importer.ReadFile(fileName,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs |aiProcess_FixInfacingNormals);

	if (scene == NULL) return;

	Model model;

	if (scene->HasMeshes()) {
		for (unsigned int i = 0; i < scene->mNumMeshes; i++) {

			const aiMesh *mesh = scene->mMeshes[i];

			//construct the vertexes
			for (unsigned int v = 0; v < mesh->mNumVertices; v++) {

				//positions
				if (mesh->HasPositions()) {
					aiVector3D vert = scene->mMeshes[i]->mVertices[v];
					model.positions.push_back(glm::vec3(vert.x, vert.y, vert.z));
				}

				//normals
				if (mesh->HasNormals()) {
					aiVector3D norm = mesh->mNormals[v];
					model.normals.push_back(glm::vec3(norm.x, norm.y, norm.z));
				}

				if (mesh->mTextureCoords[0]) {
					//texture coordinates
					auto uv = mesh->mTextureCoords[0][v];
					model.texCoords.push_back(glm::vec2(uv.x, uv.y));
				}
				else {
					model.texCoords.push_back(glm::vec2(0.0f, 0.0f));
				}
			}

			//indices
			if (mesh->HasFaces()) {
				for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
					const aiFace &face = mesh->mFaces[j];

					for (unsigned int k = 0; k < face.mNumIndices; k++) {
						model.indices.push_back(face.mIndices[k]);
					}
				}
			}
		}
		SetupMesh(model);
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
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//normals
	glBindBuffer(GL_ARRAY_BUFFER, mVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//texture coordinates, rename to uv mayhaps?
	glBindBuffer(GL_ARRAY_BUFFER, mVBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBOs[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Draw() {

	glBindVertexArray(this->mVAO);

	glDrawElementsBaseVertex(GL_TRIANGLES, this->mNumIndices, GL_UNSIGNED_INT, 0, 0);

	glBindVertexArray(0);
}