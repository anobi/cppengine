#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "Display.hpp"
#include "Entity.hpp"
#include "lib/Camera.hpp"
#include "Light.hpp"

class Renderer {
public:
	Renderer();
	~Renderer();

	bool Init(Display &display);

	void Render(EntityRef entity);

	inline void AddLight(EntityComponentRef light) { this->mLights.push_back(light); }
	inline std::vector<EntityComponentRef> GetLights() { return this->mLights; }

	inline Camera* GetCamera() { return this->mMainCamera; }
	inline void SetCamera(Camera &camera) { this->mMainCamera = &camera; }

private:
	Display* mDisplay;
	Camera* mMainCamera;
	std::vector<EntityComponentRef> mLights;
};

#endif
