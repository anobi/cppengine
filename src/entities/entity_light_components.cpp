#include "entity_light_components.hpp"

entityHandle_t EntityLightComponents::AddBaseComponents(entityHandle_t entity, baseLight_t base_light)
{
    entity.light_component_slot = this->entities_top;
    this->entities[this->entities_top]  = entity;

    this->intensities[entity.light_component_slot]  = base_light.intensity;
    this->cutoffs[entity.light_component_slot]      = base_light.cutoff;
    this->colors[entity.light_component_slot]       = base_light.color;

    this->entities_top += 1;

    return entity;
}

entityHandle_t EntityLightComponents::AddDirectionalLight(entityHandle_t entity, dirLight_t light_desc)
{
    if (entity.valid()) {
        entity = AddBaseComponents(entity, light_desc.light);
        this->light_types[entity.light_component_slot] = lightTypes::DIRECTIONAL_LIGHT;
        this->directions[entity.light_component_slot] = light_desc.direction;
    }

    return entity;
}

entityHandle_t EntityLightComponents::AddPointLight(entityHandle_t entity, pointLight_t light_desc)
{
    if (entity.valid()) {
        entity = AddBaseComponents(entity, light_desc.light);
        this->light_types[entity.light_component_slot] = lightTypes::POINTLIGHT;
        this->radiuses[entity.light_component_slot] = light_desc.radius;
    }

    return entity;
}
