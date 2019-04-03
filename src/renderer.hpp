#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>

#include "display.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "model.hpp"
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

	void Render(std::shared_ptr<Scene> scene, std::shared_ptr<Shader> shader);
	void Shutdown();

private:

	int m_tick = 0;
	glm::fvec2 resolution;

	std::vector<std::shared_ptr<Shader>> shaders;
};

#endif
