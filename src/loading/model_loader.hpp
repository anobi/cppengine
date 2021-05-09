#ifndef __LOADER__MODEL_LOADER_H__
#define __LOADER__MODEL_LOADER_H__

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "../world.hpp"
#include "../model.hpp"
#include "../rendering/render_mesh.hpp"
#include "../rendering/render_material.hpp"
#include "../entities/entity_handle.hpp"


enum class LOADINGSTATE {
    INIT,
    VALID,
    INVALID
};


class ModelLoader {
public:
    ModelLoader(World* world) { this->world = world; };
    ~ModelLoader() {};

    // TODO: proper inputs would be modelFile, *modelPool, *materialPool

    LOADINGSTATE Load(const char* modelFile, Model* model, entityHandle_t entity);
    void ProcessNode(const aiNode* node, const aiScene* scene, Model* model, entityHandle_t entity);
    void ProcessMesh(const aiMesh* mesh, const aiScene* scene, Model* model, entityHandle_t entity);

    void ProcessMaterial(const aiMaterial* aiMat, RenderMaterial* material);
    LOADINGSTATE LoadTexture(const char* filename, RenderMaterial* material, GLuint* texture);

private:
    World* world;
};

#endif // __LOADER__MODEL_LOADER_H_