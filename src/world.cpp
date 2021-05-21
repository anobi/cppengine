#include "world.hpp"

void World::AddEntity(entityHandle_t entity)
{
    this->_entity_handles[this->_entities_top] = entity;
    this->_entities_top += 1;
}

// https://www.lighthouse3d.com/tutorials/view-frustum-culling/clip-space-approach-extracting-the-planes/

glm::fvec4 normalize(glm::fvec4 v) 
{
    glm::fvec3 normal = glm::fvec3(v.x, v.y, v.z);
    float length = glm::length(normal);
    return -v / length;
}

std::vector<entityHandle_t> World::SphereFrustumCull()
{
    std::vector<entityHandle_t> entities;
    
    glm::mat4 view_projection = this->camera->GetViewProjection();
    glm::fvec4 planes[6] = {
        glm::fvec4(normalize(view_projection[3] + view_projection[0])),
        glm::fvec4(normalize(view_projection[3] - view_projection[0])),
        glm::fvec4(normalize(view_projection[3] + view_projection[1])),
        glm::fvec4(normalize(view_projection[3] - view_projection[1])),
        glm::fvec4(normalize(view_projection[3] + view_projection[2])),
        glm::fvec4(normalize(view_projection[3] - view_projection[2])),
    };

    for (int i = 0; i < this->_entities_top; i++)
    {
        entityHandle_t entity = this->_entity_handles[i];
        entitySlot_t model = this->model_manager->FindResource(entity);
        float radius = this->model_manager->bounding_sphere_radiuses[model.slot];
        glm::fvec3 position = this->model_manager->object_centers[model.slot];

        for (int j = 0; j < 6; j++) {
            glm::fvec4 plane = planes[j];
            float dist = plane.x * position.x + plane.y * position.y + plane.z * position.z + plane.w - radius;
            if (dist <= 0) {
                entities.push_back(entity);
                break;
            }
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
