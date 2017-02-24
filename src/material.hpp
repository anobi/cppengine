#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "opengl.hpp"
#include "entitycomponent.hpp"

enum TextureType 
{
	NONE = 0,
	DIFFUSE_MAP = 1,
	SPECULAR_MAP = 2,
	EMISSIVE_MAP = 4,
	HEIGHT_MAP = 5,
	NORMAL_MAP = 6
};

struct Texture
{
	TextureType type;
	std::string textureFile;
	GLuint textureMap;
};

class Material : public EntityComponent 
{

public:

	Material();
	~Material();

	void Render(Renderer& renderer);

	void LoadMaps(aiMaterial* aiMaterial);
	void LoadMap(const std::string filename, TextureType type);

private:

	std::vector<Texture> textures;

	bool useDiffuseMap;
	bool useSpecularMap;
	bool useNormalMap;
	bool useHeightMap;
	bool useEmissiveMap;

	float specularStrength;
	float emissiveStrength;
	float heightStrength;
	
	glm::fvec3 diffuse;
	glm::fvec3 specular;
	glm::fvec3 emissive;

protected:

};

#endif
