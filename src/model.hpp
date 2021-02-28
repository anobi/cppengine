#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <vector>

#include "entitycomponent.hpp"
#include "shader.hpp"
#include "rendering/render_mesh.hpp"
#include "rendering/render_material.hpp"

class Model : public EntityComponent
{
public:
    Model() {};
    ~Model() {};

    void Update() {};
    void Render(Shader* shader);
    void Cleanup();

    std::vector<RenderMesh> renderMeshes;
    std::vector<RenderMaterial> renderMaterials;
};

#endif
