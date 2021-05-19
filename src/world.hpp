#ifndef __WORLD_H__
#define __WORLD_H__

#include <vector>
#include "constants.hpp"
#include "containers/array.hpp"

#include "entities/entity_manager.hpp"
#include "rendering/model_manager.hpp"
#include "rendering/material_manager.hpp"

class Camera;

class World {
public:
    World() { }

    ~World() 
    {
        this->_entity_handles.Clear();
        this->_entities_top = 0;

        this->camera            = nullptr;
        this->entity_manager    = nullptr;
        this->material_manager  = nullptr;
        this->model_manager     = nullptr;
    }

    void Cleanup() {
        this->camera = 0;
        this->_entity_handles.Clear();
    }

    void AddEntity(entityHandle_t entity);
    unsigned int EntityCount() const { return this->_entities_top; }


    Camera* camera = 0;


    // The world shall be the new temporary home for entities
    unsigned int _entities_top = 0;  // Time to make the Queue container soon?
    Array<entityHandle_t, MAX_GAME_ENTITIES> _entity_handles;

    Entities::EntityManager* entity_manager         = nullptr;
    Rendering::MaterialManager* material_manager    = nullptr;
    Rendering::ModelManager* model_manager          = nullptr;
};
#endif  // __WORLD_H__
