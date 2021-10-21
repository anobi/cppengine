#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "world.hpp"


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

        if (!model.valid()) continue;

        float radius = this->model_manager->bounding_sphere_radiuses[model.slot];
        // glm::fvec3 position = this->model_manager->object_centers[model.slot];
        glm::fvec3 position = this->entity_manager->spatial_components.GetPosition(entity);
        glm::fvec4 p = glm::fvec4(position.x, position.y, position.z, 1.0f);
        position = glm::fvec3(p.x, p.y, p.z);

        bool visible = true;
        for (int j = 0; j < 6; j++) {
            Plane plane = this->camera->frustum_planes[j];
            float dist = glm::dot(plane.n, position) + plane.d;
            if (dist <= -radius) {
                visible = false;
                break;
            }
        }

        if (visible) {
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
