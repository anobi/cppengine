#ifndef __RENDERING__RENDER_ENTITIES_H__
#define __RENDERING__RENDER_ENTITIES_H__

#include <vector>
#include "../constants.hpp"
#include "../containers/array.hpp"
#include "../types.hpp"
#include "../entities/entity_handle.hpp"

class RenderEntities
{
public:
    RenderEntities() {};
    ~RenderEntities() {};

    void Add(entityHandle_t entity);
    void LoadModel(entityHandle_t entity, const std::vector<Vertex> vertices, const std::vector<unsigned int> indices);
    void Cleanup();

    unsigned int _entities_top = 0;
    Array<resourceSlot_t, MAX_GAME_ENTITIES> _entity_index;
    resourceSlot_t AllocateResource(entityHandle_t entity);
    resourceSlot_t FindResource(entityHandle_t entity);


    Array<unsigned int, MAX_RENDER_ENTITIES> VAOs;
    Array<unsigned int, MAX_RENDER_ENTITIES> indices;
};

#endif  // __RENDERING__RENDER_ENTITIES_H__