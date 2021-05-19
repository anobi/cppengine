#include "opengl.hpp"
#include "renderer.hpp"
#include "rendering/material_manager.hpp"

void Renderer::Shutdown()
{
}

bool Renderer::Init()
{
    return true;
}


void Renderer::Render(World* world, Shader* shader)
{
    shader->Bind();
    glUniform1i(shader->uniforms[6], this->GetTick());

    glm::fvec2 resolution = this->GetResolution();
    glm::fmat4 view = world->camera->GetView();
    glm::fmat4 projection = world->camera->GetProjection();
    glm::fvec3 eyePos = world->camera->transform.GetPosition();

    // TODO: Replace with unified light data
    unsigned int loc = 32; // PointLight uniform offset
    unsigned int dloc = 64; // PointLight uniform offset
    for (int i = 0; i < world->entity_manager->light_components._entities_top; i++)
    {
        entitySlot_t resource = world->entity_manager->light_components._entity_index[i];
        entitySlot_t spatial_resource = world->entity_manager->spatial_components.FindResource(resource.entity);
        if (world->entity_manager->light_components.light_types[resource.slot] == 3)
        {
            glUniform3fv(shader->uniforms[dloc + 0], 1, &world->entity_manager->spatial_components.positions[spatial_resource.slot][0]);
            glUniform3fv(shader->uniforms[dloc + 1], 1, &world->entity_manager->light_components.colors[resource.slot][0]);
            glUniform1f(shader->uniforms[dloc + 2], world->entity_manager->light_components.intensities[resource.slot]);
            dloc += 3; // Number of light uniforms
        }
        else if (world->entity_manager->light_components.light_types[resource.slot] == 2)
        {
            glUniform3fv(shader->uniforms[loc + 0], 1, &world->entity_manager->spatial_components.positions[spatial_resource.slot][0]);
            glUniform3fv(shader->uniforms[loc + 1], 1, &world->entity_manager->light_components.colors[resource.slot][0]);
            glUniform1f(shader->uniforms[loc + 2], world->entity_manager->light_components.intensities[resource.slot]);
            glUniform1f(shader->uniforms[loc + 3], world->entity_manager->light_components.radiuses[resource.slot]);
            glUniform1f(shader->uniforms[loc + 4], world->entity_manager->light_components.cutoffs[resource.slot]);
            loc += 5; // Number of light uniforms
        }
    }

    materialHandle_t bound_material;
    for (int i = 0; i < this->model_manager->_entities_top; i++)
    {
        entitySlot_t model_resource = this->model_manager->_entity_index[i];
        entitySlot_t spatial_resource = world->entity_manager->spatial_components.FindResource(model_resource.entity);
       
        glUniformMatrix4fv(shader->uniforms[0], 1, GL_FALSE, &world->entity_manager->spatial_components.model_matrices[spatial_resource.slot][0][0]);
        glUniformMatrix4fv(shader->uniforms[1], 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(shader->uniforms[2], 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix3fv(shader->uniforms[3], 1, GL_FALSE, &world->entity_manager->spatial_components.normal_matrices[spatial_resource.slot][0][0]);

        glUniform2fv(shader->uniforms[4], 1, &resolution[0]);
        glUniform3fv(shader->uniforms[5], 1, &eyePos[0]);

        materialHandle_t entity_material; // = world->entity_materials.FindResource(resource.entity);
        if (bound_material != entity_material) {
            this->material_manager->Bind(entity_material, shader);
            bound_material = entity_material;
        }

        glBindVertexArray(this->model_manager->VAOs[model_resource.slot]);
        glDrawElements(GL_TRIANGLES, this->model_manager->indices[model_resource.slot], GL_UNSIGNED_INT, (void*)0);
    }

    this->material_manager->Unbind();
    glBindVertexArray(0);
}
