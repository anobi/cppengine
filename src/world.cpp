#include "world.hpp"

entityHandle_t World::AddEntity(const char* name) 
{
    entityHandle_t handle;
    handle.id   = _entities_top + 1;
    handle.slot = _entities_top;

    this->_entities[this->_entities_top] = Entity(name);
    this->_entity_handles[this->_entities_top] = handle;
    this->_entities_top += 1;

    // Add a spatial component by default because how can an entity exist in the world if it's not IN the world?
    this->entity_transforms.Add(handle);

    return handle;
}

entityHandle_t World::AddChildEntity(entityHandle_t parent)
{
    Entity* parent_entity = this->GetEntity(parent);
    entityHandle_t handle = this->AddEntity(parent_entity->name);

    Entity* entity = this->GetEntity(handle);
    entity->parent = parent;

    // Clone the spatial data from the parent
    this->entity_transforms.positions[handle.slot] = this->entity_transforms.positions[parent.slot];
    this->entity_transforms.rotations[handle.slot] = this->entity_transforms.rotations[parent.slot];
    this->entity_transforms.scales[handle.slot] = this->entity_transforms.scales[parent.slot];

    return handle;
}

// Hmmmm...
void World::UpdateHandle(entityHandle_t handle)
{
    if (handle.valid()) {
        this->_entity_handles[handle.slot] = handle;
    }
}

Entity* World::GetEntity(entityHandle_t handle)
{
    if (handle.id != INVALID_HANDLE_ID && handle.slot <= this->_entities_top - 1)
    {
        return &this->_entities[handle.slot];
    }
    return 0;
}