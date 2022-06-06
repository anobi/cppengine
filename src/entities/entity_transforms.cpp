#include <omp.h>
#include <vector>
#include "entities/entity_transforms.hpp"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>


void EntityTransforms::Add(entityHandle_t entity)
{
    entitySlot_t resource = this->AllocateResource(entity);

    this->positions[resource.slot] = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->rotations[resource.slot] = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->scales[resource.slot] = glm::fvec3(1.0f, 1.0f, 1.0f);
}

// TODO: This belongs into a render/mesh component. Or the render world?
void EntityTransforms::Update(std::vector<entityHandle_t> entities, glm::fmat4 view_projection)
{
    #pragma omp parallel for
    for (int i = 0; i < entities.size(); i++) {
        entitySlot_t resource = this->FindResource(entities[i]);

        this->mvp_matrices[resource.slot] = view_projection * this->model_matrices[resource.slot];
        this->normal_matrices[resource.slot] = glm::inverse(glm::fmat3(this->model_matrices[resource.slot]));
    }
}

void EntityTransforms::UpdateModels()
{
    #pragma omp parallel for
    for (int i = 0; i < this->_model_update_queue.size(); i++) {
        unsigned int slot = this->_model_update_queue[i].slot;

        glm::fmat4 rotationMatrix =
            glm::rotate(glm::fmat4(1.0f), this->rotations[slot].x, glm::fvec3(1.0, 0.0, 0.0))
            * glm::rotate(glm::fmat4(1.0f), this->rotations[slot].y, glm::fvec3(0.0, 1.0, 0.0))
            * glm::rotate(glm::fmat4(1.0f), this->rotations[slot].z, glm::fvec3(0.0, 0.0, 1.0));

        this->model_matrices[slot] =
            glm::translate(glm::fmat4(1.0f), this->positions[slot])
            * rotationMatrix
            * glm::scale(glm::fmat4(1.0f), this->scales[slot]);
    }
    this->_model_update_queue.clear();
}

void EntityTransforms::SetPosition(entityHandle_t entity, glm::fvec3 position)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid()) {
        this->positions[resource.slot] = position;
        this->_model_update_queue.push_back(resource);
    }
}

void EntityTransforms::SetRotation(entityHandle_t entity, glm::fvec3 rotation)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid()) {
        this->rotations[resource.slot] = rotation;
        this->_model_update_queue.push_back(resource);
    }
}

void EntityTransforms::SetScale(entityHandle_t entity, glm::fvec3 scale)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid()) {
        this->scales[resource.slot] = scale;
        this->_model_update_queue.push_back(resource);
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

glm::fmat4 EntityTransforms::GetModel(entityHandle_t entity)
{
    entitySlot_t resource = this->FindResource(entity);
    if (resource.valid()) {
        return this->model_matrices[resource.slot];
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
