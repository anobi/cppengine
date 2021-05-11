#include "entity_transforms.hpp"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>


void EntityTransforms::Add(entityHandle_t entity)
{
    this->_entities[entity.slot] = entity;
    if (entity.slot > this->_entities_top) {
        this->_entities_top = entity.slot + 1;
    }
    else {
        this->_entities_top += 1;
    }

    this->positions[entity.slot] = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->rotations[entity.slot] = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->scales[entity.slot] = glm::fvec3(1.0f, 1.0f, 1.0f);

    // All transforms start in a dirty state to force the initial update
    this->_dirty_entities[this->_dirty_entities_top] = entity;
    this->_dirty_entities_top += 1;
}

// TODO: This belongs into a render/mesh component. Or the render world?
void EntityTransforms::Update(glm::fmat4 view_projection, bool update_all)
{
    if (update_all) {
        this->_dirty_entities = this->_entities;
        this->_dirty_entities_top = this->_entities_top;
    }

    for (int i = 0; i < this->_dirty_entities_top; i++) {
        entityHandle_t entity = this->_dirty_entities[i];
        if (!entity.valid()) {
            continue;
        }

        glm::fmat4 rotationMatrix = 
            glm::rotate(glm::fmat4(1.0f), this->rotations[entity.slot].x, glm::fvec3(1.0, 0.0, 0.0))
          * glm::rotate(glm::fmat4(1.0f), this->rotations[entity.slot].y, glm::fvec3(0.0, 1.0, 0.0))
          * glm::rotate(glm::fmat4(1.0f), this->rotations[entity.slot].z, glm::fvec3(0.0, 0.0, 1.0));

        this->model_matrices[entity.slot] =
            glm::translate(glm::fmat4(1.0f), this->positions[entity.slot])
          * rotationMatrix
          * glm::scale(glm::fmat4(1.0f), this->scales[entity.slot]);

        this->mvp_matrices[entity.slot] = view_projection * this->model_matrices[entity.slot];
        this->normal_matrices[entity.slot] = glm::inverse(glm::fmat3(this->model_matrices[entity.slot]));
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
    if (entity.valid()) {
        this->positions[entity.slot] = position;
        this->SetDirty(entity);
    }
}

void EntityTransforms::SetRotation(entityHandle_t entity, glm::fvec3 rotation)
{
    if (entity.valid()) {
        this->rotations[entity.slot] = rotation;
        this->SetDirty(entity);
    }
}

void EntityTransforms::SetScale(entityHandle_t entity, glm::fvec3 scale)
{
    if (entity.valid()) {
        this->scales[entity.slot] = scale;
        this->SetDirty(entity);
    }
}

glm::fvec3 EntityTransforms::GetPosition(entityHandle_t entity)
{
    if (entity.valid()) {
        return this->positions[entity.slot];
    }
}

glm::fvec3 EntityTransforms::GetRotation(entityHandle_t entity)
{
    if (entity.valid()) {
        return this->rotations[entity.slot];
    }
}

glm::fvec3 EntityTransforms::GetScale(entityHandle_t entity)
{
    if (entity.valid()) {
        return this->scales[entity.slot];
    }
}

