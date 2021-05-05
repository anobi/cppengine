#ifndef __WORLD_H__
#define __WORLD_H__

#include "constants.hpp"
#include "containers/array.hpp"
#include "entities/entity_handle.hpp"

#include "entities/entity_transforms.hpp"
#include "rendering/render_entities.hpp"

class Entity;
class Camera;

class World {
public:
    World() {}
    ~World() {}

    void Cleanup() {
        this->camera = 0;
        this->_entities.Clear();
    }

    entityHandle_T AddEntity(Entity* entity);

    unsigned int EntityCount() const { this->_entities_top; }

    Entity* GetEntity(entityHandle_T id);

    Camera* camera = 0;

    // Time to make the Queue container soon?
    unsigned int _entities_top = 0;
    Array<entityHandle_T, MAX_GAME_ENTITIES> _entity_handles;
    Array<Entity*, MAX_GAME_ENTITIES> _entities;

    EntityTransforms entity_transforms;
    RenderEntities render_entities;
};
#endif  // __WORLD_H__
