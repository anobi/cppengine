#include "opengl.hpp"
#include "renderer.hpp"

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
    for (int i = 0; i < world->entity_lights.entities_top; i++) 
    {
        if (world->entity_lights.light_types[i] == 3)
        {
            entityHandle_t entity = world->entity_lights.entities[i];
            glUniform3fv(shader->uniforms[dloc + 0], 1, &world->entity_transforms.positions[entity.slot][0]);
            glUniform3fv(shader->uniforms[dloc + 1], 1, &world->entity_lights.colors[entity.light_component_slot][0]);
            glUniform1f(shader->uniforms[dloc + 2], world->entity_lights.intensities[entity.light_component_slot]);
            dloc += 3; // Number of light uniforms
        }
        else if (world->entity_lights.light_types[i] == 2)
        {
            entityHandle_t entity = world->entity_lights.entities[i];
            glUniform3fv(shader->uniforms[loc + 0], 1, &world->entity_transforms.positions[entity.slot][0]);
            glUniform3fv(shader->uniforms[loc + 1], 1, &world->entity_lights.colors[entity.light_component_slot][0]);
            glUniform1f(shader->uniforms[loc + 2], world->entity_lights.intensities[entity.light_component_slot]);
            glUniform1f(shader->uniforms[loc + 3], world->entity_lights.radiuses[entity.light_component_slot]);
            glUniform1f(shader->uniforms[loc + 4], world->entity_lights.cutoffs[entity.light_component_slot]);
            loc += 5; // Number of light uniforms
        }
    }

    materialHandle_t bound_material;
    for (int i = 0; i < world->_entities_top; i++)
    {
        entityHandle_t entity = world->_entity_handles[i];

        glUniformMatrix4fv(shader->uniforms[0], 1, GL_FALSE, &world->entity_transforms.model_matrices[entity.slot][0][0]);
        glUniformMatrix4fv(shader->uniforms[1], 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(shader->uniforms[2], 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix3fv(shader->uniforms[3], 1, GL_FALSE, &world->entity_transforms.normal_matrices[entity.slot][0][0]);

        glUniform2fv(shader->uniforms[4], 1, &resolution[0]);
        glUniform3fv(shader->uniforms[5], 1, &eyePos[0]);

        if (bound_material.slot != entity.render_material_slot) {
            world->_materials[entity.render_material_slot].Bind(shader);
            bound_material = world->_material_handles[entity.render_material_slot];
        }

        glBindVertexArray(world->render_entities.VAOs[entity.slot]);
        glDrawElements(GL_TRIANGLES, world->render_entities.indices[entity.slot], GL_UNSIGNED_INT, (void*)0);
    }

    world->_materials[bound_material.slot].Unbind();
    glBindVertexArray(0);
}
