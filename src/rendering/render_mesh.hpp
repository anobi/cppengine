#ifndef R_RENDER_MESH_H
#define R_RENDER_MESH_H

#include <vector>
#include <glm/glm.hpp>

#include "../types.hpp"


// TODO: This could just be a struct, and the rendering logic should be moved to rendering backend
class RenderMesh
{
public:
    RenderMesh() {};
    ~RenderMesh() {};

    void Setup(const std::vector<Vertex> vertices, const std::vector<unsigned int> indices);
    void Draw();
    void Cleanup();

    int numVertices;
    int numIndices;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};

#endif // R_RENDER_MESH_H