#ifndef __ENTITIES__ENTITY_HANDLE_H__
#define __ENTITIES__ENTITY_HANDLE_H__

#include <string>
#include "../containers/array.hpp"


constexpr int INVALID_HANDLE_ID = 0;
constexpr int INVALID_SLOT = -1;
constexpr int ENTITY_MAX_CHILDREN = 8;


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

struct resourceSlot_t {
    entityHandle_t entity;
    unsigned int slot;

    inline bool valid() {
        return (entity.valid() && slot != INVALID_SLOT);
    };
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