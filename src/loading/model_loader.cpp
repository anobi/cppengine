#include "model_loader.hpp"
#include "../configuration.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

// TODO: move to some tyoe definitions file or something
// Using the more sensible linux max path length here, since 32k+ char limit in NTFS is quite insane
constexpr unsigned int MAX_PATH_LENGTH = 4096;

LOADINGSTATE ModelLoader::Load(const char* modelFile, Model* model, entityHandle_T entity)
{
#ifdef _WIN32 
    const char* resources_dir = "res/";
#else
    const char* resources_dir = "/res/";
#endif


    char path[MAX_PATH_LENGTH];
    const char* cwd = Configuration::Get().workingDirectory.c_str();
    snprintf(path, MAX_PATH_LENGTH, "%s%s%s", cwd, resources_dir, modelFile);

    printf("* Loading model: %s \n", path);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_ValidateDataStructure
        | aiProcess_Triangulate
        | aiProcess_FindInvalidData
        | aiProcess_FindInstances
        | aiProcess_FindDegenerates
        | aiProcess_JoinIdenticalVertices
        | aiProcess_OptimizeGraph
        | aiProcess_OptimizeMeshes
        | aiProcess_SortByPType
        | aiProcess_SplitLargeMeshes
        | aiProcess_RemoveRedundantMaterials
        | aiProcess_GenUVCoords
        | aiProcess_GenSmoothNormals
        | aiProcess_FixInfacingNormals
        | aiProcess_FlipUVs
        | aiProcess_CalcTangentSpace
    );

    if (scene == NULL)
    {
        printf("  !! ERROR: Unable to load model: %s \n", path);
        return LOADINGSTATE::INVALID;
    }

    this->ProcessNode(scene->mRootNode, scene, model, entity);

    return LOADINGSTATE::VALID;
}

void ModelLoader::ProcessNode(const aiNode* node, const aiScene* scene, Model* model, entityHandle_T entity)
{
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
        this->ProcessMesh(aiMesh, scene, model, entity);
    }

    for (int i = 0; i < node->mNumChildren; i++)
    {
        this->ProcessNode(node->mChildren[i], scene, model, entity);
    }
}

void ModelLoader::ProcessMesh(const aiMesh* mesh, const aiScene* scene, Model* model, entityHandle_T entity)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        //positions
        if (mesh->HasPositions())
        {
            aiVector3D vert = mesh->mVertices[i];
            vertex.position = glm::fvec3(vert.x, vert.y, vert.z);
        }

        //normals
        if (mesh->HasNormals())
        {
            aiVector3D norm = mesh->mNormals[i];
            vertex.normal = glm::fvec3(norm.x, norm.y, norm.z);
        }

        //normals
        if (mesh->HasTangentsAndBitangents())
        {
            aiVector3D t = mesh->mTangents[i];
            aiVector3D b = mesh->mBitangents[i];
            vertex.tangent = glm::fvec3(t.x, t.y, t.z);
            vertex.bitangent = glm::fvec3(b.x, b.y, b.z);
        }

        if (mesh->mTextureCoords[0])
        {
            //texture coordinates
            aiVector3D uv = mesh->mTextureCoords[0][i];
            vertex.texCoords = glm::fvec2(uv.x, uv.y);
        }
        else
        {
            vertex.texCoords = glm::fvec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    //indices
    if (mesh->HasFaces())
    {
        for (unsigned int j = 0; j < mesh->mNumFaces; j++)
        {
            const aiFace& face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++)
            {
                indices.push_back(face.mIndices[k]);
            }
        }
    }

    RenderMesh renderMesh = RenderMesh();
    RenderMaterial renderMaterial = RenderMaterial();

    if (entity.id != INVALID_HANDLE_ID) {
        this->world->render_entities.LoadModel(entity, vertices, indices);
    }
    else {
        renderMesh.Setup(vertices, indices);
        model->renderMeshes.push_back(renderMesh);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
        this->ProcessMaterial(aiMat, &renderMaterial);
        model->renderMaterials.push_back(renderMaterial);
    }
}

void ModelLoader::ProcessMaterial(const aiMaterial* aiMat, RenderMaterial* material)
{
    // Name the material so we can utilize the material cache
    aiString matName;
    aiMat->Get(AI_MATKEY_NAME, matName);

    // TODO: Dry. Loop a types list and do same stuff for every map. Don't repeat it like that.
    for (int i = 0; i < aiMat->GetTextureCount(aiTextureType_DIFFUSE); i++)
    {
        aiString texFile;
        aiMat->GetTexture(aiTextureType_DIFFUSE, i, &texFile);
        this->LoadTexture(texFile.C_Str(), material, &material->diffuseMap);
    }

    for (int i = 0; i < aiMat->GetTextureCount(aiTextureType_SPECULAR); i++)
    {
        aiString texFile;
        aiMat->GetTexture(aiTextureType_SPECULAR, i, &texFile);
        this->LoadTexture(texFile.C_Str(), material, &material->specularMap);
    }

    for (int i = 0; i < aiMat->GetTextureCount(aiTextureType_HEIGHT); i++)
    {
        aiString texFile;
        aiMat->GetTexture(aiTextureType_HEIGHT, i, &texFile);
        this->LoadTexture(texFile.C_Str(), material, &material->normalMap);
    }

    for (int i = 0; i < aiMat->GetTextureCount(aiTextureType_OPACITY); i++)
    {
        aiString texFile;
        aiMat->GetTexture(aiTextureType_OPACITY, i, &texFile);
        this->LoadTexture(texFile.C_Str(), material, &material->alphaMap);
    }
}

LOADINGSTATE ModelLoader::LoadTexture(const char* filename, RenderMaterial* material, GLuint* texture)
{

#ifdef _WIN32 
    const char* textures_dir = "res/textures/";
#else
    const char* textures_dir = "/res/textures/";
#endif

    // TODO: move to some tyoe definitions file or something
    // Using the more sensible linux max path length here, since 32k+ char limit in NTFS is quite insane
    char path[4096];
    const char* cwd = Configuration::Get().workingDirectory.c_str();
    snprintf(path, 4096, "%s%s%s", cwd, textures_dir, filename);

    int width, height, numComponents;
    unsigned char* data = stbi_load(path, &width, &height, &numComponents, 4);

    if (!data)
    {
        // TODO: Load some ugly generated default texture that doesn't require any files?
        printf("  !! ERROR: Unable to load texture: %s \n", filename);
        return LOADINGSTATE::INVALID;
    }

    material->Setup(texture, width, height, data);

    stbi_image_free(data);

    return LOADINGSTATE::VALID;
}

