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
	NORMAL_MAP = 6,
	ALPHA_MAP = 7
};

struct Texture
{
	std::string type;
	std::string filename;
	GLuint id;
};

class Material : public EntityComponent 
{

public:

	Material();
	~Material();

	std::vector<std::shared_ptr<Texture>> textures;
	std::string name;

	void Render(Renderer& renderer);

private:
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
};

#endif
