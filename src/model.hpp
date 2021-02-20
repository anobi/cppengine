#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <memory>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "opengl.hpp"
#include "entitycomponent.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "material.hpp"

class Model : public EntityComponent
{
public:
    Model(const std::string fileName);
    ~Model();

    void Update() {};
    void Render(Renderer &renderer) {};
    void Render(std::shared_ptr<Shader> shader);
    void Cleanup();
    
private:

    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<Material>> materials;

    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void ProcessNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Material> ProcessMaterials(aiMaterial* aiMat);
    std::shared_ptr<Texture> LoadCachedTexture(const std::string texFile, TextureType type);
    std::shared_ptr<Texture> LoadTexture(const std::string filename, TextureType type);
};

#endif
