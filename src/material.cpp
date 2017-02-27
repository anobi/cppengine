#include "material.hpp"

#define STB_IMAGE_IMPLEMENTATION    
#include "lib/stb_image.h"

Material::Material() : EntityComponent() 
{
	diffuse = glm::fvec3(0.8f);

	specular = glm::fvec3(1.0f);
	specularStrength = 0.5f;

	emissive = glm::fvec3(0.0f);
	emissiveStrength = 0.0f;

	heightStrength = 1.0f;
}

Material::~Material() 
{
	// Don't delete the textures here, do it on renderer's destructor IMO
	// If the textures are stored as pointers, we can skip reloading them
	// and reuse if necessary
}

void Material::Render(Renderer & renderer) 
{
}

void Material::LoadMaps(aiMaterial* aiMat) {

	for (int i = 0; i < aiMat->GetTextureCount(aiTextureType_DIFFUSE); i++)
	{
		aiString texFile;
		aiMat->GetTexture(aiTextureType_DIFFUSE, i, &texFile);

		char path[256];
		snprintf(path, 256, "res/textures/%s", texFile.C_Str());

		LoadMap(path, DIFFUSE_MAP);
	}
}

void Material::LoadMap(const std::string filename, TextureType type) 
{
	int width, height, numComponents;
	unsigned char* data = stbi_load((filename).c_str(), &width, &height, &numComponents, 4);

	// Should prolly be a reference that gets loaded to material's texture list
	// then we can reuse old already loaded textures instead of creating duplicates
	Texture texture;

	texture.filename = filename;
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);


	switch (type) 
	{
	case DIFFUSE_MAP:
		texture.type = "diffuse";
		break;
	case SPECULAR_MAP:
		texture.type = "specular";
		useSpecularMap = true;
		break;
	case NORMAL_MAP:
		texture.type = "normal";
		useNormalMap = true;
		break;
	case HEIGHT_MAP:
		texture.type = "height";
		useHeightMap = true;
		break;
	case EMISSIVE_MAP:
		texture.type = "emissive";
		useEmissiveMap = true;
		break;
	default:
		break;
	}

	this->textures.push_back(texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}
