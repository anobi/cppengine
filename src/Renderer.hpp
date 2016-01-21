#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "Display.hpp"
#include "Entity.hpp"
#include "lib/Camera.hpp"
#include "EntityComponents/Light.hpp"
#include "EntityComponents/Shader.hpp"

class Entity;

class Renderer {
public:
	Renderer();
	~Renderer();

	bool Init(Display* display);

	void Render(Entity &entity);

	inline void AddLight(Light &light) { this->mLights.push_back(&light); }
	inline void SetCamera(Camera &camera) { this->mMainCamera = &camera; }

private:
	Display* mDisplay;
	Camera* mMainCamera;
	std::vector<const Light*> mLights;

	Shader mShader;
};

#endif