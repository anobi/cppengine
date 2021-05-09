#ifndef __ENTITIES__ENTITY_LIGHT_COMPONENTS_H__
#define __ENTITIES__ENTITY_LIGHT_COMPONENTS_H__

#include <glm/glm.hpp>
#include "../constants.hpp"
#include "../containers/array.hpp"
#include "entity_handle.hpp"

enum lightTypes 
{
    NONE,
    SPOTLIGHT,
    POINTLIGHT,
    DIRECTIONAL_LIGHT
};

struct baseLight_t 
{
    float intensity;
    float cutoff;
    glm::fvec3 color;
};

struct dirLight_t 
{
    baseLight_t light;
    glm::fvec3 direction;  // Should be tied to the entity rotation
};

struct pointLight_t 
{
    baseLight_t light;
    float radius; 
};

class EntityLightComponents 
{
public:
    entityHandle_t AddDirectionalLight(entityHandle_t entity, dirLight_t light_desc);
    entityHandle_t AddPointLight(entityHandle_t entity, pointLight_t light_desc);

    unsigned int entities_top;
    Array<entityHandle_t, MAX_GAME_ENTITIES>    entities;
    Array<lightTypes, MAX_GAME_ENTITIES>        light_types;

    // Base light components
    Array<float, MAX_GAME_ENTITIES>             intensities;
    Array<float, MAX_GAME_ENTITIES>             cutoffs;
    Array<glm::fvec3, MAX_GAME_ENTITIES>        colors;
    Array<glm::fvec3, MAX_GAME_ENTITIES>        directions; // Directional light components
    Array<float, MAX_GAME_ENTITIES>             radiuses;   // Spotlight components

private:
    entityHandle_t AddBaseComponents(entityHandle_t entity, baseLight_t base);
};

#endif