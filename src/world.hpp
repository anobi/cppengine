#ifndef __WORLD_H__
#define __WORLD_H__

#include <vector>
#include "constants.hpp"
#include "containers/array.hpp"

#include "entities/entity_handle.hpp"
#include "entities/entity_transforms.hpp"
#include "entities/entity_light_components.hpp"
#include "rendering/render_entities.hpp"

class Camera;

class World {
public:
    World() {}
    ~World() {}

    void Cleanup() {
        this->camera = 0;
        this->_entity_handles.Clear();
    }

    entityHandle_t AddEntity(const char* name);
    entityHandle_t AddChildEntity(entityHandle_t parent);
    unsigned int EntityCount() const { return this->_entities_top; }
    entityHandle_t GetHandle(const char* name);


    Camera* camera = 0;


    // The world shall be the new temporary home for entities
    unsigned int _entities_top = 0;  // Time to make the Queue container soon?
    Array<entityHandle_t, MAX_GAME_ENTITIES> _entity_handles;

    EntityTransforms entity_transforms;
    EntityLightComponents entity_lights;

    RenderEntities render_entities;
};
#endif  // __WORLD_H__
