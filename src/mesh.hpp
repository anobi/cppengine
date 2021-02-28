#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>

#include "types.hpp"
#include "opengl.hpp"
#include "entitycomponent.hpp"
#include "material.hpp"
#include "shader.hpp"

class Mesh
{
public:
    std::vector<std::shared_ptr<Texture>> textures;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<std::shared_ptr<Texture>> textures);
    ~Mesh();

    void Draw(Shader* shader);
    void Cleanup();
    void SetupMesh();

    // TODO: These belong into a RenderModel class
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};

#endif
