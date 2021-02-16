#include <memory>
#include "gl_mesh.hpp"

Rendering::GL::GLMesh::GLMesh(std::shared_ptr<Resources::Mesh> mesh) 
{
	this->_num_indices = mesh->indices.size();
	this->_num_vertices = mesh->vertices.size();

	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);
	glGenBuffers(1, &this->_vbo);
	glGenBuffers(1, &this->_ebo);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Resources::Vertex) * mesh->vertices.size(), &mesh->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(GLuint), &mesh->indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Resources::Vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Resources::Vertex), (GLvoid*)offsetof(Resources::Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Resources::Vertex), (GLvoid*)offsetof(Resources::Vertex, tangent));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Resources::Vertex), (GLvoid*)offsetof(Resources::Vertex, bitangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Resources::Vertex), (GLvoid*)offsetof(Resources::Vertex, texCoords));

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

Rendering::GL::GLMesh::~GLMesh() 
{
	this->_textures.clear();

	glDisableVertexAttribArray(0);  // Vertex
	glDisableVertexAttribArray(1);	// Normal
	glDisableVertexAttribArray(2);	// Tangent
	glDisableVertexAttribArray(3);	// Bitangent
	glDisableVertexAttribArray(4);	// Texture coordinates

	glDeleteBuffers(1, &this->_vbo);
	glDeleteBuffers(1, &this->_ebo);
	glDeleteVertexArrays(1, &this->_vao);
}

void Rendering::GL::GLMesh::Draw()
{
	glBindVertexArray(this->_vao);
	glDrawElements(GL_TRIANGLES, this->_num_indices, GL_UNSIGNED_INT, (void*)0);
}

void Rendering::GL::GLMesh::PostDraw()
{
	glBindVertexArray(0);
}
