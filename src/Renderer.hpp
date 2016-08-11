#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>

#include "Display.hpp"
#include "Entity.hpp"
#include "lib/Camera.hpp"
#include "Light.hpp"

class Renderer {
public:
	Renderer();
	~Renderer();

	bool Init();

	void Render(EntityRef entity);

	inline void AddLight(EntityComponentRef light) { this->mLights.push_back(light); }
	inline std::vector<EntityComponentRef> GetLights() { return this->mLights; }

	std::shared_ptr<Camera> GetCamera() { return this->mMainCamera; }
	void SetCamera(std::shared_ptr<Camera> camera) { this->mMainCamera = camera; }

private:
	std::shared_ptr<Camera> mMainCamera;
	std::vector<EntityComponentRef> mLights;
	std::vector<glm::fmat4> mShadowMaps();
};

#endif
