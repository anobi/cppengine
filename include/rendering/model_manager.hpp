#ifndef __RENDERING__MODEL_MANAGER_H__
#define __RENDERING__MODEL_MANAGER_H__

#include <vector>
#include "../constants.hpp"
#include "../containers/array.hpp"
#include "../types.hpp"
#include "../entities/entity_handle.hpp"

namespace Rendering 
{
    class ModelManager
    {
    public:
        ModelManager();
        ~ModelManager();

        void Add(entityHandle_t entity);
        void LoadModel(entityHandle_t entity, const std::vector<Vertex> vertices, const std::vector<unsigned int> indices);
        void Cleanup();

        unsigned int _entities_top = 0;
        Array<entitySlot_t, MAX_GAME_ENTITIES> _entity_index;
        entitySlot_t AllocateResource(entityHandle_t entity);
        entitySlot_t FindResource(entityHandle_t entity);

        Array<unsigned int, MAX_GAME_ENTITIES> VAOs;
        Array<unsigned int, MAX_GAME_ENTITIES> indices;

        Array<float, MAX_GAME_ENTITIES> bounding_sphere_radiuses;
        Array<glm::fvec3, MAX_GAME_ENTITIES> object_centers;  // Until the stuff can be properly positionalized
        Array<AABB, MAX_GAME_ENTITIES> AABBs;
    };
}


#endif  // __RENDERING__MODEL_MANAGER_H__