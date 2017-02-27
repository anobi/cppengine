#include "model.hpp"

#define STB_IMAGE_IMPLEMENTATION    
#include "lib/stb_image.h"

Model::Model(const std::string fileName) : EntityComponent() {

	this->SetName("Mesh");

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fileName,
		aiProcess_Triangulate
		| aiProcess_GenUVCoords
		| aiProcess_GenSmoothNormals
		| aiProcess_JoinIdenticalVertices
		| aiProcess_FlipUVs
		| aiProcess_FixInfacingNormals
		| aiProcess_CalcTangentSpace
		| aiProcess_RemoveRedundantMaterials);

	if (scene == NULL) return;

	this->ProcessNode(scene->mRootNode, scene);
}

Model::~Model()
{
	for (int i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i]->Cleanup();
	}
}

void Model::Render(std::shared_ptr<Shader> shader) 
{
	for (int i = 0; i < this->meshes.size(); i++) 
	{
		this->meshes[i]->Draw(shader);
	}
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
		this->ProcessMesh(aiMesh, scene);
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}

void Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	//construct the vertexes
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
			auto uv = mesh->mTextureCoords[0][i];
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
			const aiFace &face = mesh->mFaces[j];

			for (unsigned int k = 0; k < face.mNumIndices; k++)
			{
				indices.push_back(face.mIndices[k]);
			}
		}
	}

	std::shared_ptr<Material> material = std::make_shared<Material>();
	if (mesh->mMaterialIndex >= 0) 
	{
		aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
		aiString matName;
		aiMat->Get(AI_MATKEY_NAME, matName);

		// Use already loaded materials from cache if possible
		bool skip = false;
		for (int i = 0; i < this->materials.size(); i++)
		{
			if (this->materials[i]->name == matName.C_Str())
			{
				material = materials[i];
				skip = true;
			}
		}

		// Material not in cache, load it
		if (!skip) 
		{
			material = ProcessMaterials(aiMat);
			materials.push_back(material);
		}
	}

	std::shared_ptr<Mesh> m = std::make_shared<Mesh>(Mesh(vertices, indices, material->textures));
	meshes.push_back(m);
}

std::shared_ptr<Material> Model::ProcessMaterials(aiMaterial* aiMat)
{
	std::shared_ptr<Material> material = std::make_shared<Material>();

	// Name the material so we can utilize the material cache
	aiString matName;
	aiMat->Get(AI_MATKEY_NAME, matName);
	material->name = matName.C_Str();

	for (int i = 0; i < aiMat->GetTextureCount(aiTextureType_DIFFUSE); i++)
	{
		aiString texFile;
		aiMat->GetTexture(aiTextureType_DIFFUSE, i, &texFile);
		material->textures.push_back(LoadCachedTexture(texFile.C_Str()));
	}

	for (int i = 0; i < aiMat->GetTextureCount(aiTextureType_SPECULAR); i++)
	{
		aiString texFile;
		aiMat->GetTexture(aiTextureType_SPECULAR, i, &texFile);
		material->textures.push_back(LoadCachedTexture(texFile.C_Str()));;
	}

	if (aiMat->GetTextureCount(aiTextureType_NORMALS) >= 0) {
		for (int i = 0; i < aiMat->GetTextureCount(aiTextureType_NORMALS); i++)
		{
			aiString texFile;
			aiMat->GetTexture(aiTextureType_NORMALS, i, &texFile);
			material->textures.push_back(LoadCachedTexture(texFile.C_Str()));
		}
	}
	else 
	{
		material->textures.push_back(LoadCachedTexture("Default.Normal.png"));
	}

	for (int i = 0; i < aiMat->GetTextureCount(aiTextureType_HEIGHT); i++)
	{
		aiString texFile;
		aiMat->GetTexture(aiTextureType_HEIGHT, i, &texFile);
		material->textures.push_back(LoadCachedTexture(texFile.C_Str()));
	}

	return material;
}

std::shared_ptr<Texture> Model::LoadCachedTexture(const std::string texFile)
{
	std::shared_ptr<Texture> texture;
	bool skip = false;

	char path[256];
	snprintf(path, 256, "res/textures/%s", texFile.c_str());

	for (int j = 0; j < materials.size(); j++)
	{
		for (int t = 0; t < materials[j]->textures.size(); t++)
		{
			if (materials[j]->textures[t]->filename == path)
			{
				texture = materials[j]->textures[t];
				skip = true;
			}
		}
	}

	if (!skip)
	{
		texture = LoadTexture(path, DIFFUSE_MAP);
	}

	return texture;
}

std::shared_ptr<Texture> Model::LoadTexture(const std::string filename, TextureType type)
{
	int width, height, numComponents;
	unsigned char* data = stbi_load((filename).c_str(), &width, &height, &numComponents, 4);


	// Should prolly be a reference that gets loaded to material's texture list
	// then we can reuse old already loaded textures instead of creating duplicates
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	texture->filename = filename;

	switch (type)
	{
	case DIFFUSE_MAP:
		if(data == nullptr)
		{
			data = stbi_load("res/Default.Diffuse.png", &width, &height, &numComponents, 4);
		}
		texture->type = "diffuse";
		break;
	case SPECULAR_MAP:
		if(data == nullptr)
		{
			data = stbi_load("res/Default.Specular.png", &width, &height, &numComponents, 4);
		}
		texture->type = "specular";
		break;
	case NORMAL_MAP:
		if(data == nullptr)
		{
			data = stbi_load("res/Default.Normal.png", &width, &height, &numComponents, 4);
		}
		texture->type = "normal";
		break;
	case HEIGHT_MAP:
		if(data == nullptr)
		{
			data = stbi_load("res/Default.Height.png", &width, &height, &numComponents, 4);
		}
		texture->type = "height";
		break;
	case EMISSIVE_MAP:
		texture->type = "emissive";
		break;
	default:
		break;
	}

	glGenTextures(1, &texture->id);
	glBindTexture(GL_TEXTURE_2D, texture->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	return texture;
}
