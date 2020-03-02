#include "res_mesh.hpp"

Resources::Mesh::Mesh(std::vector<Resources::Vertex> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;
}

Resources::Mesh::~Mesh() 
{
	this->vertices.clear();
	this->indices.clear();
}

void Resources::Mesh::Cleanup()
{
}

void Resources::Mesh::SetupMesh()
{
}
