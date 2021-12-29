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


void Renderer::Render(RenderWorld render_world, Shader* shader)
{
    shader->Bind();
    glUniform1i(shader->uniforms[6], this->GetTick());

    // TODO: Replace with unified light data
    unsigned int loc = 32; // PointLight uniform offset
    unsigned int dloc = 64; // PointLight uniform offset
    for (int i = 0; i < render_world.light_count; i++)
    {
        if (render_world.light_types[i] == lightTypes::DIRECTIONAL_LIGHT)
        {
            glUniform3fv(shader->uniforms[dloc + 0], 1, &render_world.light_positions[i][0]);
            glUniform3fv(shader->uniforms[dloc + 1], 1, &render_world.light_colors[i][0]);
            glUniform1f(shader->uniforms[dloc + 2], render_world.light_intensities[i]);
            dloc += 3; // Number of light uniforms
        }
        else if (render_world.light_types[i] == lightTypes::POINTLIGHT)
        {
            glUniform3fv(shader->uniforms[loc + 0], 1, &render_world.light_positions[i][0]);
            glUniform3fv(shader->uniforms[loc + 1], 1, &render_world.light_colors[i][0]);
            glUniform1f(shader->uniforms[loc + 2], render_world.light_intensities[i]);
            glUniform1f(shader->uniforms[loc + 3], render_world.light_radiuses[i]);
            glUniform1f(shader->uniforms[loc + 4], render_world.light_cutoffs[i]);
            loc += 5; // Number of light uniforms
        }
    }

    materialHandle_t bound_material;
    for (int i = 0; i < render_world.render_entity_count; i++)
    {  
        glUniformMatrix4fv(shader->uniforms[0], 1, GL_FALSE, &render_world.model_matrices[i][0][0]);
        glUniformMatrix4fv(shader->uniforms[1], 1, GL_FALSE, &render_world.view_matrix[0][0]);
        glUniformMatrix4fv(shader->uniforms[2], 1, GL_FALSE, &render_world.projection_matrix[0][0]);
        glUniformMatrix3fv(shader->uniforms[3], 1, GL_FALSE, &render_world.normal_matrices[i][0][0]);

        glUniform2fv(shader->uniforms[4], 1, &render_world.render_resolution[0]);
        glUniform3fv(shader->uniforms[5], 1, &render_world.eye_position[0]);

        if (bound_material != render_world.materials[i]) {
            this->material_manager->Bind(render_world.materials[i], shader);
            bound_material = render_world.materials[i];
        }

        glBindVertexArray(render_world.VAOs[i]);
        glDrawElements(GL_TRIANGLES, render_world.indices[i], GL_UNSIGNED_INT, (void*)0);
    }

    this->material_manager->Unbind();
    glBindVertexArray(0);
}
