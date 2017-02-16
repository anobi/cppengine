#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>

#include "display.hpp"
#include "entity.hpp"
#include "camera.hpp"
#include "light.hpp"

class Renderer {
public:
	Renderer();
	~Renderer();

	bool Init();

	void UpdateTick(const int tick) { this->m_tick = tick; }
	int GetTick() { return this->m_tick; }

	void UpdateResolution(const int w, const int h) { this->resolution = glm::fvec2(w, h); }
	glm::fvec2 GetResolution() { return this->resolution; }

	void Render(EntityRef entity);

	inline void AddLight(EntityComponentRef light) { this->mLights.push_back(light); }
	inline std::vector<EntityComponentRef> GetLights() { return this->mLights; }

	std::shared_ptr<Camera> GetCamera() { return this->mMainCamera; }
	void SetCamera(std::shared_ptr<Camera> camera) { this->mMainCamera = camera; }

private:

	int m_tick = 0;
	glm::fvec2 resolution;

	std::shared_ptr<Camera> mMainCamera;
	std::vector<EntityComponentRef> mLights;
	std::vector<glm::fmat4> mShadowMaps();
};

#endif
