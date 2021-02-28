#include "scene.hpp"

void Scene::Cleanup()
{
    for (unsigned int i = 0; i < this->_pointLights.size(); i++)
    {
        this->_pointLights[i]->Cleanup();
    }

    for (unsigned int i = 0; i < this->_directionalLights.size(); i++)
    {
        this->_directionalLights[i]->Cleanup();
    }

    for (unsigned int i = 0; i < this->_models.size(); i++)
    {
        this->_models[i]->Cleanup();
    }
}
