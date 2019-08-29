#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "rendering/gl/opengl.hpp"
#include "entitycomponent.hpp"

struct Texture
{
	std::string type;
	std::string filename;
	unsigned int id;
};

class Material : public EntityComponent 
{

public:

	Material();
	~Material();

	std::vector<std::shared_ptr<Texture>> textures;
	std::string name;

	void Render(Renderer& renderer);
	void Cleanup();

private:
	bool useDiffuseMap;
	bool useSpecularMap;
	bool useNormalMap;
	bool useHeightMap;
	bool useEmissiveMap;
	bool useAplhaMap;

	float specularStrength;
	float emissiveStrength;
	float heightStrength;
	
	glm::fvec3 diffuse;
	glm::fvec3 specular;
	glm::fvec3 emissive;
};

#endif
