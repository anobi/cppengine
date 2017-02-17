#include "material.hpp"

#define STB_IMAGE_IMPLEMENTATION    
#include "lib/stb_image.h"

Material::Material() : EntityComponent() {
	mAlbedo = glm::fvec3(1.0f);
	mMetallic = glm::fvec3(0.0f);
	mEmission = glm::fvec3(0.0f);
	mSubsurface = glm::vec3(0.0f);

	// Set default texture maps so we don't get any funky looking effects
	this->SetHeightMap("Default.Height.png");
	this->SetNormalMap("Default.Normal.png");
}

Material::~Material() {
	if(mUseAlbedoMap) glDeleteTextures(1, &mAlbedoMap);
	if(mUseMetallicMap) glDeleteTextures(1, &mMetallicMap);
	if(mUseNormalMap) glDeleteTextures(1, &mNormalMap);
}

void Material::Render(Renderer & renderer) {
	if (mUseAlbedoMap) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mAlbedoMap);
	}

	if (mUseNormalMap) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mNormalMap);
	}

	if (mUseHeightMap) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mHeightMap);
	}
}

void Material::LoadMap(const std::string filename, GLuint & texture) {
	
	int width, height, numComponents;
	unsigned char* data = stbi_load((filename).c_str(), &width, &height, &numComponents, 4);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}
