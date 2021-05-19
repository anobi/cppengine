#include "entity_manager.hpp"

Entities::EntityManager::EntityManager()
{
}

Entities::EntityManager::~EntityManager()
{
}

entityHandle_t Entities::EntityManager::Add(const char* name)
{
    entityHandle_t existing = this->Find(name);
    if (existing.valid()) {
        return existing;
    }

    entityHandle_t entity = entityHandle_t();
    entity.id = this->_next_id;
    this->_next_id += 1;

    entitySlot_t resource = this->AllocateResource(entity);
    resource.name = name;
    this->names[resource.slot] = name;
    this->UpdateResource(resource);

    this->AddSpatialComponent(entity);

    return entity;
}

entityHandle_t Entities::EntityManager::Find(const char* name)
{
    for (int i = 0; i < this->_entities_top; i++) {
        int slot = this->_entity_index[i].slot;
        if (this->names[slot] == name) {
            return this->_entity_index[i].entity;
        }
    }
    return entityHandle_t();
}

entityHandle_t Entities::EntityManager::AddChild(entityHandle_t parent)
{
    entitySlot_t resource = this->FindResource(parent);
    if (resource.valid() && resource.num_children < ENTITY_MAX_CHILDREN) 
    {
        std::string name = resource.name + " [" + std::to_string(resource.num_children) + "]";
        entityHandle_t child = this->Add(name.c_str());

        if (resource.has_spatial_component) {
            this->spatial_components.Add(child);
        }

        entitySlot_t child_resource = this->FindResource(child);
        resource.children[resource.num_children] = child_resource.slot;
        resource.num_children += 1;
        
        this->UpdateResource(resource);
    }

    return entityHandle_t();
}

void Entities::EntityManager::AddSpatialComponent(entityHandle_t entity)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid()) {
        
        this->spatial_components.Add(resource.entity);

        resource.has_spatial_component = true;
        this->UpdateResource(resource);
    }
}

void Entities::EntityManager::SetPosition(entityHandle_t entity, glm::fvec3 position)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid() && resource.has_spatial_component) {
        this->spatial_components.SetPosition(entity, position);
    }
}

void Entities::EntityManager::SetRotation(entityHandle_t entity, glm::fvec3 rotation)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid() && resource.has_spatial_component) {
        this->spatial_components.SetRotation(entity, rotation);
    }
}

void Entities::EntityManager::SetScale(entityHandle_t entity, glm::fvec3 scale)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid() && resource.has_spatial_component) {
        this->spatial_components.SetScale(entity, scale);
    }
}


void Entities::EntityManager::AddPointLight(entityHandle_t entity, pointLight_t light_desc)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid()) {
        this->light_components.AddPointLight(resource.entity, light_desc);
        resource.has_pointlight_component = true;
        this->UpdateResource(resource);
    }
}

void Entities::EntityManager::AddDirectionalLight(entityHandle_t entity, dirLight_t light_desc)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid()) {
        this->light_components.AddDirectionalLight(resource.entity, light_desc);
        resource.has_dirlight_component = true;
        this->UpdateResource(resource);
    }
}

entitySlot_t Entities::EntityManager::AllocateResource(entityHandle_t entity)
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

entitySlot_t Entities::EntityManager::FindResource(entityHandle_t entity)
{
    for (int i = 0; i < this->_entities_top; i++) {
        if (this->_entity_index[i].entity == entity) {
            return this->_entity_index[i];
        }
    }
    return entitySlot_t();
}

void Entities::EntityManager::UpdateResource(entitySlot_t resource)
{
    for (int i = 0; i < this->_entities_top; i++) {
        if (this->_entity_index[i] == resource) {
             this->_entity_index[i] = resource;
        }
    }
}
