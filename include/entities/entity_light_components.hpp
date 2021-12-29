#ifndef __ENTITIES__ENTITY_LIGHT_COMPONENTS_H__
#define __ENTITIES__ENTITY_LIGHT_COMPONENTS_H__

#include <glm/glm.hpp>
#include "../types.hpp"
#include "../constants.hpp"
#include "../containers/array.hpp"
#include "entity_handle.hpp"



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
    void AddDirectionalLight(entityHandle_t entity, dirLight_t light_desc);
    void AddPointLight(entityHandle_t entity, pointLight_t light_desc);

    
    unsigned int _entities_top = 0;
    Array<entitySlot_t, MAX_GAME_ENTITIES> _entity_index;
    entitySlot_t AllocateResource(entityHandle_t entity);
    entitySlot_t FindResource(entityHandle_t entity);


    Array<lightTypes, MAX_GAME_ENTITIES>        light_types;

    // Base light components
    Array<float, MAX_GAME_ENTITIES>             intensities;
    Array<float, MAX_GAME_ENTITIES>             cutoffs;
    Array<glm::fvec3, MAX_GAME_ENTITIES>        colors;
    Array<glm::fvec3, MAX_GAME_ENTITIES>        directions; // Directional light components
    Array<float, MAX_GAME_ENTITIES>             radiuses;   // Spotlight components

private:
    entitySlot_t AddBaseComponents(entityHandle_t entity, baseLight_t base);
};

#endif