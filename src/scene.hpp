#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include "camera.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "light.hpp"

class Scene {

public:

    Scene() {}
    ~Scene() {}

    inline void AddModel(Model* model) { this->_models.push_back(model); }
    inline std::vector<Model*> GetModels() { return this->_models; }

    inline void AddPointLight(PointLight* light) { this->_pointLights.push_back(light); }
    inline std::vector<PointLight*> GetPointLights() { return this->_pointLights; }

    inline void AddDirectionalLight(DirectionalLight* light) { this->_directionalLights.push_back(light); }
    inline std::vector<DirectionalLight*> GetDirectionalLights() { return this->_directionalLights; }

    inline void SetCamera(Camera* camera) { this->_camera = camera; }
    inline Camera* GetCamera() { return this->_camera; }

private:
    Camera* _camera;
    std::vector<PointLight*> _pointLights;
    std::vector<DirectionalLight*> _directionalLights;
    std::vector<Model*> _models;
};
#endif
