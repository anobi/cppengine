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
	Material(std::shared_ptr<Entity> parent);
	Material() {};
	~Material();

	std::vector<std::shared_ptr<Texture>> textures;
	std::string name;

	void Cleanup();

private:
	float specularStrength = 0.5;
	float emissiveStrength = 0.0;
	float heightStrength  = 0.0;
	
	glm::fvec3 diffuse;
	glm::fvec3 specular;
	glm::fvec3 emissive;
};

#endif
