#ifndef __ENTITIES__ENTITY_MANAGER_H__
#define __ENTITIES__ENTITY_MANAGER_H__

#include "../constants.hpp"

#include "entity_handle.hpp"
#include "entity_transforms.hpp"
#include "entity_light_components.hpp"
#include "entity_model_components.hpp"

#include "../rendering/material_manager.hpp"


namespace Entities {
    class EntityManager {
    public:
        EntityManager();
        ~EntityManager();

        entityHandle_t Add(const char* name);
        entityHandle_t Find(const char* name);
        entityHandle_t AddChild(entityHandle_t parent);

        void AddSpatialComponent(entityHandle_t entity);
        void SetPosition(entityHandle_t entity, glm::fvec3 position);
        void SetRotation(entityHandle_t entity, glm::fvec3 rotation);
        void SetScale(entityHandle_t entity, glm::fvec3 scale);

        void AddPointLight(entityHandle_t entity, pointLight_t light_desc);
        void AddDirectionalLight(entityHandle_t entity, dirLight_t light_desc);

        void AddMaterialComponent(entityHandle_t entity, materialHandle_t material);

        Array<std::string, MAX_GAME_ENTITIES> names;

        unsigned int _next_id = 1;
        unsigned int _entities_top = 0;
        Array<entitySlot_t, MAX_GAME_ENTITIES> _entity_index;
        entitySlot_t AllocateResource(entityHandle_t entity);
        entitySlot_t FindResource(entityHandle_t entity);
        void UpdateResource(entitySlot_t resource);

        EntityTransforms spatial_components;
        EntityLightComponents light_components;
        Entities::ModelComponents model_components;
    };
}

#endif