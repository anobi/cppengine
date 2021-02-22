#include "opengl.hpp"
#include "renderer.hpp"

Renderer::Renderer()
{

}
Renderer::~Renderer()
{
}

void Renderer::Shutdown()
{
}

bool Renderer::Init()
{
    return true;
}

void Renderer::Render(Scene* scene, Shader* shader)
{
    shader->Bind();

    auto models = scene->GetModels();
    auto point_lights = scene->GetPointLights();
    auto dir_lights = scene->GetDirectionalLights();

    //
    // Update dynamic lights
    //
    unsigned int loc = 32; // PointLight uniform offset
    for (unsigned int i = 0; i < point_lights.size(); i++)
    {
        glm::fvec3 lPos = point_lights[i]->transform->GetPosition();

        glUniform3fv(shader->uniforms[loc + 0], 1, &lPos[0]);
        glUniform3fv(shader->uniforms[loc + 1], 1, &point_lights[i]->GetColor()[0]);
        glUniform1f(shader->uniforms[loc + 2], point_lights[i]->GetIntensity());
        glUniform1f(shader->uniforms[loc + 3], point_lights[i]->GetRadius());
        glUniform1f(shader->uniforms[loc + 4], point_lights[i]->GetCutoff());

        loc += 5; // Number of light uniforms
    }

    unsigned int dloc = 64; // PointLight uniform offset
    for (unsigned int i = 0; i < dir_lights.size(); i++)
    {
        glUniform3fv(shader->uniforms[dloc + 0], 1, &dir_lights[i]->transform->GetPosition()[0]);
        glUniform3fv(shader->uniforms[dloc + 1], 1, &dir_lights[i]->GetColor()[0]);
        glUniform1f(shader->uniforms[dloc + 2], dir_lights[i]->GetIntensity());

        dloc += 3; // Number of light uniforms
    }

    //
    // Update and render models
    //

    glm::fvec2 resolution = this->GetResolution();
    glm::fmat4 view = scene->camera->GetView();
    glm::fmat4 projection = scene->camera->GetProjection();
    glm::fvec3 eyePos = scene->camera->transform.GetPosition();

    for (int i = 0; i < models.size(); i++)
    {
        // Update uniforms
        glm::fmat4 model = models[i]->transform->GetModel();

        glm::fmat3 normalMatrix = glm::inverse(glm::fmat3(model));
        glUniformMatrix4fv(shader->uniforms[0], 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(shader->uniforms[1], 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(shader->uniforms[2], 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix3fv(shader->uniforms[3], 1, GL_FALSE, &normalMatrix[0][0]);

        glUniform2fv(shader->uniforms[4], 1, &resolution[0]);
        glUniform3fv(shader->uniforms[5], 1, &eyePos[0]);

        // Time
        glUniform1i(shader->uniforms[6], this->GetTick());

        // Render
        models[i]->Render(shader);
    }

    // Post processing pass here?
}
