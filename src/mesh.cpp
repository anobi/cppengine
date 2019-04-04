#include "mesh.hpp"

Resources::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<std::shared_ptr<Texture>> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
}

Resources::Mesh::~Mesh() 
{
	this->vertices.clear();
	this->indices.clear();
	this->textures.clear();
}