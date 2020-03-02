#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include "camera.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "core/light.hpp"

class Scene {

public:

	Scene() {}
	~Scene() {}

	inline void AddModel(std::shared_ptr<Model> model) { this->_models.push_back(model); }
	inline std::vector<std::shared_ptr<Model>> GetModels() { return this->_models; }

	inline void AddPointLight(std::shared_ptr<Core::PointLight> light) { this->_pointLights.push_back(light); }
	inline std::vector<std::shared_ptr<Core::PointLight>> GetPointLights() { return this->_pointLights; }

	inline void AddDirectionalLight(std::shared_ptr<Core::DirectionalLight> light) { this->_directionalLights.push_back(light); }
	inline std::vector<std::shared_ptr<Core::DirectionalLight>> GetDirectionalLights() { return this->_directionalLights; }

	inline void SetCamera(std::shared_ptr<Camera> camera) { this->_camera = camera; }
	inline std::shared_ptr<Camera> GetCamera() { return this->_camera; }

private:
	std::shared_ptr<Camera> _camera;
	std::vector<std::shared_ptr<Core::PointLight>> _pointLights;
	std::vector<std::shared_ptr<Core::DirectionalLight>> _directionalLights;
	std::vector<std::shared_ptr<Model>> _models;
};
#endif
