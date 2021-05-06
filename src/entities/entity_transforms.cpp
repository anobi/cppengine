#include "entity_transforms.hpp"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>


void EntityTransforms::Add(entityHandle_T entity)
{
    this->_entities[entity.slot] = entity;
    this->_entities_top += 1;

    this->positions[entity.slot] = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->rotations[entity.slot] = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->scales[entity.slot] = glm::fvec3(1.0f, 1.0f, 1.0f);
}

void EntityTransforms::Update(glm::fmat4 view_projection)
{
    for (int i = 0; i < this->_entities_top; i++) {
        glm::fmat4 rotationMatrix = 
            glm::rotate(glm::fmat4(1.0f), this->rotations[i].x, glm::fvec3(1.0, 0.0, 0.0))
          * glm::rotate(glm::fmat4(1.0f), this->rotations[i].y, glm::fvec3(0.0, 1.0, 0.0))
          * glm::rotate(glm::fmat4(1.0f), this->rotations[i].z, glm::fvec3(0.0, 0.0, 1.0));

        this->model_matrices[i] =
            glm::translate(glm::fmat4(1.0f), this->positions[i]) 
          * rotationMatrix
          * glm::scale(glm::fmat4(1.0f), this->scales[i]);

        this->mvp_matrices[i] = view_projection * this->model_matrices[i];
        this->normal_matrices[i] = glm::inverse(glm::fmat3(this->model_matrices[i]));
    }
}

void EntityTransforms::SetPosition(entityHandle_T entity, glm::fvec3 position)
{
    if (entity.id != INVALID_HANDLE_ID && entity.slot != INVALID_SLOT) {
        this->positions[entity.slot] = position;
    }
}

void EntityTransforms::SetRotation(entityHandle_T entity, glm::fvec3 rotation)
{
    if (entity.id != INVALID_HANDLE_ID && entity.slot != INVALID_SLOT) {
        this->rotations[entity.slot] = rotation;
    }
}

