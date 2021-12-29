#include "entities/entity_model_components.hpp"

Entities::ModelComponents::ModelComponents()
{
}

Entities::ModelComponents::~ModelComponents()
{
}

void Entities::ModelComponents::AddMaterial(entityHandle_t entity, materialHandle_t material)
{
    entitySlot_t resource = this->AllocateResource(entity);
    if (resource.valid()) 
    {
        this->materials[resource.slot] = material;
    }
}

materialHandle_t Entities::ModelComponents::GetMaterial(entityHandle_t entity)
{
    entitySlot_t resource = this->AllocateResource(entity);
    if (resource.valid())
    {
        return this->materials[resource.slot];
    }
    return materialHandle_t();
}

entitySlot_t Entities::ModelComponents::AllocateResource(entityHandle_t entity)
{
    entitySlot_t resource;
    if (this->_entities_top < MAX_GAME_ENTITIES) {
        resource.slot = this->_entities_top;
        resource.entity = entity;
        this->_entity_index[this->_entities_top] = resource;
        this->_entities_top += 1;
    }
    return resource;
}

entitySlot_t Entities::ModelComponents::FindResource(entityHandle_t entity)
{
    for (int i = 0; i < this->_entities_top; i++) {
        if (this->_entity_index[i].entity == entity) {
            return this->_entity_index[i];
        }
    }
    return entitySlot_t();
}
