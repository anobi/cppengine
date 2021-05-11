#include "world.hpp"

entityHandle_t World::AddEntity(const char* name) 
{
    entityHandle_t handle;
    handle.id   = _entities_top + 1;
    handle.slot = _entities_top;
    handle.name = name;

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

materialHandle_t World::AddMaterial(RenderMaterial material)
{
    for (int i = 0; i < this->_materials_top; i++) {
        if (this->_material_handles[i].name == material.name) {
            return this->_material_handles[i];
        }
    }
    materialHandle_t handle;
    handle.id = this->_materials_top + 1;
    handle.slot = this->_materials_top;
    handle.name = material.name;

    this->_materials[this->_materials_top] = material;
    this->_material_handles[this->_materials_top] = handle;
    this->_materials_top += 1;

    return handle;
}

// Hmmmm...
void World::UpdateHandle(entityHandle_t handle)
{
    if (handle.valid()) {
        this->_entity_handles[handle.slot] = handle;
    }
}

entityHandle_t World::GetHandle(const char* name)
{
    for (int i = 0; i < this->_entities_top; i++) {
        if (this->_entity_handles[i].name == name) {
            return this->_entity_handles[i];
        }
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