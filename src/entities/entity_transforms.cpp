#include <vector>
#include "entity_transforms.hpp"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>


void EntityTransforms::Add(entityHandle_t entity)
{
    entitySlot_t resource = this->AllocateResource(entity);

    this->positions[resource.slot] = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->rotations[resource.slot] = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->scales[resource.slot] = glm::fvec3(1.0f, 1.0f, 1.0f);

    // All transforms start in a dirty state to force the initial update
    this->_dirty_entities[this->_dirty_entities_top] = entity;
    this->_dirty_entities_top += 1;
}

// TODO: This belongs into a render/mesh component. Or the render world?
void EntityTransforms::Update(glm::fmat4 view_projection, bool update_all)
{
    if (update_all) {
        this->_dirty_entities_top = 0;
        for (int i = 0; i < this->_entities_top; i++) {
            entityHandle_t entity = this->_entity_index[i].entity;
            this->_dirty_entities[i] = entity;
            this->_dirty_entities_top += 1;
        }
    }

    std::vector<entitySlot_t> dirty_index;
    for (int i = 0; i < this->_dirty_entities_top; i++) {
        entitySlot_t resource = this->FindResource(this->_dirty_entities[i]);
        dirty_index.push_back(resource);
    }

    for (int i = 0; i < dirty_index.size(); i++) {
        entitySlot_t resource = dirty_index[i];

        glm::fmat4 rotationMatrix = 
            glm::rotate(glm::fmat4(1.0f), this->rotations[resource.slot].x, glm::fvec3(1.0, 0.0, 0.0))
          * glm::rotate(glm::fmat4(1.0f), this->rotations[resource.slot].y, glm::fvec3(0.0, 1.0, 0.0))
          * glm::rotate(glm::fmat4(1.0f), this->rotations[resource.slot].z, glm::fvec3(0.0, 0.0, 1.0));

        this->model_matrices[resource.slot] =
            glm::translate(glm::fmat4(1.0f), this->positions[resource.slot] * this->scales[resource.slot])
          * rotationMatrix
          * glm::scale(glm::fmat4(1.0f), this->scales[resource.slot]);

        this->mvp_matrices[resource.slot] = view_projection * this->model_matrices[resource.slot];
        this->normal_matrices[resource.slot] = glm::inverse(glm::fmat3(this->model_matrices[resource.slot]));
    }

    this->_dirty_entities_top = 0;
}

void EntityTransforms::SetDirty(entityHandle_t entity)
{
    this->_dirty_entities[this->_dirty_entities_top] = entity;
    this->_dirty_entities_top += 1;
}

void EntityTransforms::SetPosition(entityHandle_t entity, glm::fvec3 position)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid()) {
        this->positions[resource.slot] = position;
        this->SetDirty(resource.entity);
    }
}

void EntityTransforms::SetRotation(entityHandle_t entity, glm::fvec3 rotation)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid()) {
        this->rotations[resource.slot] = rotation;
        this->SetDirty(resource.entity);
    }
}

void EntityTransforms::SetScale(entityHandle_t entity, glm::fvec3 scale)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid()) {
        this->scales[resource.slot] = scale;
        this->SetDirty(resource.entity);
    }
}

glm::fvec3 EntityTransforms::GetPosition(entityHandle_t entity)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid()) {
        return this->positions[resource.slot];
    }
}

glm::fvec3 EntityTransforms::GetRotation(entityHandle_t entity)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid()) {
        return this->rotations[resource.slot];
    }
}

glm::fvec3 EntityTransforms::GetScale(entityHandle_t entity)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid()) {
        return this->scales[resource.slot];
    }
}

entitySlot_t EntityTransforms::AllocateResource(entityHandle_t entity)
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

entitySlot_t EntityTransforms::FindResource(entityHandle_t entity)
{
    for (int i = 0; i < this->_entities_top; i++) {
        if (this->_entity_index[i].entity == entity) {
            return this->_entity_index[i];
        }
    }
    return entitySlot_t();
}
