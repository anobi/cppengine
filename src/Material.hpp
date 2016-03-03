#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include "lib/OpenGL.hpp"
#include "EntityComponent.hpp"

class Material : public EntityComponent {

public:

	Material();
	~Material();

	void SetColor(const glm::fvec3 color) { this->mColor = color; }
	void SetSpecular(const glm::fvec3 specular) { this->mSpecular = specular; }
	void SetMetallic(const glm::fvec3 metallic) { this->mSpecular = metallic; }
	void SetSmoothness(const float smoothness) { this->mSmoothness = smoothness; }
	void SetEmission(const glm::fvec3 emission) { this->mEmission = emission; }
	void SetSubsurface(const glm::fvec3 subsurface) { this->mSubsurface = subsurface; }
	void SetOcclusion(const glm::fvec3 occlusion) { this->mOcclusion = occlusion; }

	void SetColorMap(const std::string filename) { this->mColorMap = LoadMap(filename, mColorMap); }
	void SetSpecularMap(const std::string filename) { this->mSpecularMap = LoadMap(filename, mSpecularMap); }
	void SetMetallicMap(const std::string filename) { this->mMetallicMap = LoadMap(filename, mMetallicMap); }
	void SetNormalMap(const std::string filename) { this->mNormalMap = LoadMap(filename, mNormalMap); }
	void SetEmissionMap(const std::string filename) { this->mEmissionMap = LoadMap(filename, mEmissionMap); }
	void SetSubsurfaceMap(const std::string filename) { this->mSubsurfaceMap = LoadMap(filename, mSubsurfaceMap); }
	void SetOcclusionMap(const std::string filename) { this->mOcclusionMap = LoadMap(filename, mOcclusionMap); }

private:

	GLuint LoadMap(const std::string filename, GLuint & texture);

	//might aswell put all the PBR stuff in right away

	float mSmoothness;
	
	glm::fvec3 mColor;
	glm::fvec3 mSpecular;
	glm::fvec3 mMetallic;
	glm::fvec3 mEmission;
	glm::fvec3 mSubsurface;
	glm::fvec3 mOcclusion;

	GLuint mColorMap;
	GLuint mSpecularMap;
	GLuint mMetallicMap;
	GLuint mNormalMap;
	GLuint mEmissionMap;
	GLuint mSubsurfaceMap;
	GLuint mOcclusionMap;

protected:

};

#endif
