#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include "opengl.hpp"
#include "entitycomponent.hpp"

class Material : public EntityComponent {

public:

	Material();
	~Material();

	void Render(Renderer& renderer);

	void SetAlbedo(const glm::fvec3 color) { this->mAlbedo = color; }
	void SetMetallic(const glm::fvec3 metallic) { this->mMetallic = metallic; }
	void SetRoughness(const float smoothness) { this->mRoughness = smoothness; }
	void SetEmission(const glm::fvec3 emission) { this->mEmission = emission; }
	void SetSubsurface(const glm::fvec3 subsurface) { this->mSubsurface = subsurface; }
	void SetOcclusion(const glm::fvec3 occlusion) { this->mOcclusion = occlusion; }

	void SetAlbedoMap(const std::string filename) { 
		LoadMap(filename, mAlbedoMap); 
		mUseAlbedoMap = true; 
	}

	void SetMetallicMap(const std::string filename) { 
		LoadMap(filename, mMetallicMap); 
		mUseNormalMap = true;
	}

	void SetNormalMap(const std::string filename) { 
		LoadMap(filename, mNormalMap); 
		mUseNormalMap = true;
	}

	void SetHeightMap(const std::string filename) { 
		LoadMap(filename, mHeightMap);
		mUseHeightMap = true;
	}

	void SetEmissionMap(const std::string filename) { LoadMap(filename, mEmissionMap); }
	void SetSubsurfaceMap(const std::string filename) { LoadMap(filename, mSubsurfaceMap); }
	void SetOcclusionMap(const std::string filename) { LoadMap(filename, mOcclusionMap); }
	void SetRoughnessMap(const std::string filename) { LoadMap(filename, mRoughnessMap); }

private:

	void LoadMap(const std::string filename, GLuint & texture);

	//might aswell put all the PBR stuff in right away

	bool mUseAlbedoMap;
	bool mUseMetallicMap;
	bool mUseNormalMap;
	bool mUseHeightMap;

	float mRoughness;
	
	glm::fvec3 mAlbedo;
	glm::fvec3 mMetallic;
	glm::fvec3 mEmission;
	glm::fvec3 mSubsurface;
	glm::fvec3 mOcclusion;

	GLuint mAlbedoMap;
	GLuint mMetallicMap;
	GLuint mNormalMap;
	GLuint mHeightMap;
	GLuint mRoughnessMap;
	GLuint mEmissionMap;
	GLuint mSubsurfaceMap;
	GLuint mOcclusionMap;

protected:

};

#endif
