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

	bool Init(Display* display);

	void Render(Entity& entity, Renderer& renderer);

	inline void AddLight(Light &light) { this->mLights.push_back(&light); }
	inline std::vector<Light*> GetLights() { return this->mLights; }

	inline const Camera& GetCamera() const { return *this->mMainCamera; }
	inline void SetCamera(Camera &camera) { this->mMainCamera = &camera; }

private:
	Display* mDisplay;
	Camera* mMainCamera;
	std::vector<Light*> mLights;
};

#endif
