#ifndef __RENDERING__RENDER_WORLD_H__
#define __RENDERING__RENDER_WORLD_H__

#include <vector>
#include <glm/glm.hpp>

#include "../types.hpp"
#include "../constants.hpp"
#include "../entities/entity_handle.hpp"
#include "material_manager.hpp"
#include "model_manager.hpp"



struct RenderWorld 
{
    glm::fvec2 render_resolution;
    glm::fmat4 view_matrix;
    glm::fmat4 projection_matrix;
    glm::fvec3 eye_position;


    unsigned int light_count = 0;
    lightTypes light_types          [MAX_RENDER_LIGHTS];
    float light_intensities         [MAX_RENDER_LIGHTS];
    float light_radiuses            [MAX_RENDER_LIGHTS];
    float light_cutoffs             [MAX_RENDER_LIGHTS];
    glm::fvec3 light_positions      [MAX_RENDER_LIGHTS];
    glm::fvec3 light_colors         [MAX_RENDER_LIGHTS];


    unsigned int render_entity_count = 0;
    unsigned int VAOs               [MAX_RENDER_ENTITIES];
    unsigned int indices            [MAX_RENDER_ENTITIES];
    glm::mat4 model_matrices        [MAX_RENDER_ENTITIES];
    glm::mat4 normal_matrices       [MAX_RENDER_ENTITIES];
    materialHandle_t materials      [MAX_RENDER_ENTITIES];
};

#endif