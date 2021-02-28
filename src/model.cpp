#include <string.h>
#include <cstdio>

#include "configuration.hpp"
#include "model.hpp"

void Model::Cleanup()
{
    for (int i = 0; i < this->renderMeshes.size(); i++)
    {
        this->renderMeshes[i].Cleanup();
    }

    for (int i = 0; i < this->renderMaterials.size(); i++)
    {
        this->renderMaterials[i].Cleanup();
    }
}

void Model::Render(Shader* shader)
{
    for (int i = 0; i < this->renderMeshes.size(); i++)
    {
        this->renderMaterials[i].Bind(shader);
        this->renderMeshes[i].Draw();
        this->renderMaterials[i].Unbind();
    }
}