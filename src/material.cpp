#include "material.hpp"

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

void Material::Cleanup()
{
	for(int i = 0; i < this->textures.size(); i++)
	{
		glDeleteTextures(1, &this->textures[i]->id);
	}
}

void Material::Render(Renderer & renderer) 
{
}
