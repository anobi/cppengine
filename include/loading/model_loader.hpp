#ifndef __LOADER__MODEL_LOADER_H__
#define __LOADER__MODEL_LOADER_H__

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "../world.hpp"
#include "../entities/entity_manager.hpp"
#include "../rendering/material_manager.hpp"
#include "../rendering/model_manager.hpp"


enum class LOADINGSTATE {
    INIT,
    VALID,
    INVALID
};


class ModelLoader {
public:
    ModelLoader(World* world) 
    { 
        this->world = world;
    };

    ~ModelLoader() 
    {
        this->world = nullptr;
    };

    // TODO: proper inputs would be modelFile, *modelPool, *materialPool

    LOADINGSTATE Load(const char* modelFile, entityHandle_t entity);
    void ProcessNode(const aiNode* node, const aiScene* scene, entityHandle_t entity);
    void ProcessMesh(const aiMesh* mesh, const aiScene* scene, entityHandle_t entity);

    materialHandle_t ProcessMaterial(const aiMaterial* aiMat);

private:
    World* world = nullptr;
};

#endif // __LOADER__MODEL_LOADER_H_