#ifndef L_MODEL_LOADER_H
#define L_MODEL_LOADER_H

#include "../model.hpp"
#include "../rendering/render_mesh.hpp"
#include "../rendering/render_material.hpp"


typedef enum {
    LOADINGSTATE_INIT,
    LOADINGSTATE_VALID,
    LOADINGSTATE_INVALID
} loadingState_e;


class ModelLoader {
public:
    ModelLoader() {};
    ~ModelLoader() {};

    // TODO: proper inputs would be modelFile, *modelPool, *materialPool

    loadingState_e Load(const char* modelFile, Model* model);
    void ProcessNode(const aiNode* node, const aiScene* scene, Model* model);
    void ProcessMesh(const aiMesh* mesh, const aiScene* scene, Model* model);

    void ProcessMaterial(const aiMaterial* aiMat, RenderMaterial* material);
    loadingState_e LoadTexture(const char* filename, RenderMaterial* material, GLuint* texture);
};

#endif // L_MODEL_LOADER_H