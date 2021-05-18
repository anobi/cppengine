#ifndef __LOADER__MODEL_LOADER_H__
#define __LOADER__MODEL_LOADER_H__

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "../world.hpp"
#include "../rendering/material_manager.hpp"
#include "../entities/entity_handle.hpp"


enum class LOADINGSTATE {
    INIT,
    VALID,
    INVALID
};


class ModelLoader {
public:
    ModelLoader(World* world, Rendering::MaterialManager* material_manager) 
    { 
        this->world = world;
        this->material_manager = material_manager;
    };

    ~ModelLoader() {};

    // TODO: proper inputs would be modelFile, *modelPool, *materialPool

    LOADINGSTATE Load(const char* modelFile, entityHandle_t entity);
    void ProcessNode(const aiNode* node, const aiScene* scene, entityHandle_t entity);
    void ProcessMesh(const aiMesh* mesh, const aiScene* scene, entityHandle_t entity);

    materialHandle_t ProcessMaterial(const aiMaterial* aiMat);

private:
    World* world;
    Rendering::MaterialManager* material_manager;
};

#endif // __LOADER__MODEL_LOADER_H_