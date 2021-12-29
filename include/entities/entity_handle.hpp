#ifndef __ENTITIES__ENTITY_HANDLE_H__
#define __ENTITIES__ENTITY_HANDLE_H__

#include <string>
#include "../containers/array.hpp"


constexpr int INVALID_HANDLE_ID = 0;
constexpr int INVALID_SLOT = -1;
constexpr int ENTITY_MAX_CHILDREN = 1024;


struct entityHandle_t
{
    unsigned int id = INVALID_HANDLE_ID;

    inline bool valid() {
        return (id != INVALID_HANDLE_ID);
    };

    inline bool operator==(entityHandle_t other) {
        return this->id == other.id;
    }
};

struct entitySlot_t 
{
    entityHandle_t entity;
    int slot = INVALID_SLOT;

    unsigned int num_children = 0;
    unsigned int children[ENTITY_MAX_CHILDREN];

    bool has_spatial_component = false;
    bool has_pointlight_component = false;
    bool has_dirlight_component = false;
    bool has_model_component = false;
    bool has_material_component = false;

    std::string name = "Entity";

    inline bool valid() {
        return (entity.valid() && slot != INVALID_SLOT);
    };

    inline bool operator==(entitySlot_t other) {
        return this->entity == other.entity;
    }
};

class Entity 
{
    Entity() {};
    ~Entity() {};

    std::string name = "Entity";

    entityHandle_t handle;
    entityHandle_t parent;
    Array<entityHandle_t, ENTITY_MAX_CHILDREN> children;
};

#endif  // __ENTITIES__ENTITY_HANDLE_H__