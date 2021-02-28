#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "opengl.hpp"
#include "entitycomponent.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "material.hpp"

#include "rendering/render_mesh.hpp"
#include "rendering/render_material.hpp"

class Model : public EntityComponent
{
public:
    Model() {};
    Model(const char* fileName);
    ~Model() {};

    void Update() {};
    void Render(Renderer& renderer) {};
    void Render(Shader* shader);
    void Cleanup();

    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<Material>> materials;

    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void ProcessNode(aiNode* node, const aiScene* scene);

    // TODO: This stuff will go to material loader or something
    std::shared_ptr<Material> ProcessMaterials(aiMaterial* aiMat);

    // TODO: This will become useless once we start loading unique textures and assigning them to models
    std::shared_ptr<Texture> LoadCachedTexture(const char* texFile, TextureType type);

    // TODO: This will go to RenderMaterial or something
    std::shared_ptr<Texture> LoadTexture(const char* filename, TextureType type);

    // TODO temp trigger to render with old/new way
    bool useNewStuff = false;

    std::vector<RenderMesh> renderMeshes;
    std::vector<RenderMaterial> renderMaterials;
};

#endif
