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

    void Cleanup();

    inline void AddModel(Model* model) { this->_models.push_back(model); }
    inline std::vector<Model*> GetModels() { return this->_models; }

    inline void AddPointLight(PointLight* light) { this->_pointLights.push_back(light); }
    inline std::vector<PointLight*> GetPointLights() { return this->_pointLights; }

    inline void AddDirectionalLight(DirectionalLight* light) { this->_directionalLights.push_back(light); }
    inline std::vector<DirectionalLight*> GetDirectionalLights() { return this->_directionalLights; }

    Camera* camera = 0;

private:
    std::vector<PointLight*> _pointLights;
    std::vector<DirectionalLight*> _directionalLights;
    std::vector<Model*> _models;
};
#endif
