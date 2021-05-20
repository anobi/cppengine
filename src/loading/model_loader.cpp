#include <numeric>
#include "model_loader.hpp"
#include "../types.hpp"
#include "../configuration.hpp"


// TODO: move to some tyoe definitions file or something
// Using the more sensible linux max path length here, since 32k+ char limit in NTFS is quite insane
constexpr unsigned int MAX_PATH_LENGTH = 4096;

LOADINGSTATE ModelLoader::Load(const char* modelFile, entityHandle_t entity)
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
        // | aiProcess_RemoveComponent
        | aiProcess_JoinIdenticalVertices
        | aiProcess_OptimizeGraph
        | aiProcess_OptimizeMeshes
        | aiProcess_SortByPType
        | aiProcess_SplitLargeMeshes
        // | aiProcess_RemoveRedundantMaterials
        | aiProcess_GenUVCoords
        | aiProcess_GenSmoothNormals
        | aiProcess_FixInfacingNormals
        | aiProcess_FlipUVs
        | aiProcess_CalcTangentSpace
        | aiProcess_ImproveCacheLocality
    );

    if (scene == NULL)
    {
        printf("  !! ERROR: Unable to load model: %s \n", path);
        return LOADINGSTATE::INVALID;
    }

    this->ProcessNode(scene->mRootNode, scene, entity);

    return LOADINGSTATE::VALID;
}

void ModelLoader::ProcessNode(const aiNode* node, const aiScene* scene, entityHandle_t entity)
{
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        if (i > 0) {
            auto child = this->world->entity_manager->AddChild(entity);
            if (child.valid()) 
            {
                this->ProcessMesh(mesh, scene, child);
            }
        }
        else {
            this->ProcessMesh(mesh, scene, entity);
        } 
    }

    for (int i = 0; i < node->mNumChildren; i++)
    {
        this->ProcessNode(node->mChildren[i], scene, entity);
    }
}

void ModelLoader::ProcessMesh(const aiMesh* mesh, const aiScene* scene, entityHandle_t entity)
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

    // TODO: Translate vertices into object space and the entity origin position in the center of the mesh
    // TODO: Also generate and add bounding boxes for each mesh entity

    // Calculate the center position by averaging out the mesh X, Y and Z positions
    // ---
    //std::vector<float> x_positions;
    //std::vector<float> y_positions;
    //std::vector<float> z_positions;
    //for (int i = 0; i < vertices.size(); i++) {
    //    x_positions.push_back(vertices[i].position.x);
    //    y_positions.push_back(vertices[i].position.y);
    //    z_positions.push_back(vertices[i].position.z);
    //}

    //float center_x = std::accumulate(x_positions.begin(), x_positions.end(), 0.0f) / x_positions.size();
    //float center_y = std::accumulate(y_positions.begin(), y_positions.end(), 0.0f) / y_positions.size();
    //float center_z = std::accumulate(z_positions.begin(), z_positions.end(), 0.0f) / z_positions.size();
    //glm::fvec3 center_position = glm::fvec3(center_x, center_y, center_z);
    //// ---

    //// Calculate new vertex posisions offset around the origin
    //for (int i = 0; i < vertices.size(); i++) {
    //    vertices[i].position = glm::fvec3(
    //        vertices[i].position.x - center_position.x,
    //        vertices[i].position.y - center_position.y,
    //        vertices[i].position.z - center_position.z
    //    );
    //}

    //// Translate the entity into the new center position
    //this->world->entity_manager->SetPosition(entity, center_position);

    this->world->model_manager->Add(entity);
    this->world->model_manager->LoadModel(entity, vertices, indices);

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
        auto material = this->ProcessMaterial(aiMat);
        this->world->entity_manager->AddMaterialComponent(entity, material);
    }
}

materialHandle_t ModelLoader::ProcessMaterial(const aiMaterial* aiMat)
{
    // Name the material so we can utilize the material cache
    aiString matName;
    aiMat->Get(AI_MATKEY_NAME, matName);

    // Check if the material has already been loaded
    materialHandle_t handle = this->world->material_manager->Find(matName.C_Str());
    if (handle.valid()) {
        return handle;
    }

    materialHandle_t material = this->world->material_manager->Add(matName.C_Str());

    // TODO: Dry. Loop a types list and do same stuff for every map. Don't repeat it like that.
    for (int i = 0; i < aiMat->GetTextureCount(aiTextureType_DIFFUSE); i++)
    {
        aiString texFile;
        aiMat->GetTexture(aiTextureType_DIFFUSE, i, &texFile);
        this->world->material_manager->LoadTexture(texFile.C_Str(), material, TextureType_e::DIFFUSE);
    }

    for (int i = 0; i < aiMat->GetTextureCount(aiTextureType_SPECULAR); i++)
    {
        aiString texFile;
        aiMat->GetTexture(aiTextureType_SPECULAR, i, &texFile);
        this->world->material_manager->LoadTexture(texFile.C_Str(), material, TextureType_e::SPECULAR);
    }

    for (int i = 0; i < aiMat->GetTextureCount(aiTextureType_HEIGHT); i++)
    {
        aiString texFile;
        aiMat->GetTexture(aiTextureType_HEIGHT, i, &texFile);
        this->world->material_manager->LoadTexture(texFile.C_Str(), material, TextureType_e::NORMAL);
    }

    for (int i = 0; i < aiMat->GetTextureCount(aiTextureType_OPACITY); i++)
    {
        aiString texFile;
        aiMat->GetTexture(aiTextureType_OPACITY, i, &texFile);
        this->world->material_manager->LoadTexture(texFile.C_Str(), material, TextureType_e::ALPHA);
    }

    return material;
}