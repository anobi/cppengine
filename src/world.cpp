#include "world.hpp"

entityHandle_T World::AddEntity(const char* name) 
{
    entityHandle_T handle;
    handle.id   = _entities_top + 1;
    handle.slot = _entities_top;

    this->_entities[this->_entities_top] = Entity(name);
    this->_entity_handles[this->_entities_top] = handle;
    this->_entities_top += 1;

    // Add a spatial component by default because how can an entity exist in the world if it's not IN the world?
    this->entity_transforms.Add(handle);

    return handle;
}

entityHandle_T World::AddChildEntity(entityHandle_T parent)
{
    Entity* parent_entity = this->GetEntity(parent);
    entityHandle_T handle = this->AddEntity(parent_entity->name);

    Entity* entity = this->GetEntity(handle);
    entity->parent = parent;

    // Clone the spatial data from the parent
    this->entity_transforms.positions[handle.slot] = this->entity_transforms.positions[parent.slot];
    this->entity_transforms.rotations[handle.slot] = this->entity_transforms.rotations[parent.slot];
    this->entity_transforms.scales[handle.slot] = this->entity_transforms.scales[parent.slot];

    return handle;
}

Entity* World::GetEntity(entityHandle_T handle)
{
    if (handle.id != INVALID_HANDLE_ID && handle.slot <= this->_entities_top - 1)
    {
        return &this->_entities[handle.slot];
    }
    return 0;
}