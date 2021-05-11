#ifndef __WORLD_H__
#define __WORLD_H__

#include "constants.hpp"
#include "containers/array.hpp"

#include "entity.hpp"
#include "entities/entity_handle.hpp"
#include "entities/entity_transforms.hpp"
#include "entities/entity_light_components.hpp"
#include "rendering/render_entities.hpp"
#include "rendering/render_material.hpp"

class Camera;

class World {
public:
    World() {}
    ~World() {}

    void Cleanup() {
        this->camera = 0;
        this->_entities.Clear();
    }

    entityHandle_t AddEntity(const char* name);
    entityHandle_t AddChildEntity(entityHandle_t parent);

    materialHandle_t AddMaterial(RenderMaterial material);
    
    void UpdateHandle(entityHandle_t handle);
    unsigned int EntityCount() const { return this->_entities_top; }

    entityHandle_t GetHandle(const char* name);
    Entity* GetEntity(entityHandle_t id);

    Camera* camera = 0;

    // The world shall be the new temporary home for entities
    unsigned int _entities_top = 0;  // Time to make the Queue container soon?
    Array<entityHandle_t, MAX_GAME_ENTITIES> _entity_handles;
    Array<Entity, MAX_GAME_ENTITIES> _entities;

    EntityTransforms entity_transforms;
    EntityLightComponents entity_lights;

    unsigned int _materials_top = 0;
    Array<materialHandle_t, MAX_GAME_ENTITIES> _material_handles;
    Array<RenderMaterial, MAX_GAME_ENTITIES> _materials;

    RenderEntities render_entities;
};
#endif  // __WORLD_H__
