#include "entity_light_components.hpp"

resourceSlot_t EntityLightComponents::AddBaseComponents(entityHandle_t entity, baseLight_t base_light)
{
    resourceSlot_t resource = this->AllocateResource(entity);

    this->intensities[resource.slot]  = base_light.intensity;
    this->cutoffs[resource.slot]      = base_light.cutoff;
    this->colors[resource.slot]       = base_light.color;

    return resource;
}

void EntityLightComponents::AddDirectionalLight(entityHandle_t entity, dirLight_t light_desc)
{
    if (entity.valid()) {
        resourceSlot_t resource = AddBaseComponents(entity, light_desc.light);
        this->light_types[resource.slot] = lightTypes::DIRECTIONAL_LIGHT;
        this->directions[resource.slot] = light_desc.direction;
    }
}

void EntityLightComponents::AddPointLight(entityHandle_t entity, pointLight_t light_desc)
{
    if (entity.valid()) {
        resourceSlot_t resource = AddBaseComponents(entity, light_desc.light);
        this->light_types[resource.slot] = lightTypes::POINTLIGHT;
        this->radiuses[resource.slot] = light_desc.radius;
    }
}

resourceSlot_t EntityLightComponents::AllocateResource(entityHandle_t entity)
{
    resourceSlot_t resource;
    if (this->_entities_top < MAX_GAME_ENTITIES) {
        resource.slot = this->_entities_top;
        resource.entity = entity;
        this->_entity_index[this->_entities_top] = resource;
        this->_entities_top += 1;
    }
    return resource;
}

resourceSlot_t EntityLightComponents::FindResource(entityHandle_t entity)
{
    for (int i = 0; i < this->_entities_top; i++) {
        if (this->_entity_index[i].entity == entity) {
            return this->_entity_index[i];
        }
    }
    return resourceSlot_t();
}
