#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "world.hpp"

typedef enum {
    CULL_OUT    = 0,
    CULL_CLIP   = 1,
    CULL_IN     = 2
} CullStatus;


void World::AddEntity(entityHandle_t entity)
{
    this->_entity_handles[this->_entities_top] = entity;
    this->_entities_top += 1;
}

std::vector<entityHandle_t> World::SphereFrustumCull()
{
    std::vector<entityHandle_t> entities;
   
    for (int i = 0; i < this->_entities_top; i++)
    {
        entityHandle_t entity = this->_entity_handles[i];
        entitySlot_t model = this->model_manager->FindResource(entity);

        // Temporarily disable culling
        entities.push_back(entity);
        continue;

        float radius = this->model_manager->bounding_sphere_radiuses[model.slot];
        glm::fvec3 position = this->entity_manager->spatial_components.GetPosition(entity);
        int culled = false;
        for (int j = 0; j < 6; j++) {
            Plane frustum = this->camera->frustum_planes[j];
            float dist = glm::dot(position, frustum.n) - frustum.d;
            if (dist < -radius) {
                culled = CullStatus::CULL_OUT;
                break;
            }
            else if (dist <= radius) {
                culled = CullStatus::CULL_CLIP;
            }
        }

        if (culled != CullStatus::CULL_OUT) {
            entities.push_back(entity);
        }
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
