#ifndef __ENTITIES__ENTITY_MODEL_COMPONENTS_H__
#define __ENTITIES__ENTITY_MODEL_COMPONENTS_H__

#include "../containers/array.hpp"
#include "../constants.hpp"

#include "entity_handle.hpp"
#include "../rendering/material_manager.hpp"
#include "../rendering/model_manager.hpp"


namespace Entities {
    class ModelComponents {
    public:
        ModelComponents();
        ~ModelComponents();

        void AddMaterial(entityHandle_t entity, materialHandle_t material);
        materialHandle_t GetMaterial(entityHandle_t entity);

        unsigned int _entities_top = 0;
        Array<entitySlot_t, MAX_GAME_ENTITIES> _entity_index;
        entitySlot_t AllocateResource(entityHandle_t entity);
        entitySlot_t FindResource(entityHandle_t entity);

        Array<materialHandle_t, MAX_GAME_ENTITIES> materials;
    };
}

#endif  // __ENTITIES__ENTITY_MODEL_COMPONENTS_H__