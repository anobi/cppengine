#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>

#include "display.hpp"
#include "camera.hpp"
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

	void Render();

	inline void AddShader(std::shared_ptr<Shader> shader) { this->shaders.push_back(shader); }
	inline void AddModel(std::shared_ptr<Model> model) { this->models.push_back(model); }

	inline void AddLight(std::shared_ptr<Light> light) { this->lights.push_back(light); }
	inline std::vector<std::shared_ptr<Light>> GetLights() { return this->lights; }

	inline void SetCamera(std::shared_ptr<Camera> camera) { this->camera = camera; }
	inline std::shared_ptr<Camera> GetCamera() { return this->camera; }

private:

	int m_tick = 0;
	glm::fvec2 resolution;

	std::shared_ptr<Camera> camera;
	std::shared_ptr<Shader> currentShader;
	std::shared_ptr<Shader> GetShader(const std::string name);

	std::vector<std::shared_ptr<Light>> lights;
	std::vector<std::shared_ptr<Model>> models;
	std::vector<std::shared_ptr<Shader>> shaders;
};

#endif
