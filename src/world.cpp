#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <SDL.h>
#include <omp.h>
#include "world.hpp"


void World::AddEntity(entityHandle_t entity)
{
    this->_entity_handles[this->_entities_top] = entity;
    this->_entities_top += 1;
}

std::vector<entityHandle_t> World::SphereFrustumCull()
{
    std::vector<entityHandle_t> entities;
    #pragma omp parallel 
    {
        std::vector<entityHandle_t> _entities;

        #pragma omp for nowait
        for (auto i = 0; i < this->_entities_top; i++)
        {
            entityHandle_t entity = this->_entity_handles[i];
            entitySlot_t model = this->model_manager->FindResource(entity);

            BoundingSphere bsphere = this->model_manager->bounding_spheres[model.slot];
            glm::fvec3 position = this->entity_manager->spatial_components.GetPosition(entity);

            auto cull_result = this->camera->view_frustum.SphereIntersect(position, bsphere.radius);
            if (cull_result != FrustumIntersectResult::OUT) {
                _entities.push_back(entity);
            }
        }

        #pragma omp critical
        entities.insert(entities.end(), _entities.begin(), _entities.end());
    }

    return entities;
}

std::vector<entityHandle_t> World::BoundingBoxFrustumCull()
{
    std::vector<entityHandle_t> input_entities;
    std::vector<entityHandle_t> entities;

    for (int i = 0; i < this->_entities_top; i++)
    {
        entityHandle_t entity = input_entities[i];
        AABB aabb = this->model_manager->AABBs[this->model_manager->FindResource(entity).slot];

        glm::fvec3 corners[8] = {
            glm::fvec3(aabb.min.x, aabb.min.y, aabb.min.z),
            glm::fvec3(aabb.max.x, aabb.min.y, aabb.min.z),
            glm::fvec3(aabb.min.x, aabb.max.y, aabb.min.z),
            glm::fvec3(aabb.max.x, aabb.max.y, aabb.min.z),

            glm::fvec3(aabb.min.x, aabb.min.y, aabb.max.z),
            glm::fvec3(aabb.max.x, aabb.min.y, aabb.max.z),
            glm::fvec3(aabb.min.x, aabb.max.y, aabb.max.z),
            glm::fvec3(aabb.max.x, aabb.max.y, aabb.max.z),
        };
    }

    return entities;
}
