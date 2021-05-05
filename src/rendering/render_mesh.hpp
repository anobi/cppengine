#ifndef __RENDERING__RENDER_MESH_H__
#define __RENDERING__RENDER_MESH_H__

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

    unsigned int numVertices    = 0;
    unsigned int numIndices     = 0;
    unsigned int VAO            = 0;
};

#endif  // __RENDERING__RENDER_MESH_H__